#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <GLES3/gl3.h>

struct TextureInfo {
    GLuint id;
    int width, height;
    GLenum format;
    std::string name;
};

class Texture {
private:
    std::vector<TextureInfo> textures;
    
public:
    Texture();
    ~Texture();
    
    bool loadTexture(const std::string& filename);
    bool loadCubemap(const std::vector<std::string>& filenames);
    void unload();
    
    void bind(int unit = 0) const;
    void unbind() const;
    
    GLuint getTextureId(int index = 0) const;
    int getWidth() const;
    int getHeight() const;
    
    // Procedural texture generation
    static TextureInfo generateCheckerboard(int size, int squares);
    static TextureInfo generateNoise(int width, int height);
    static TextureInfo generateGradient(const char* type);
};

#endif // TEXTURE_H
