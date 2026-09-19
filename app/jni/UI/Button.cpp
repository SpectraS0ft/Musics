#include "Button.h"

Button::Button() 
    : text("Button")
    , textUzbek("")
    , x(0), y(0)
    , isHovered(false)
    , isPressed(false)
    , isVisible(true)
    , isEnabled(true)
{
}

Button::Button(const std::string& txt, float posX, float posY)
    : text(txt)
    , textUzbek("")
    , x(posX)
    , y(posY)
    , isHovered(false)
    , isPressed(false)
    , isVisible(true)
    , isEnabled(true)
{
}

Button::~Button() {
}

void Button::setText(const std::string& txt) {
    text = txt;
}

void Button::setTextUzbek(const std::string& txt) {
    textUzbek = txt;
}

void Button::setPosition(float posX, float posY) {
    x = posX;
    y = posY;
}

void Button::setSize(float w, float h) {
    style.width = w;
    style.height = h;
}

void Button::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

bool Button::isPointInside(float px, float py) const {
    return px >= x && px <= x + style.width &&
           py >= y && py <= y + style.height;
}

void Button::onTouchBegin(float px, float py) {
    if (isEnabled && isVisible && isPointInside(px, py)) {
        isPressed = true;
    }
}

void Button::onTouchEnd(float px, float py) {
    if (isPressed && isEnabled && isVisible) {
        if (isPointInside(px, py) && onClickCallback) {
            onClickCallback();
        }
    }
    isPressed = false;
}

void Button::update(float deltaTime) {
    // Update hover state (would need touch input tracking)
    // Simplified for now
}

void Button::render() const {
    // Rendering is done by Renderer class
    // This is a placeholder
}

ButtonStyle Button::getDefaultStyle() {
    return ButtonStyle();
}

ButtonStyle Button::getMenuStyle() {
    ButtonStyle style;
    style.width = 300;
    style.height = 80;
    style.cornerRadius = 15;
    style.fontSize = 32.0f;
    return style;
}

ButtonStyle Button::getSmallStyle() {
    ButtonStyle style;
    style.width = 150;
    style.height = 50;
    style.cornerRadius = 8;
    style.fontSize = 20.0f;
    return style;
}
