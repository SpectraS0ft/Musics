#ifndef BOTAI_H
#define BOTAI_H

#include "../Utils/Math3D.h"
#include <vector>

enum class AIState {
    IDLE,
    PATROL,
    CHASE,
    ATTACK,
    EVADE,
    RETREAT
};

struct Waypoint {
    Math3D::Vector3 position;
    bool visited;
    
    Waypoint() : visited(false) {}
    Waypoint(const Math3D::Vector3& pos) : position(pos), visited(false) {}
};

class BotAI {
private:
    AIState currentState;
    std::vector<Waypoint> patrolRoute;
    int currentWaypointIndex;
    
    float detectionRange;
    float attackRange;
    float retreatHealthThreshold;
    
    Math3D::Vector3 lastKnownPlayerPos;
    float timeSinceLastSighting;
    
public:
    BotAI();
    ~BotAI();
    
    // State management
    AIState getState() const { return currentState; }
    void setState(AIState state) { currentState = state; }
    
    // Detection
    bool detectPlayer(const Math3D::Vector3& botPos, 
                     const Math3D::Vector3& playerPos,
                     float deltaTime);
    
    // Decision making
    AIState decideState(float health, float ammo, bool playerVisible);
    
    // Pathfinding (simplified)
    void generatePatrolRoute(const Math3D::Vector3& center, float radius, int numPoints);
    Math3D::Vector3 getNextPatrolPoint();
    
    // Combat AI
    bool shouldEngage(const Math3D::Vector3& playerPos, float distance);
    Math3D::Vector3 calculateInterceptPoint(const Math3D::Vector3& playerPos,
                                           const Math3D::Vector3& playerVel,
                                           const Math3D::Vector3& botPos,
                                           float projectileSpeed);
    
    // Behavior execution
    void executeIdle(float deltaTime);
    void executePatrol(float deltaTime);
    void executeChase(float deltaTime, const Math3D::Vector3& target);
    void executeAttack(float deltaTime, const Math3D::Vector3& target);
    void executeEvade(float deltaTime, const Math3D::Vector3& threat);
    void executeRetreat(float deltaTime);
    
    // Utility
    float calculateDistance(const Math3D::Vector3& a, const Math3D::Vector3& b);
    bool isLineOfSightClear(const Math3D::Vector3& from, const Math3D::Vector3& to);
    
    // Reset
    void reset();
};

#endif // BOTAI_H
