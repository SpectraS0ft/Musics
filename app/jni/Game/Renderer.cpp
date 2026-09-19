#include "Renderer.h"

Renderer::Renderer() 
    : screenWidth(800)
    , screenHeight(480)
    , shaderProgram(0)
    , vao(0)
    , vbo(0)
{
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create simple shader program (placeholder)
    // In real implementation, load and compile shaders
    
    return true;
}

void Renderer::shutdown() {
    if (shaderProgram != 0) {
        glDeleteProgram(shaderProgram);
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
    }
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
    }
}

void Renderer::setScreenSize(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    
    float aspect = (float)width / (float)height;
    projectionMatrix = Math3D::Matrix4::perspective(
        3.14159f / 3.0f, aspect, 0.1f, 1000.0f);
}

void Renderer::setupCamera(const Math3D::Vector3& eye, 
                          const Math3D::Vector3& target,
                          const Math3D::Vector3& up) {
    viewMatrix = Math3D::Matrix4::lookAt(eye, target, up);
}

void Renderer::beginFrame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::endFrame() {
    // Swap buffers handled by Android activity
}

void Renderer::renderShip(const Math3D::Vector3& pos, 
                         const Math3D::Quaternion& rot,
                         float healthPercent, bool isPlayer) {
    // Render ship model with position and rotation
    // Color based on team (blue for player, red for enemy)
}

void Renderer::renderMenu() {
    // Render UI menu elements
}

void Renderer::renderHUD() {
    // Render heads-up display
}

void Renderer::renderSplitScreenDivider() {
    // Draw vertical line in middle of screen using OpenGL ES 3.0
    glDisable(GL_DEPTH_TEST);
    
    // Simple shader-based line rendering would go here
    // For now, this is a placeholder - in real implementation,
    // you would use a vertex buffer and shader to draw the line
    
    GLfloat vertices[] = {
        screenWidth/2.0f, 0.0f, 0.0f,
        screenWidth/2.0f, (float)screenHeight, 0.0f
    };
    
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Enable vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Draw line
    glDrawArrays(GL_LINES, 0, 2);
    
    // Cleanup
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    
    glEnable(GL_DEPTH_TEST);
}

void Renderer::setupSplitScreenLeft() {
    glViewport(0, 0, screenWidth/2, screenHeight);
}

void Renderer::setupSplitScreenRight() {
    glViewport(screenWidth/2, 0, screenWidth/2, screenHeight);
}
