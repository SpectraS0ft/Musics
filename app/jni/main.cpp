#include <android/native_activity.h>
#include <android/input.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <cstdlib>
#include <ctime>

#include "Game/Game.h"
#include "Utils/Logger.h"

static Game* game = nullptr;
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLSurface surface = EGL_NO_SURFACE;
static EGLContext context = EGL_NO_CONTEXT;
static bool running = false;

// Initialize EGL
bool initEGL(ANativeWindow* window) {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (display == EGL_NO_DISPLAY) {
        LOGE("Failed to get EGL display");
        return false;
    }
    
    eglInitialize(display, 0, 0);
    
    EGLConfig config;
    EGLint numConfigs;
    EGLint attribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };
    
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    
    surface = eglCreateWindowSurface(display, config, window, NULL);
    if (surface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface");
        return false;
    }
    
    EGLint ctxAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxAttribs);
    if (context == EGL_NO_CONTEXT) {
        LOGE("Failed to create EGL context");
        return false;
    }
    
    eglMakeCurrent(display, surface, surface, context);
    
    return true;
}

void cleanupEGL() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
}

void handleInput(AInputEvent* event) {
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
        int32_t action = AMotionEvent_getAction(event);
        int32_t pointerIndex = (action >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT) 
                               & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK;
        action &= AMOTION_EVENT_ACTION_MASK;
        
        size_t pointerCount = AMotionEvent_getPointerCount(event);
        
        for (size_t i = 0; i < pointerCount; i++) {
            int32_t id = AMotionEvent_getPointerId(event, i);
            float x = AMotionEvent_getX(event, i);
            float y = AMotionEvent_getY(event, i);
            
            switch (action) {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                    if (game) game->onTouchBegin(id, x, y);
                    break;
                    
                case AMOTION_EVENT_ACTION_MOVE:
                    if (game) game->onTouchMove(id, x, y);
                    break;
                    
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                    if (game) game->onTouchEnd(id, x, y);
                    break;
            }
        }
    }
}

void engine_term_display() {
    running = false;
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }
}

void engine_draw_frame() {
    if (display == EGL_NO_DISPLAY) return;
    
    static double lastTime = 0;
    double currentTime = clock() / (double)CLOCKS_PER_SEC;
    float deltaTime = (float)(currentTime - lastTime);
    lastTime = currentTime;
    
    if (game) {
        game->update(deltaTime);
        game->render();
    }
    
    eglSwapBuffers(display, surface);
}

void engine_init_display() {
    // Display initialization handled by initEGL
}

void android_main(struct android_app* app) {
    srand(time(NULL));
    
    app->onAppCmd = [](struct android_app* app, int32_t cmd) {
        switch (cmd) {
            case APP_CMD_INIT_WINDOW:
                if (app->window != NULL) {
                    if (initEGL(app->window)) {
                        if (!game) {
                            game = new Game();
                            if (game->initialize()) {
                                int32_t width = ANativeWindow_getWidth(app->window);
                                int32_t height = ANativeWindow_getHeight(app->window);
                                game->setScreenSize(width, height);
                            }
                        }
                        running = true;
                    }
                }
                break;
                
            case APP_CMD_TERM_WINDOW:
                engine_term_display();
                break;
                
            case APP_CMD_GAINED_FOCUS:
                if (display != EGL_NO_DISPLAY) {
                    eglMakeCurrent(display, surface, surface, context);
                }
                break;
                
            case APP_CMD_LOST_FOCUS:
                if (display != EGL_NO_DISPLAY) {
                    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                }
                break;
        }
    };
    
    app->onInputEvent = [](struct android_app* app, AInputEvent* event) -> int32_t {
        handleInput(event);
        return 1;
    };
    
    while (true) {
        int ident;
        int events;
        struct android_poll_source* source;
        
        while ((ident = ALooper_pollAll(running ? 0 : -1, NULL, &events, 
                                        (void**)&source)) >= 0) {
            if (source != NULL) {
                source->process(app, source);
            }
            
            if (app->destroyRequested != 0) {
                engine_term_display();
                goto cleanup;
            }
        }
        
        if (running && display != EGL_NO_DISPLAY) {
            engine_draw_frame();
        }
    }
    
cleanup:
    if (game) {
        delete game;
        game = nullptr;
    }
    cleanupEGL();
}
