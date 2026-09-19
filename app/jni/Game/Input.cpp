#include "Input.h"

Input::Input() 
    : screenWidth(800)
    , screenHeight(480)
{
}

Input::~Input() {
}

void Input::setScreenSize(float width, float height) {
    screenWidth = width;
    screenHeight = height;
}

void Input::addTouch(int id, float x, float y) {
    TouchPoint point;
    point.id = id;
    point.x = x;
    point.y = y;
    point.prevX = x;
    point.prevY = y;
    point.isPressed = true;
    touchPoints.push_back(point);
}

void Input::moveTouch(int id, float x, float y) {
    for (auto& point : touchPoints) {
        if (point.id == id) {
            point.prevX = point.x;
            point.prevY = point.y;
            point.x = x;
            point.y = y;
            break;
        }
    }
}

void Input::removeTouch(int id) {
    for (auto it = touchPoints.begin(); it != touchPoints.end(); ++it) {
        if (it->id == id) {
            touchPoints.erase(it);
            return;
        }
    }
}

void Input::clearTouches() {
    touchPoints.clear();
}

bool Input::isSplitScreen() const {
    // Check if there are touches on both sides of screen
    bool leftSide = false;
    bool rightSide = false;
    
    for (const auto& point : touchPoints) {
        if (point.x < screenWidth / 2) {
            leftSide = true;
        } else {
            rightSide = true;
        }
    }
    
    return leftSide && rightSide;
}
