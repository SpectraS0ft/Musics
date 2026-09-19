#ifndef MENU_H
#define MENU_H

#include "Button.h"
#include <vector>
#include <string>
#include <functional>

enum class MenuState {
    MAIN_MENU,
    GAME_MODE_SELECT,
    DIFFICULTY_SELECT,
    MAP_SELECT,
    UPGRADES,
    OPTIONS,
    PAUSE,
    GAME_OVER
};

enum class GameMode {
    CAMPAIGN,    // Kampaniya
    SURVIVAL,    // Yashash
    DEATHMATCH   // O'lim Match
};

class Menu {
private:
    MenuState currentState;
    GameMode selectedMode;
    int selectedDifficulty;
    int selectedMap;
    
    std::vector<Button> buttons;
    std::string title;
    std::string titleUzbek;
    
    bool isVisible;
    
public:
    Menu();
    ~Menu();
    
    void initMainMenu();
    void initGameModeSelect();
    void initDifficultySelect();
    void initMapSelect();
    void initUpgrades();
    void initPause();
    void initGameOver();
    
    void setState(MenuState state);
    MenuState getState() const { return currentState; }
    
    void update(float deltaTime);
    void render() const;
    
    void onTouchBegin(float x, float y);
    void onTouchEnd(float x, float y);
    
    void setIsVisible(bool visible) { isVisible = visible; }
    bool getIsVisible() const { return isVisible; }
    
    // Getters
    GameMode getSelectedMode() const { return selectedMode; }
    int getSelectedDifficulty() const { return selectedDifficulty; }
    int getSelectedMap() const { return selectedMap; }
    
    // Callbacks
    std::function<void()> onStartGame;
    std::function<void()> onQuit;
    std::function<void()> onBack;
};

// Uzbek translations
namespace MenuUzbek {
    const char* TITLE = "Kosmik Jang Arenasi";
    const char* MAIN_MENU = "Bosh Menyu";
    const char* SELECT_MODE = "Rejim Tanlash";
    const char* SELECT_DIFFICULTY = "Qiyinchilik Tanlash";
    const char* SELECT_MAP = "Xarita Tanlash";
    const char* GAME_OVER = "O'yin Tugadi";
    const char* VICTORY = "G'alaba!";
    const char* DEFEAT = "Mag'lubiyat";
}

#endif // MENU_H
