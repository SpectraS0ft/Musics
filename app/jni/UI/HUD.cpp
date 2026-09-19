#include "HUD.h"

HUD::HUD() 
    : mode(HUDMode::SINGLE_PLAYER)
    , healthBarWidth(200)
    , healthBarHeight(20)
    , scoreX(10)
    , scoreY(10)
    , isVisible(true)
    , currentHealth(100)
    , maxHealth(100)
    , score(0)
    , ammo(100)
    , gameTime(0)
{
}

HUD::~HUD() {
}

void HUD::setupSinglePlayer() {
    mode = HUDMode::SINGLE_PLAYER;
    
    // Left side joystick for movement
    joystick1.x = 100;
    joystick1.y = 400;
    
    // Right side action buttons
    ActionButton fireBtn;
    fireBtn.label = "Fire";
    fireBtn.labelUzbek = "O't ochish";
    fireBtn.x = 700;
    fireBtn.y = 300;
    fireBtn.size = 80;
    fireBtn.actionId = 1;
    buttons1.push_back(fireBtn);
    
    ActionButton boostBtn;
    boostBtn.label = "Boost";
    boostBtn.labelUzbek = "Tezlatish";
    boostBtn.x = 600;
    boostBtn.y = 450;
    boostBtn.size = 60;
    boostBtn.actionId = 2;
    buttons1.push_back(boostBtn);
}

void HUD::setupSplitScreen() {
    // Player 1 (left half)
    joystick1.x = 50;
    joystick1.y = 300;
    
    ActionButton fireBtn1;
    fireBtn1.label = "Fire";
    fireBtn1.labelUzbek = "O't ochish";
    fireBtn1.x = 300;
    fireBtn1.y = 250;
    fireBtn1.size = 70;
    fireBtn1.actionId = 1;
    buttons1.push_back(fireBtn1);
    
    // Player 2 (right half)
    joystick2.x = 500;
    joystick2.y = 300;
    
    ActionButton fireBtn2;
    fireBtn2.label = "Fire";
    fireBtn2.labelUzbek = "O't ochish";
    fireBtn2.x = 750;
    fireBtn2.y = 250;
    fireBtn2.size = 70;
    fireBtn2.actionId = 1;
    buttons2.push_back(fireBtn2);
}

void HUD::setHealth(int current, int max) {
    currentHealth = current;
    maxHealth = max;
}

void HUD::onTouchBegin(int touchId, float x, float y) {
    // Check joystick 1
    float dx = x - joystick1.x;
    float dy = y - joystick1.y;
    if (dx*dx + dy*dy < 2500) { // 50px radius
        joystick1.isTouching = true;
        joystick1.touchId = touchId;
        return;
    }
    
    // Check buttons 1
    for (auto& btn : buttons1) {
        float dx = x - (btn.x + btn.size/2);
        float dy = y - (btn.y + btn.size/2);
        if (dx*dx + dy*dy < (btn.size/2)*(btn.size/2)) {
            btn.isPressed = true;
            return;
        }
    }
    
    // Split screen - check player 2 controls
    if (mode != HUDMode::SINGLE_PLAYER) {
        float dx2 = x - joystick2.x;
        float dy2 = y - joystick2.y;
        if (dx2*dx2 + dy2*dy2 < 2500) {
            joystick2.isTouching = true;
            joystick2.touchId = touchId;
            return;
        }
        
        for (auto& btn : buttons2) {
            float dx = x - (btn.x + btn.size/2);
            float dy = y - (btn.y + btn.size/2);
            if (dx*dx + dy*dy < (btn.size/2)*(btn.size/2)) {
                btn.isPressed = true;
                return;
            }
        }
    }
}

void HUD::onTouchMove(int touchId, float x, float y) {
    if (touchId == joystick1.touchId && joystick1.isTouching) {
        joystick1.deltaX = x - joystick1.x;
        joystick1.deltaY = y - joystick1.y;
        
        // Clamp to joystick radius
        float dist = std::sqrt(joystick1.deltaX*joystick1.deltaX + 
                              joystick1.deltaY*joystick1.deltaY);
        if (dist > 50) {
            float scale = 50.0f / dist;
            joystick1.deltaX *= scale;
            joystick1.deltaY *= scale;
        }
    }
    
    if (touchId == joystick2.touchId && joystick2.isTouching) {
        joystick2.deltaX = x - joystick2.x;
        joystick2.deltaY = y - joystick2.y;
        
        float dist = std::sqrt(joystick2.deltaX*joystick2.deltaX + 
                              joystick2.deltaY*joystick2.deltaY);
        if (dist > 50) {
            float scale = 50.0f / dist;
            joystick2.deltaX *= scale;
            joystick2.deltaY *= scale;
        }
    }
}

void HUD::onTouchEnd(int touchId, float x, float y) {
    if (touchId == joystick1.touchId) {
        joystick1.isTouching = false;
        joystick1.touchId = -1;
        joystick1.deltaX = 0;
        joystick1.deltaY = 0;
    }
    
    if (touchId == joystick2.touchId) {
        joystick2.isTouching = false;
        joystick2.touchId = -1;
        joystick2.deltaX = 0;
        joystick2.deltaY = 0;
    }
    
    for (auto& btn : buttons1) {
        if (btn.isPressed) {
            btn.isPressed = false;
        }
    }
    
    for (auto& btn : buttons2) {
        if (btn.isPressed) {
            btn.isPressed = false;
        }
    }
}

void HUD::update(float deltaTime) {
    gameTime += deltaTime;
}

void HUD::render() const {
    // Rendering handled by Renderer
}

void HUD::renderHealthBar() const {
    // Health bar rendering
}

void HUD::renderJoystick(const JoystickState& state) const {
    // Joystick rendering
}

void HUD::renderActionButtons(const std::vector<ActionButton>& buttons) const {
    // Button rendering
}

void HUD::renderScore() const {
    // Score rendering
}

bool HUD::isPointInLeftScreen(float x) const {
    return x < 400; // Assuming 800px width
}

bool HUD::isPointInRightScreen(float x) const {
    return x >= 400;
}

const char* HUD::getUzbekLabel(const std::string& english) {
    if (english == "Fire") return "O't ochish";
    if (english == "Boost") return "Tezlatish";
    if (english == "Health") return "Sog'lik";
    if (english == "Score") return "Hisob";
    if (english == "Ammo") return "O'q-dori";
    return english.c_str();
}
