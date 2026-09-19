#include "Bot.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

Bot::Bot() 
    : Ship()
    , difficulty(Difficulty::MEDIUM)
    , ai(nullptr)
    , reactionTime(0.5f)
    , accuracy(0.7f)
    , aggressionLevel(0.5f)
    , botId(-1)
    , botName("Bot")
{
    team = ShipTeam::ENEMY;
    ai = new BotAI();
}

Bot::~Bot() {
    if (ai) {
        delete ai;
        ai = nullptr;
    }
}

void Bot::setDifficulty(Difficulty diff) {
    difficulty = diff;
    
    switch(diff) {
        case Difficulty::EASY:
            reactionTime = 0.8f;      // Slow reaction
            accuracy = 0.5f;          // Low accuracy
            aggressionLevel = 0.3f;   // Passive
            stats.turnSpeed = 1.5f;
            stats.speed = 40;
            break;
            
        case Difficulty::MEDIUM:
            reactionTime = 0.5f;      // Normal reaction
            accuracy = 0.7f;          // Normal accuracy
            aggressionLevel = 0.5f;   // Balanced
            stats.turnSpeed = 2.0f;
            stats.speed = 50;
            break;
            
        case Difficulty::HARD:
            reactionTime = 0.2f;      // Fast reaction
            accuracy = 0.9f;          // High accuracy
            aggressionLevel = 0.8f;   // Aggressive
            stats.turnSpeed = 3.0f;
            stats.speed = 65;
            break;
    }
    
    botName = std::string("Bot ") + getDifficultyUzbek(diff);
}

void Bot::update(float deltaTime, const Math3D::Vector3& targetPos,
                 const Math3D::Vector3& playerPos) {
    if (!isActive) return;
    
    think(deltaTime);
    
    // Update ship physics
    Ship::update(deltaTime);
}

void Bot::updateAI(float deltaTime, const Math3D::Vector3& targetPos,
                 const Math3D::Vector3& playerPos) {
    update(deltaTime, targetPos, playerPos);
}

void Bot::think(float deltaTime) {
    static float thinkTimer = 0;
    thinkTimer += deltaTime;
    
    if (thinkTimer >= reactionTime) {
        thinkTimer = 0;
        makeDecision();
    }
}

void Bot::makeDecision() {
    // Simple AI decision making
    float randVal = static_cast<float>(rand()) / RAND_MAX;
    
    if (randVal < aggressionLevel) {
        // Attack behavior
        attack(Math3D::Vector3(0, 0, -100)); // Default forward
    } else if (randVal < 0.8f) {
        // Patrol behavior
        patrol();
    } else {
        // Evade behavior
        evade(Math3D::Vector3(0, 0, 100));
    }
}

bool Bot::shouldFire() const {
    float randVal = static_cast<float>(rand()) / RAND_MAX;
    return randVal < accuracy;
}

Math3D::Vector3 Bot::calculateAimOffset(const Math3D::Vector3& targetPos) const {
    // Add some inaccuracy based on difficulty
    float errorMargin = (1.0f - accuracy) * 10.0f;
    
    float offsetX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * errorMargin;
    float offsetY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * errorMargin;
    
    return Math3D::Vector3(offsetX, offsetY, 0);
}

void Bot::patrol() {
    // Simple patrol pattern - move in current direction
    Math3D::Vector3 forward = getForwardDirection();
    move(forward, 0.016f); // Assume 60 FPS
    
    // Random turns
    if (rand() % 100 < 30) {
        float turnDir = (rand() % 2 == 0) ? 1.0f : -1.0f;
        rotate(turnDir * 0.5f, 0, 0.016f);
    }
}

void Bot::chase(const Math3D::Vector3& target) {
    Math3D::Vector3 toTarget = target - position;
    Math3D::Vector3 direction = toTarget.normalize();
    
    move(direction, 0.016f);
    
    // Rotate towards target
    // Simplified rotation logic
}

void Bot::evade(const Math3D::Vector3& threat) {
    Math3D::Vector3 awayFromThreat = position - threat;
    Math3D::Vector3 direction = awayFromThreat.normalize();
    
    move(direction * 1.5f, 0.016f); // Move faster when evading
}

void Bot::attack(const Math3D::Vector3& target) {
    chase(target);
    
    if (shouldFire()) {
        fire();
    }
}

void Bot::reset() {
    activate();
    setPosition(Math3D::Vector3(
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 100,
        10,
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 100
    ));
}
