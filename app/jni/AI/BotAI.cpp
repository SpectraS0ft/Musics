#include "BotAI.h"
#include <cstdlib>
#include <cmath>

BotAI::BotAI() 
    : currentState(AIState::IDLE)
    , currentWaypointIndex(0)
    , detectionRange(150.0f)
    , attackRange(80.0f)
    , retreatHealthThreshold(0.3f)
    , timeSinceLastSighting(0)
{
}

BotAI::~BotAI() {
}

bool BotAI::detectPlayer(const Math3D::Vector3& botPos,
                        const Math3D::Vector3& playerPos,
                        float deltaTime) {
    float distance = calculateDistance(botPos, playerPos);
    
    if (distance <= detectionRange) {
        lastKnownPlayerPos = playerPos;
        timeSinceLastSighting = 0;
        return true;
    }
    
    timeSinceLastSighting += deltaTime;
    return false;
}

AIState BotAI::decideState(float health, float ammo, bool playerVisible) {
    // Low health - retreat
    if (health < retreatHealthThreshold) {
        return AIState::RETREAT;
    }
    
    // No ammo - evade or retreat
    if (ammo <= 0) {
        return playerVisible ? AIState::EVADE : AIState::RETREAT;
    }
    
    // Player visible
    if (playerVisible) {
        float distance = calculateDistance(lastKnownPlayerPos, Math3D::Vector3(0,0,0));
        
        if (distance <= attackRange) {
            return AIState::ATTACK;
        } else {
            return AIState::CHASE;
        }
    }
    
    // Default behavior based on current state
    switch(currentState) {
        case AIState::PATROL:
            return AIState::PATROL;
        default:
            return AIState::PATROL;
    }
}

void BotAI::generatePatrolRoute(const Math3D::Vector3& center, float radius, int numPoints) {
    patrolRoute.clear();
    
    for (int i = 0; i < numPoints; i++) {
        float angle = (2.0f * 3.14159f * i) / numPoints;
        Math3D::Vector3 point(
            center.x + std::cos(angle) * radius,
            center.y + 10, // Maintain altitude
            center.z + std::sin(angle) * radius
        );
        patrolRoute.push_back(Waypoint(point));
    }
}

Math3D::Vector3 BotAI::getNextPatrolPoint() {
    if (patrolRoute.empty()) {
        return Math3D::Vector3(0, 10, 0);
    }
    
    Waypoint& current = patrolRoute[currentWaypointIndex];
    current.visited = true;
    
    currentWaypointIndex = (currentWaypointIndex + 1) % patrolRoute.size();
    
    // Reset visited flags when all points visited
    bool allVisited = true;
    for (const auto& wp : patrolRoute) {
        if (!wp.visited) {
            allVisited = false;
            break;
        }
    }
    
    if (allVisited) {
        for (auto& wp : patrolRoute) {
            wp.visited = false;
        }
    }
    
    return patrolRoute[currentWaypointIndex].position;
}

bool BotAI::shouldEngage(const Math3D::Vector3& playerPos, float distance) {
    return distance <= attackRange && distance > 10.0f; // Don't get too close
}

Math3D::Vector3 BotAI::calculateInterceptPoint(const Math3D::Vector3& playerPos,
                                               const Math3D::Vector3& playerVel,
                                               const Math3D::Vector3& botPos,
                                               float projectileSpeed) {
    // Simple lead calculation
    Math3D::Vector3 toPlayer = playerPos - botPos;
    float distance = toPlayer.length();
    float timeToImpact = distance / projectileSpeed;
    
    return playerPos + playerVel * timeToImpact;
}

void BotAI::executeIdle(float deltaTime) {
    // Do nothing or minimal movement
}

void BotAI::executePatrol(float deltaTime) {
    // Move to next patrol point
    // Implementation in Bot class
}

void BotAI::executeChase(float deltaTime, const Math3D::Vector3& target) {
    // Move towards target
    // Implementation in Bot class
}

void BotAI::executeAttack(float deltaTime, const Math3D::Vector3& target) {
    // Fire at target
    // Implementation in Bot class
}

void BotAI::executeEvade(float deltaTime, const Math3D::Vector3& threat) {
    // Move away from threat
    // Implementation in Bot class
}

void BotAI::executeRetreat(float deltaTime) {
    // Find safe position and move there
    // Implementation in Bot class
}

float BotAI::calculateDistance(const Math3D::Vector3& a, const Math3D::Vector3& b) {
    return (b - a).length();
}

bool BotAI::isLineOfSightClear(const Math3D::Vector3& from, const Math3D::Vector3& to) {
    // Simplified - always returns true
    // In real implementation, check for obstacles
    return true;
}

void BotAI::reset() {
    currentState = AIState::IDLE;
    currentWaypointIndex = 0;
    timeSinceLastSighting = 0;
    
    for (auto& wp : patrolRoute) {
        wp.visited = false;
    }
}
