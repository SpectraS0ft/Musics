#ifndef BOT_H
#define BOT_H

#include "Ship.h"
#include "../AI/BotAI.h"
#include <string>

enum class Difficulty {
    EASY,    // Oson
    MEDIUM,  // O'rta
    HARD     // Qiyin
};

class Bot : public Ship {
private:
    Difficulty difficulty;
    BotAI* ai;
    
    float reactionTime;
    float accuracy;
    float aggressionLevel;
    
    int botId;
    std::string botName;
    
public:
    Bot();
    virtual ~Bot();
    
    // Getters
    Difficulty getDifficulty() const { return difficulty; }
    float getReactionTime() const { return reactionTime; }
    float getAccuracy() const { return accuracy; }
    float getAggressionLevel() const { return aggressionLevel; }
    int getBotId() const { return botId; }
    std::string getBotName() const { return botName; }
    
    // Setters
    void setDifficulty(Difficulty diff);
    void setBotId(int id) { botId = id; }
    void setBotName(const std::string& name) { botName = name; }
    
    // AI Control
    void update(float deltaTime) override; // AI-specific update
    void update(float deltaTime, const Math3D::Vector3& targetPos, 
                const Math3D::Vector3& playerPos); // Overloaded version for game loop
    void updateAI(float deltaTime, const Math3D::Vector3& targetPos, 
                const Math3D::Vector3& playerPos);
    void think(float deltaTime);
    void makeDecision();
    
    // Combat
    bool shouldFire() const;
    Math3D::Vector3 calculateAimOffset(const Math3D::Vector3& targetPos) const;
    
    // Behavior
    void patrol();
    void chase(const Math3D::Vector3& target);
    void evade(const Math3D::Vector3& threat);
    void attack(const Math3D::Vector3& target);
    
    // Reset
    void reset();
};

// Uzbek translations for difficulty
inline const char* getDifficultyUzbek(Difficulty diff) {
    switch(diff) {
        case Difficulty::EASY:   return "Oson";
        case Difficulty::MEDIUM: return "O'rta";
        case Difficulty::HARD:   return "Qiyin";
        default: return "Noma'lum";
    }
}

#endif // BOT_H
