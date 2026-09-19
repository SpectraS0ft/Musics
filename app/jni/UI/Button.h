#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include <functional>

struct ButtonStyle {
    float width;
    float height;
    float cornerRadius;
    
    // Colors (RGBA)
    float bgColor[4];
    float textColor[4];
    float hoverColor[4];
    float pressedColor[4];
    
    float fontSize;
    std::string fontName;
    
    ButtonStyle() 
        : width(200), height(60), cornerRadius(10)
        , fontSize(24.0f), fontName("default")
    {
        // Default blue button
        bgColor[0] = 0.2f; bgColor[1] = 0.4f; bgColor[2] = 0.8f; bgColor[3] = 1.0f;
        textColor[0] = 1.0f; textColor[1] = 1.0f; textColor[2] = 1.0f; textColor[3] = 1.0f;
        hoverColor[0] = 0.3f; hoverColor[1] = 0.5f; hoverColor[2] = 0.9f; hoverColor[3] = 1.0f;
        pressedColor[0] = 0.1f; pressedColor[1] = 0.3f; pressedColor[2] = 0.7f; pressedColor[3] = 1.0f;
    }
};

class Button {
private:
    std::string text;
    std::string textUzbek; // Uzbek translation
    
    float x, y; // Position (top-left corner)
    ButtonStyle style;
    
    bool isHovered;
    bool isPressed;
    bool isVisible;
    bool isEnabled;
    
    std::function<void()> onClickCallback;
    
public:
    Button();
    Button(const std::string& txt, float posX, float posY);
    ~Button();
    
    // Getters
    std::string getText() const { return text; }
    std::string getTextUzbek() const { return textUzbek; }
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return style.width; }
    float getHeight() const { return style.height; }
    bool getIsVisible() const { return isVisible; }
    bool getIsEnabled() const { return isEnabled; }
    
    // Setters
    void setText(const std::string& txt);
    void setTextUzbek(const std::string& txt);
    void setPosition(float posX, float posY);
    void setSize(float w, float h);
    void setIsVisible(bool visible) { isVisible = visible; }
    void setIsEnabled(bool enabled) { isEnabled = enabled; }
    void setStyle(const ButtonStyle& s) { style = s; }
    
    // Callbacks
    void setOnClick(std::function<void()> callback);
    
    // Interaction
    bool isPointInside(float px, float py) const;
    void onTouchBegin(float px, float py);
    void onTouchEnd(float px, float py);
    void update(float deltaTime);
    
    // Render (called by Renderer)
    void render() const;
    
    // Predefined button styles
    static ButtonStyle getDefaultStyle();
    static ButtonStyle getMenuStyle();
    static ButtonStyle getSmallStyle();
};

// Uzbek translations for common buttons
namespace UzbekButtons {
    inline constexpr const char* PLAY = "O'ynash";
    inline constexpr const char* OPTIONS = "Sozlamalar";
    inline constexpr const char* QUIT = "Chiqish";
    inline constexpr const char* BACK = "Ortga";
    inline constexpr const char* START = "Boshlash";
    inline constexpr const char* PAUSE = "Tanaffus";
    inline constexpr const char* RESUME = "Davom etish";
    inline constexpr const char* RESTART = "Qayta boshlash";
    inline constexpr const char* DIFFICULTY = "Qiyinchilik";
    inline constexpr const char* MAPS = "Xaritalar";
    inline constexpr const char* MODE = "Rejim";
    inline constexpr const char* UPGRADES = "Yangilashlar";
    inline constexpr const char* MULTIPLAYER = "Ko'p o'yinchi";
    inline constexpr const char* SINGLE_PLAYER = "Yakka o'yinchi";
    inline constexpr const char* EASY = "Oson";
    inline constexpr const char* MEDIUM = "O'rta";
    inline constexpr const char* HARD = "Qiyin";
}

#endif // BUTTON_H
