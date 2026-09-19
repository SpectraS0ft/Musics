# Android NDK Build Configuration
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := spacebattle
LOCAL_SRC_FILES := main.cpp \
                   Game/Game.cpp \
                   Game/Renderer.cpp \
                   Game/Input.cpp \
                   Entities/Player.cpp \
                   Entities/Bot.cpp \
                   Entities/Ship.cpp \
                   AI/BotAI.cpp \
                   UI/Menu.cpp \
                   UI/HUD.cpp \
                   UI/Button.cpp \
                   World/Map.cpp \
                   World/Texture.cpp \
                   Utils/Logger.cpp \
                   Utils/Math3D.cpp

LOCAL_C_INCLUDES := $(NDK)/sources/android/native_app_glue
LOCAL_CFLAGS    := -Wall -Werror
LOCAL_CPPFLAGS  := -std=c++17 -frtti -fexceptions
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv3 -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
