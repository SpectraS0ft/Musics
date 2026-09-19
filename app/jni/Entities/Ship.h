#ifndef SHIP_H
#define SHIP_H

#include "../Utils/Math3D.h"
#include <string>
#include <vector>

enum class ShipType {
    FIGHTER,
    CRUISER,
    BOMBER,
    INTERCEPTOR
};

enum class ShipTeam {
    PLAYER,
    ENEMY,
    NEUTRAL
};

struct ShipStats {
    float maxHealth;
    float currentHealth;
    float speed;
    float turnSpeed;
    float fireRate;
    float damage;
    float armor;
    int maxMissiles;
    int currentMissiles;
    
    ShipStats() : maxHealth(100), currentHealth(100), speed(50), 
                  turnSpeed(2), fireRate(0.5f), damage(10), 
                  armor(5), maxMissiles(10), currentMissiles(10) {}
};

class Ship {
protected:
    Math3D::Vector3 position;
    Math3D::Vector3 velocity;
    Math3D::Quaternion rotation;
    Math3D::Vector3 scale;
    
    ShipStats stats;
    ShipType type;
    ShipTeam team;
    
    std::string name;
    int id;
    
    bool isActive;
    float lastFireTime;
    
public:
    Ship();
    virtual ~Ship();
    
    // Getters
    Math3D::Vector3 getPosition() const { return position; }
    Math3D::Vector3 getVelocity() const { return velocity; }
    Math3D::Quaternion getRotation() const { return rotation; }
    ShipStats getStats() const { return stats; }
    ShipType getType() const { return type; }
    ShipTeam getTeam() const { return team; }
    std::string getName() const { return name; }
    int getId() const { return id; }
    bool getIsActive() const { return isActive; }
    float getCurrentHealth() const { return stats.currentHealth; }
    float getMaxHealth() const { return stats.maxHealth; }
    
    // Setters
    void setPosition(const Math3D::Vector3& pos) { position = pos; }
    void setVelocity(const Math3D::Vector3& vel) { velocity = vel; }
    void setRotation(const Math3D::Quaternion& rot) { rotation = rot; }
    void setName(const std::string& n) { name = n; }
    void setId(int i) { id = i; }
    void setTeam(ShipTeam t) { team = t; }
    
    // Actions
    virtual void update(float deltaTime);
    virtual void move(const Math3D::Vector3& direction, float deltaTime);
    virtual void rotate(float yaw, float pitch, float deltaTime);
    virtual bool fire();
    virtual void takeDamage(float damage);
    virtual void heal(float amount);
    virtual void activate();
    virtual void deactivate();
    
    // Utility
    Math3D::Vector3 getForwardDirection() const;
    Math3D::Vector3 getRightDirection() const;
    Math3D::Vector3 getUpDirection() const;
    
    bool isAlive() const { return stats.currentHealth > 0; }
    float getHealthPercentage() const { 
        return stats.currentHealth / stats.maxHealth; 
    }
    
    // Upgrade system
    void upgradeSpeed(float amount);
    void upgradeDamage(float amount);
    void upgradeArmor(float amount);
    void upgradeFireRate(float amount);
    void addMissiles(int count);
};

#endif // SHIP_H
