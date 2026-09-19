#ifndef RENDERER_H
#define RENDERER_H

#include "../Utils/Math3D.h"
#include <GLES3/gl3.h>

class Renderer {
private:
    int screenWidth, screenHeight;
    Math3D::Matrix4 projectionMatrix;
    Math3D::Matrix4 viewMatrix;
    
    GLuint shaderProgram;
    GLuint vao, vbo;
    
public:
    Renderer();
    ~Renderer();
    
    bool initialize();
    void shutdown();
    
    void setScreenSize(int width, int height);
    void setupCamera(const Math3D::Vector3& eye, const Math3D::Vector3& target, 
                    const Math3D::Vector3& up);
    
    void beginFrame();
    void endFrame();
    
    void renderShip(const Math3D::Vector3& pos, const Math3D::Quaternion& rot,
                   float healthPercent, bool isPlayer);
    void renderMenu();
    void renderHUD();
    void renderSplitScreenDivider();
    
    // Split screen rendering
    void setupSplitScreenLeft();
    void setupSplitScreenRight();
};
#endif // RENDERER_H
