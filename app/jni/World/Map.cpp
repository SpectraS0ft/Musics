#include "Map.h"

Map::Map() 
    : type(MapType::NEBULA)
    , name("Nebula")
    , ambientLightIntensity(0.3f)
{
}

Map::~Map() {
    unload();
}

void Map::loadMap(MapType mapType) {
    type = mapType;
    obstacles.clear();
    spawnPoints.clear();
    
    switch(mapType) {
        case MapType::NEBULA:
            name = "Nebula";
            nameUzbek = "Tumanlik";
            skyboxTexture = "nebula_skybox";
            boundary = {-500, 500, -100, 200, -500, 500};
            break;
            
        case MapType::ASTEROID:
            name = "Asteroid Field";
            nameUzbek = "Asteroidlar Maydoni";
            skyboxTexture = "asteroid_skybox";
            boundary = {-300, 300, -50, 150, -300, 300};
            // Add asteroid obstacles
            for (int i = 0; i < 20; i++) {
                Obstacle ast;
                ast.position = Math3D::Vector3(
                    (rand() % 600) - 300,
                    (rand() % 200) - 100,
                    (rand() % 600) - 300
                );
                ast.size = Math3D::Vector3(20, 20, 20);
                ast.meshName = "asteroid";
                ast.isDestructible = true;
                ast.health = 100;
                obstacles.push_back(ast);
            }
            break;
            
        case MapType::STATION:
            name = "Space Station";
            nameUzbek = "Kosmik Stansiya";
            skyboxTexture = "station_skybox";
            boundary = {-200, 200, -50, 100, -200, 200};
            break;
            
        case MapType::VOID:
            name = "The Void";
            nameUzbek = "Bo'shliq";
            skyboxTexture = "void_skybox";
            boundary = {-1000, 1000, -200, 300, -1000, 1000};
            break;
            
        case MapType::HOMEWORLD:
            name = "Homeworld";
            nameUzbek = "Vatan Sayyorasi";
            skyboxTexture = "homeworld_skybox";
            boundary = {-400, 400, 0, 200, -400, 400};
            break;
    }
    
    // Setup spawn points
    SpawnPoint playerSpawn;
    playerSpawn.position = Math3D::Vector3(0, 50, 0);
    playerSpawn.rotation = Math3D::Vector3(0, 0, 0);
    playerSpawn.teamId = 0;
    spawnPoints.push_back(playerSpawn);
    
    // Enemy spawns
    for (int i = 0; i < 4; i++) {
        SpawnPoint enemySpawn;
        float angle = (i * 90) * 3.14159f / 180.0f;
        enemySpawn.position = Math3D::Vector3(
            std::cos(angle) * 200,
            50,
            std::sin(angle) * 200
        );
        enemySpawn.rotation = Math3D::Vector3(0, angle, 0);
        enemySpawn.teamId = i + 1;
        spawnPoints.push_back(enemySpawn);
    }
}

void Map::unload() {
    obstacles.clear();
    spawnPoints.clear();
}

bool Map::checkCollision(const Math3D::Vector3& pos, float radius) const {
    // Check bounds
    if (!isWithinBounds(pos)) return true;
    
    // Check obstacles
    for (const auto& obs : obstacles) {
        float dx = std::abs(pos.x - obs.position.x);
        float dy = std::abs(pos.y - obs.position.y);
        float dz = std::abs(pos.z - obs.position.z);
        
        if (dx < (obs.size.x/2 + radius) &&
            dy < (obs.size.y/2 + radius) &&
            dz < (obs.size.z/2 + radius)) {
            return true;
        }
    }
    
    return false;
}

bool Map::isWithinBounds(const Math3D::Vector3& pos) const {
    return pos.x >= boundary.minX && pos.x <= boundary.maxX &&
           pos.y >= boundary.minY && pos.y <= boundary.maxY &&
           pos.z >= boundary.minZ && pos.z <= boundary.maxZ;
}

SpawnPoint Map::getPlayerSpawn() const {
    for (const auto& sp : spawnPoints) {
        if (sp.teamId == 0) return sp;
    }
    return spawnPoints[0];
}

SpawnPoint Map::getEnemySpawn(int index) const {
    int enemyIndex = 1 + index;
    if (enemyIndex < spawnPoints.size()) {
        return spawnPoints[enemyIndex];
    }
    return spawnPoints[1];
}

void Map::render() const {
    renderSkybox();
    renderObstacles();
}

void Map::renderSkybox() const {
    // Skybox rendering via OpenGL
}

void Map::renderObstacles() const {
    // Obstacle rendering via OpenGL
}

const char* Map::getMapNameUzbek(MapType type) {
    switch(type) {
        case MapType::NEBULA:   return "Tumanlik";
        case MapType::ASTEROID: return "Asteroidlar Maydoni";
        case MapType::STATION:  return "Kosmik Stansiya";
        case MapType::VOID:     return "Bo'shliq";
        case MapType::HOMEWORLD: return "Vatan Sayyorasi";
        default: return "Noma'lum";
    }
}
