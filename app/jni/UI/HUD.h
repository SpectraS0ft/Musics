#ifndef HUD_H
#define HUD_H

#include "../Utils/Math3D.h"
#include <string>
#include <vector>

struct JoystickState {
    float x, y; // Position
    float deltaX, deltaY; // Movement delta
    bool isTouching;
    int touchId;
    
    JoystickState() : x(0), y(0), deltaX(0), deltaY(0), 
                      isTouching(false), touchId(-1) {}
};

struct ActionButton {
    std::string label;
    std::string labelUzbek;
    float x, y, size;
    bool isPressed;
    int actionId;
    
    ActionButton() : x(0), y(0), size(60), isPressed(false), actionId(0) {}
};

enum class HUDMode {
    SINGLE_PLAYER,
    SPLIT_SCREEN_P1,
    SPLIT_SCREEN_P2
};

class HUD {
private:
    HUDMode mode;
    
    // Player 1 controls (left side)
    JoystickState joystick1;
    std::vector<ActionButton> buttons1;
    
    // Player 2 controls (right side, split-screen only)
    JoystickState joystick2;
    std::vector<ActionButton> buttons2;
    
    // Display elements
    float healthBarWidth;
    float healthBarHeight;
    float scoreX, scoreY;
    
    // State
    bool isVisible;
    int currentHealth;
    int maxHealth;
    int score;
    int ammo;
    float gameTime;
    
public:
    HUD();
    ~HUD();
    
    // Setup
    void setupSinglePlayer();
    void setupSplitScreen();
    void setMode(HUDMode m) { mode = m; }
    
    // Getters
    JoystickState getJoystick1() const { return joystick1; }
    JoystickState getJoystick2() const { return joystick2; }
    bool getIsVisible() const { return isVisible; }
    
    // Setters
    void setIsVisible(bool visible) { isVisible = visible; }
    void setHealth(int current, int max);
    void setScore(int s) { score = s; }
    void setAmmo(int a) { ammo = a; }
    void setGameTime(float time) { gameTime = time; }
    
    // Touch handling
    void onTouchBegin(int touchId, float x, float y);
    void onTouchMove(int touchId, float x, float y);
    void onTouchEnd(int touchId, float x, float y);
    
    // Update
    void update(float deltaTime);
    
    // Render
    void render() const;
    void renderHealthBar() const;
    void renderJoystick(const JoystickState& state) const;
    void renderActionButtons(const std::vector<ActionButton>& buttons) const;
    void renderScore() const;
    
    // Split screen helpers
    bool isPointInLeftScreen(float x) const;
    bool isPointInRightScreen(float x) const;
    
    // Uzbek labels
    static const char* getUzbekLabel(const std::string& english);
};

#endif // HUD_H
