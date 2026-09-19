#include "Ship.h"
#include <algorithm>

Ship::Ship() 
    : position(0, 0, 0)
    , velocity(0, 0, 0)
    , rotation(1, 0, 0, 0)
    , scale(1, 1, 1)
    , type(ShipType::FIGHTER)
    , team(ShipTeam::NEUTRAL)
    , name("Unnamed")
    , id(-1)
    , isActive(false)
    , lastFireTime(0)
{
}

Ship::~Ship() {
}

void Ship::update(float deltaTime) {
    if (!isActive) return;
    
    // Update position based on velocity
    position = position + velocity * deltaTime;
    
    // Apply simple drag
    velocity = velocity * 0.98f;
}

void Ship::move(const Math3D::Vector3& direction, float deltaTime) {
    Math3D::Vector3 moveDir = rotation.rotate(direction);
    position = position + moveDir * stats.speed * deltaTime;
}

void Ship::rotate(float yaw, float pitch, float deltaTime) {
    Math3D::Vector3 upAxis(0, 1, 0);
    Math3D::Vector3 rightAxis(1, 0, 0);
    
    Math3D::Quaternion yawRot = Math3D::Quaternion::fromAxisAngle(
        yaw * stats.turnSpeed * deltaTime, upAxis);
    Math3D::Quaternion pitchRot = Math3D::Quaternion::fromAxisAngle(
        pitch * stats.turnSpeed * deltaTime, rightAxis);
    
    rotation = yawRot * rotation * pitchRot;
}

bool Ship::fire() {
    float currentTime = 0; // Should be passed in real implementation
    if (currentTime - lastFireTime >= stats.fireRate) {
        lastFireTime = currentTime;
        return true;
    }
    return false;
}

void Ship::takeDamage(float damage) {
    float actualDamage = std::max(1.0f, damage - stats.armor);
    stats.currentHealth = std::max(0.0f, stats.currentHealth - actualDamage);
    
    if (stats.currentHealth <= 0) {
        deactivate();
    }
}

void Ship::heal(float amount) {
    stats.currentHealth = std::min(stats.maxHealth, stats.currentHealth + amount);
}

void Ship::activate() {
    isActive = true;
    stats.currentHealth = stats.maxHealth;
}

void Ship::deactivate() {
    isActive = false;
}

Math3D::Vector3 Ship::getForwardDirection() const {
    return rotation.rotate(Math3D::Vector3(0, 0, -1));
}

Math3D::Vector3 Ship::getRightDirection() const {
    return rotation.rotate(Math3D::Vector3(1, 0, 0));
}

Math3D::Vector3 Ship::getUpDirection() const {
    return rotation.rotate(Math3D::Vector3(0, 1, 0));
}

void Ship::upgradeSpeed(float amount) {
    stats.speed += amount;
}

void Ship::upgradeDamage(float amount) {
    stats.damage += amount;
}

void Ship::upgradeArmor(float amount) {
    stats.armor += amount;
}

void Ship::upgradeFireRate(float amount) {
    stats.fireRate = std::max(0.1f, stats.fireRate - amount);
}

void Ship::addMissiles(int count) {
    stats.currentMissiles = std::min(stats.maxMissiles, stats.currentMissiles + count);
}
