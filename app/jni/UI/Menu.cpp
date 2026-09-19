#include "Menu.h"

Menu::Menu() 
    : currentState(MenuState::MAIN_MENU)
    , selectedMode(GameMode::CAMPAIGN)
    , selectedDifficulty(1) // Medium
    , selectedMap(0)
    , isVisible(true)
{
    initMainMenu();
}

Menu::~Menu() {
}

void Menu::initMainMenu() {
    currentState = MenuState::MAIN_MENU;
    title = "Space Battle Arena";
    titleUzbek = "Kosmik Jang Arenasi";
    buttons.clear();
    
    float screenWidth = 800;
    float centerY = 200;
    float buttonSpacing = 100;
    
    Button playBtn(UzbekButtons::PLAY, screenWidth/2 - 150, centerY);
    playBtn.setStyle(Button::getMenuStyle());
    playBtn.setOnClick([this]() { 
        setState(MenuState::GAME_MODE_SELECT); 
    });
    buttons.push_back(playBtn);
    
    Button upgradesBtn(UzbekButtons::UPGRADES, screenWidth/2 - 150, centerY + buttonSpacing);
    upgradesBtn.setStyle(Button::getMenuStyle());
    upgradesBtn.setOnClick([this]() { 
        setState(MenuState::UPGRADES); 
    });
    buttons.push_back(upgradesBtn);
    
    Button optionsBtn(UzbekButtons::OPTIONS, screenWidth/2 - 150, centerY + buttonSpacing*2);
    optionsBtn.setStyle(Button::getMenuStyle());
    optionsBtn.setOnClick([this]() { 
        // Open options
    });
    buttons.push_back(optionsBtn);
    
    Button quitBtn(UzbekButtons::QUIT, screenWidth/2 - 150, centerY + buttonSpacing*3);
    quitBtn.setStyle(Button::getMenuStyle());
    quitBtn.setOnClick([this]() { 
        if (onQuit) onQuit(); 
    });
    buttons.push_back(quitBtn);
}

void Menu::initGameModeSelect() {
    currentState = MenuState::GAME_MODE_SELECT;
    titleUzbek = "Rejim Tanlash";
    buttons.clear();
    
    float centerX = 400;
    float startY = 150;
    float spacing = 120;
    
    Button campaignBtn("Kampaniya", centerX - 150, startY);
    campaignBtn.setStyle(Button::getMenuStyle());
    campaignBtn.setOnClick([this]() { 
        selectedMode = GameMode::CAMPAIGN;
        setState(MenuState::DIFFICULTY_SELECT);
    });
    buttons.push_back(campaignBtn);
    
    Button survivalBtn("Yashash", centerX - 150, startY + spacing);
    survivalBtn.setStyle(Button::getMenuStyle());
    survivalBtn.setOnClick([this]() { 
        selectedMode = GameMode::SURVIVAL;
        setState(MenuState::DIFFICULTY_SELECT);
    });
    buttons.push_back(survivalBtn);
    
    Button deathmatchBtn("O'lim Match", centerX - 150, startY + spacing*2);
    deathmatchBtn.setStyle(Button::getMenuStyle());
    deathmatchBtn.setOnClick([this]() { 
        selectedMode = GameMode::DEATHMATCH;
        setState(MenuState::DIFFICULTY_SELECT);
    });
    buttons.push_back(deathmatchBtn);
    
    Button backBtn(UzbekButtons::BACK, centerX - 150, startY + spacing*3);
    backBtn.setStyle(Button::getSmallStyle());
    backBtn.setOnClick([this]() { 
        setState(MenuState::MAIN_MENU); 
    });
    buttons.push_back(backBtn);
}

void Menu::initDifficultySelect() {
    currentState = MenuState::DIFFICULTY_SELECT;
    titleUzbek = "Qiyinchilik Tanlash";
    buttons.clear();
    
    float centerX = 400;
    float startY = 150;
    float spacing = 120;
    
    Button easyBtn("Oson", centerX - 150, startY);
    easyBtn.setStyle(Button::getMenuStyle());
    easyBtn.setOnClick([this]() { 
        selectedDifficulty = 0;
        setState(MenuState::MAP_SELECT);
    });
    buttons.push_back(easyBtn);
    
    Button mediumBtn("O'rta", centerX - 150, startY + spacing);
    mediumBtn.setStyle(Button::getMenuStyle());
    mediumBtn.setOnClick([this]() { 
        selectedDifficulty = 1;
        setState(MenuState::MAP_SELECT);
    });
    buttons.push_back(mediumBtn);
    
    Button hardBtn("Qiyin", centerX - 150, startY + spacing*2);
    hardBtn.setStyle(Button::getMenuStyle());
    hardBtn.setOnClick([this]() { 
        selectedDifficulty = 2;
        setState(MenuState::MAP_SELECT);
    });
    buttons.push_back(hardBtn);
    
    Button backBtn(UzbekButtons::BACK, centerX - 150, startY + spacing*3);
    backBtn.setStyle(Button::getSmallStyle());
    backBtn.setOnClick([this]() { 
        setState(MenuState::GAME_MODE_SELECT); 
    });
    buttons.push_back(backBtn);
}

void Menu::initMapSelect() {
    currentState = MenuState::MAP_SELECT;
    titleUzbek = "Xarita Tanlash";
    buttons.clear();
    
    const char* maps[] = {"Nebula", "Asteroid", "Station", "Void", "Homeworld"};
    float centerX = 400;
    float startY = 100;
    float spacing = 90;
    
    for (int i = 0; i < 5; i++) {
        Button mapBtn(maps[i], centerX - 150, startY + i*spacing);
        mapBtn.setStyle(Button::getMenuStyle());
        mapBtn.setOnClick([this, i]() { 
            selectedMap = i;
            if (onStartGame) onStartGame();
        });
        buttons.push_back(mapBtn);
    }
    
    Button backBtn(UzbekButtons::BACK, centerX - 150, startY + 5*spacing);
    backBtn.setStyle(Button::getSmallStyle());
    backBtn.setOnClick([this]() { 
        setState(MenuState::DIFFICULTY_SELECT); 
    });
    buttons.push_back(backBtn);
}

void Menu::initUpgrades() {
    currentState = MenuState::UPGRADES;
    titleUzbek = "Yangilashlar";
    buttons.clear();
    
    Button backBtn(UzbekButtons::BACK, 400 - 75, 500);
    backBtn.setStyle(Button::getSmallStyle());
    backBtn.setOnClick([this]() { 
        setState(MenuState::MAIN_MENU); 
    });
    buttons.push_back(backBtn);
}

void Menu::initPause() {
    currentState = MenuState::PAUSE;
    titleUzbek = "Tanaffus";
    buttons.clear();
    
    Button resumeBtn(UzbekButtons::RESUME, 400 - 150, 200);
    resumeBtn.setStyle(Button::getMenuStyle());
    resumeBtn.setOnClick([this]() { 
        setIsVisible(false);
    });
    buttons.push_back(resumeBtn);
    
    Button restartBtn(UzbekButtons::RESTART, 400 - 150, 300);
    restartBtn.setStyle(Button::getMenuStyle());
    restartBtn.setOnClick([this]() { 
        if (onStartGame) onStartGame();
    });
    buttons.push_back(restartBtn);
    
    Button menuBtn(UzbekButtons::BACK, 400 - 150, 400);
    menuBtn.setStyle(Button::getMenuStyle());
    menuBtn.setOnClick([this]() { 
        setState(MenuState::MAIN_MENU);
        setIsVisible(false);
    });
    buttons.push_back(menuBtn);
}

void Menu::initGameOver() {
    currentState = MenuState::GAME_OVER;
    titleUzbek = "O'yin Tugadi";
    buttons.clear();
    
    Button restartBtn(UzbekButtons::RESTART, 400 - 150, 300);
    restartBtn.setStyle(Button::getMenuStyle());
    restartBtn.setOnClick([this]() { 
        if (onStartGame) onStartGame();
    });
    buttons.push_back(restartBtn);
    
    Button menuBtn(UzbekButtons::BACK, 400 - 150, 400);
    menuBtn.setStyle(Button::getMenuStyle());
    menuBtn.setOnClick([this]() { 
        setState(MenuState::MAIN_MENU);
    });
    buttons.push_back(menuBtn);
}

void Menu::setState(MenuState state) {
    currentState = state;
    
    switch(state) {
        case MenuState::MAIN_MENU: initMainMenu(); break;
        case MenuState::GAME_MODE_SELECT: initGameModeSelect(); break;
        case MenuState::DIFFICULTY_SELECT: initDifficultySelect(); break;
        case MenuState::MAP_SELECT: initMapSelect(); break;
        case MenuState::UPGRADES: initUpgrades(); break;
        case MenuState::PAUSE: initPause(); break;
        case MenuState::GAME_OVER: initGameOver(); break;
        default: break;
    }
}

void Menu::update(float deltaTime) {
    for (auto& btn : buttons) {
        btn.update(deltaTime);
    }
}

void Menu::render() const {
    // Rendering handled by Renderer
}

void Menu::onTouchBegin(float x, float y) {
    for (auto& btn : buttons) {
        btn.onTouchBegin(x, y);
    }
}

void Menu::onTouchEnd(float x, float y) {
    for (auto& btn : buttons) {
        btn.onTouchEnd(x, y);
    }
}
