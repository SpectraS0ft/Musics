#ifndef MAP_H
#define MAP_H

#include "../Utils/Math3D.h"
#include <string>
#include <vector>

enum class MapType {
    NEBULA,
    ASTEROID,
    STATION,
    VOID,
    HOMEWORLD
};

struct MapBoundary {
    float minX, maxX;
    float minY, maxY;
    float minZ, maxZ;
};

struct Obstacle {
    Math3D::Vector3 position;
    Math3D::Vector3 size;
    std::string meshName;
    bool isDestructible;
    float health;
};

struct SpawnPoint {
    Math3D::Vector3 position;
    Math3D::Vector3 rotation;
    int teamId; // 0 = player, 1+ = enemies
};

class Map {
private:
    MapType type;
    std::string name;
    std::string nameUzbek;
    
    MapBoundary boundary;
    std::vector<Obstacle> obstacles;
    std::vector<SpawnPoint> spawnPoints;
    
    std::string skyboxTexture;
    std::string environmentMap;
    
    float ambientLightIntensity;
    Math3D::Vector3 sunDirection;
    
public:
    Map();
    ~Map();
    
    void loadMap(MapType mapType);
    void unload();
    
    // Getters
    MapType getType() const { return type; }
    std::string getName() const { return name; }
    std::string getNameUzbek() const { return nameUzbek; }
    MapBoundary getBoundary() const { return boundary; }
    std::vector<Obstacle> getObstacles() const { return obstacles; }
    std::vector<SpawnPoint> getSpawnPoints() const { return spawnPoints; }
    
    // Collision
    bool checkCollision(const Math3D::Vector3& pos, float radius) const;
    bool isWithinBounds(const Math3D::Vector3& pos) const;
    
    // Spawn
    SpawnPoint getPlayerSpawn() const;
    SpawnPoint getEnemySpawn(int index) const;
    
    // Rendering
    void render() const;
    void renderSkybox() const;
    void renderObstacles() const;
    
    // Static map names in Uzbek
    static const char* getMapNameUzbek(MapType type);
};

#endif // MAP_H
