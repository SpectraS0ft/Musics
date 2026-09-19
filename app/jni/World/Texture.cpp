#include "Texture.h"

Texture::Texture() {
}

Texture::~Texture() {
    unload();
}

bool Texture::loadTexture(const std::string& filename) {
    // In real implementation, load image file
    // For now, generate procedural texture
    TextureInfo info = generateCheckerboard(256, 8);
    info.name = filename;
    textures.push_back(info);
    return true;
}

bool Texture::loadCubemap(const std::vector<std::string>& filenames) {
    // Cubemap loading for skybox
    return true;
}

void Texture::unload() {
    for (auto& tex : textures) {
        if (tex.id != 0) {
            glDeleteTextures(1, &tex.id);
        }
    }
    textures.clear();
}

void Texture::bind(int unit) const {
    if (!textures.empty()) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, textures[0].id);
    }
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTextureId(int index) const {
    if (index < textures.size()) {
        return textures[index].id;
    }
    return 0;
}

int Texture::getWidth() const {
    if (!textures.empty()) {
        return textures[0].width;
    }
    return 0;
}

int Texture::getHeight() const {
    if (!textures.empty()) {
        return textures[0].height;
    }
    return 0;
}

TextureInfo Texture::generateCheckerboard(int size, int squares) {
    TextureInfo info;
    info.width = size;
    info.height = size;
    info.format = GL_RGBA;
    
    std::vector<unsigned char> pixels(size * size * 4);
    int squareSize = size / squares;
    
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int idx = (y * size + x) * 4;
            bool isWhite = ((x / squareSize) + (y / squareSize)) % 2 == 0;
            
            if (isWhite) {
                pixels[idx] = 255;     // R
                pixels[idx+1] = 255;   // G
                pixels[idx+2] = 255;   // B
                pixels[idx+3] = 255;   // A
            } else {
                pixels[idx] = 64;      // R
                pixels[idx+1] = 64;    // G
                pixels[idx+2] = 128;   // B
                pixels[idx+3] = 255;   // A
            }
        }
    }
    
    glGenTextures(1, &info.id);
    glBindTexture(GL_TEXTURE_2D, info.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, 
                 GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    return info;
}

TextureInfo Texture::generateNoise(int width, int height) {
    TextureInfo info;
    info.width = width;
    info.height = height;
    info.format = GL_RGBA;
    
    std::vector<unsigned char> pixels(width * height * 4);
    
    for (int i = 0; i < width * height * 4; i++) {
        pixels[i] = rand() % 256;
    }
    
    glGenTextures(1, &info.id);
    glBindTexture(GL_TEXTURE_2D, info.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return info;
}

TextureInfo Texture::generateGradient(const char* type) {
    TextureInfo info;
    int size = 256;
    info.width = size;
    info.height = size;
    info.format = GL_RGBA;
    
    std::vector<unsigned char> pixels(size * size * 4);
    
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int idx = (y * size + x) * 4;
            
            if (strcmp(type, "horizontal") == 0) {
                pixels[idx] = x;       // R
                pixels[idx+1] = 0;     // G
                pixels[idx+2] = 255-x; // B
                pixels[idx+3] = 255;   // A
            } else {
                pixels[idx] = y;       // R
                pixels[idx+1] = x;     // G
                pixels[idx+2] = 128;   // B
                pixels[idx+3] = 255;   // A
            }
        }
    }
    
    glGenTextures(1, &info.id);
    glBindTexture(GL_TEXTURE_2D, info.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return info;
}
