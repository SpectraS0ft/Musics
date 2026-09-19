#ifndef INPUT_H
#define INPUT_H

#include <vector>

struct TouchPoint {
    int id;
    float x, y;
    float prevX, prevY;
    bool isPressed;
    
    TouchPoint() : id(-1), x(0), y(0), prevX(0), prevY(0), isPressed(false) {}
};

class Input {
private:
    std::vector<TouchPoint> touchPoints;
    float screenWidth, screenHeight;
    
public:
    Input();
    ~Input();
    
    void setScreenSize(float width, float height);
    float getScreenWidth() const { return screenWidth; }
    float getScreenHeight() const { return screenHeight; }
    
    void addTouch(int id, float x, float y);
    void moveTouch(int id, float x, float y);
    void removeTouch(int id);
    void clearTouches();
    
    const std::vector<TouchPoint>& getTouchPoints() const { return touchPoints; }
    
    bool isSplitScreen() const;
};

#endif // INPUT_H
