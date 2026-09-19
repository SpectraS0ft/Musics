#ifndef PLAYER_H
#define PLAYER_H

#include "Ship.h"
#include <string>

class Player : public Ship {
private:
    int score;
    int kills;
    int deaths;
    int playerId; // 1 or 2 for split-screen
    bool isHuman;
    
    // Upgrade points
    int upgradePoints;
    
public:
    Player();
    virtual ~Player();
    
    // Getters
    int getScore() const { return score; }
    int getKills() const { return kills; }
    int getDeaths() const { return deaths; }
    int getPlayerId() const { return playerId; }
    bool getIsHuman() const { return isHuman; }
    int getUpgradePoints() const { return upgradePoints; }
    
    // Setters
    void setPlayerId(int id) { playerId = id; }
    void setIsHuman(bool human) { isHuman = human; }
    
    // Actions
    void addScore(int points);
    void addKill();
    void addDeath();
    void addUpgradePoints(int points);
    
    // Upgrades
    void spendUpgradePoints(const std::string& upgradeType, float amount);
    
    // Reset for new game
    void reset();
};

#endif // PLAYER_H
