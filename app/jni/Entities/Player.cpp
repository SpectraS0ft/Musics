#include "Player.h"

Player::Player() 
    : Ship()
    , score(0)
    , kills(0)
    , deaths(0)
    , playerId(1)
    , isHuman(true)
    , upgradePoints(0)
{
    team = ShipTeam::PLAYER;
    name = "Player";
}

Player::~Player() {
}

void Player::addScore(int points) {
    score += points;
}

void Player::addKill() {
    kills++;
    addScore(100); // 100 points per kill
    addUpgradePoints(1); // 1 upgrade point per kill
}

void Player::addDeath() {
    deaths++;
}

void Player::addUpgradePoints(int points) {
    upgradePoints += points;
}

void Player::spendUpgradePoints(const std::string& upgradeType, float amount) {
    int cost = static_cast<int>(amount * 10);
    
    if (upgradePoints >= cost) {
        upgradePoints -= cost;
        
        if (upgradeType == "speed") {
            upgradeSpeed(amount);
        } else if (upgradeType == "damage") {
            upgradeDamage(amount);
        } else if (upgradeType == "armor") {
            upgradeArmor(amount);
        } else if (upgradeType == "firerate") {
            upgradeFireRate(amount);
        }
    }
}

void Player::reset() {
    score = 0;
    kills = 0;
    deaths = 0;
    upgradePoints = 0;
    activate();
    setPosition(Math3D::Vector3(0, 0, 0));
    setVelocity(Math3D::Vector3(0, 0, 0));
}
