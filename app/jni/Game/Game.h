#ifndef GAME_H
#define GAME_H

#include "Renderer.h"
#include "Input.h"
#include "../UI/Menu.h"
#include "../UI/HUD.h"
#include "../Entities/Player.h"
#include "../Entities/Bot.h"
#include "../World/Map.h"
#include <vector>

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    GameState state;
    
    Renderer renderer;
    Input input;
    Menu menu;
    HUD hud;
    Map currentMap;
    
    Player player1;
    Player* player2; // For split-screen
    std::vector<Bot*> bots;
    
    bool isSplitScreen;
    int difficulty;
    GameMode gameMode;
    
    float gameTime;
    float deltaTime;
    
public:
    Game();
    ~Game();
    
    bool initialize();
    void shutdown();
    
    void update(float dt);
    void render();
    
    void onTouchBegin(int id, float x, float y);
    void onTouchMove(int id, float x, float y);
    void onTouchEnd(int id, float x, float y);
    void onTouchCancel();
    
    void setScreenSize(int width, int height);
    
    void startGame(GameMode mode, int diff, int mapIndex);
    void pauseGame();
    void resumeGame();
    void endGame(bool playerWon);
    
    GameState getState() const { return state; }
    bool getIsSplitScreen() const { return isSplitScreen; }
    
    // Uzbek translations for game messages
    static const char* getMessageUzbek(const char* english);
};

#endif // GAME_H
