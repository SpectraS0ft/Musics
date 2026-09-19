#include "Game.h"

Game::Game() 
    : state(GameState::MENU)
    , player2(nullptr)
    , isSplitScreen(false)
    , difficulty(1)
    , gameMode(GameMode::CAMPAIGN)
    , gameTime(0)
    , deltaTime(0)
{
}

Game::~Game() {
    shutdown();
}

bool Game::initialize() {
    if (!renderer.initialize()) {
        return false;
    }
    
    menu.onStartGame = [this]() {
        startGame(menu.getSelectedMode(), menu.getSelectedDifficulty(), menu.getSelectedMap());
    };
    
    menu.onQuit = []() {
        // Exit application
    };
    
    hud.setupSinglePlayer();
    
    return true;
}

void Game::shutdown() {
    renderer.shutdown();
    
    for (auto* bot : bots) {
        delete bot;
    }
    bots.clear();
    
    if (player2) {
        delete player2;
        player2 = nullptr;
    }
}

void Game::update(float dt) {
    deltaTime = dt;
    gameTime += dt;
    
    switch(state) {
        case GameState::MENU:
            menu.update(dt);
            break;
            
        case GameState::PLAYING:
            // Update players
            player1.update(dt);
            if (player2) {
                player2->update(dt);
            }
            
            // Update bots
            for (auto* bot : bots) {
                bot->update(dt, player1.getPosition(), player1.getPosition());
            }
            
            // Update HUD
            hud.update(dt);
            hud.setHealth((int)player1.getCurrentHealth(), (int)player1.getMaxHealth());
            hud.setScore(player1.getScore());
            break;
            
        case GameState::PAUSED:
            menu.update(dt);
            break;
            
        case GameState::GAME_OVER:
            menu.update(dt);
            break;
    }
}

void Game::render() {
    renderer.beginFrame();
    
    switch(state) {
        case GameState::MENU:
            menu.render();
            break;
            
        case GameState::PLAYING:
            if (isSplitScreen) {
                // Render split screen
                renderer.setupSplitScreenLeft();
                currentMap.render();
                renderer.renderShip(player1.getPosition(), player1.getRotation(),
                                   player1.getHealthPercentage(), true);
                
                renderer.setupSplitScreenRight();
                currentMap.render();
                if (player2) {
                    renderer.renderShip(player2->getPosition(), player2->getRotation(),
                                       player2->getHealthPercentage(), true);
                }
                
                renderer.renderSplitScreenDivider();
            } else {
                currentMap.render();
                renderer.renderShip(player1.getPosition(), player1.getRotation(),
                                   player1.getHealthPercentage(), true);
                
                for (const auto* bot : bots) {
                    renderer.renderShip(bot->getPosition(), bot->getRotation(),
                                       bot->getHealthPercentage(), false);
                }
            }
            
            hud.render();
            break;
            
        case GameState::PAUSED:
            menu.render();
            break;
            
        case GameState::GAME_OVER:
            menu.render();
            break;
    }
    
    renderer.endFrame();
}

void Game::onTouchBegin(int id, float x, float y) {
    input.addTouch(id, x, y);
    
    if (state == GameState::MENU) {
        menu.onTouchBegin(x, y);
    } else if (state == GameState::PLAYING) {
        hud.onTouchBegin(id, x, y);
    }
}

void Game::onTouchMove(int id, float x, float y) {
    input.moveTouch(id, x, y);
    
    if (state == GameState::PLAYING) {
        hud.onTouchMove(id, x, y);
    }
}

void Game::onTouchEnd(int id, float x, float y) {
    input.removeTouch(id);
    
    if (state == GameState::MENU) {
        menu.onTouchEnd(x, y);
    } else if (state == GameState::PLAYING) {
        hud.onTouchEnd(id, x, y);
    }
}

void Game::setScreenSize(int width, int height) {
    input.setScreenSize(width, height);
    renderer.setScreenSize(width, height);
}

void Game::startGame(GameMode mode, int diff, int mapIndex) {
    gameMode = mode;
    difficulty = diff;
    
    // Load map
    currentMap.loadMap(static_cast<MapType>(mapIndex));
    
    // Setup player
    player1.reset();
    SpawnPoint spawn = currentMap.getPlayerSpawn();
    player1.setPosition(spawn.position);
    
    // Check for split-screen
    isSplitScreen = false; // Could be enabled from menu
    
    if (isSplitScreen) {
        player2 = new Player();
        player2->setPlayerId(2);
        player2->reset();
    }
    
    // Create bots based on difficulty
    int numBots = 3 + difficulty;
    for (int i = 0; i < numBots; i++) {
        Bot* bot = new Bot();
        bot->setBotId(i);
        bot->setDifficulty(static_cast<Difficulty>(difficulty));
        SpawnPoint enemySpawn = currentMap.getEnemySpawn(i);
        bot->setPosition(enemySpawn.position);
        bot->activate();
        bots.push_back(bot);
    }
    
    // Setup HUD
    if (isSplitScreen) {
        hud.setupSplitScreen();
    }
    
    state = GameState::PLAYING;
}

void Game::pauseGame() {
    if (state == GameState::PLAYING) {
        state = GameState::PAUSED;
        menu.setState(MenuState::PAUSE);
        menu.setIsVisible(true);
    }
}

void Game::resumeGame() {
    if (state == GameState::PAUSED) {
        state = GameState::PLAYING;
        menu.setIsVisible(false);
    }
}

void Game::endGame(bool playerWon) {
    state = GameState::GAME_OVER;
    menu.setState(MenuState::GAME_OVER);
    menu.setIsVisible(true);
}

const char* Game::getMessageUzbek(const char* english) {
    if (strcmp(english, "Game Over") == 0) return "O'yin Tugadi";
    if (strcmp(english, "Victory") == 0) return "G'alaba";
    if (strcmp(english, "Defeat") == 0) return "Mag'lubiyat";
    if (strcmp(english, "Pause") == 0) return "Tanaffus";
    if (strcmp(english, "Score") == 0) return "Hisob";
    if (strcmp(english, "Time") == 0) return "Vaqt";
    return english;
}
