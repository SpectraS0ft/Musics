# 3D Android Game - "Space Battle Arena"

A full-featured 3D space combat game for Android with split-screen multiplayer, AI bots, multiple difficulty levels, and Uzbek language support.

## Features

- **3D Graphics**: OpenGL ES 3.0 with textured models and shaders
- **Mobile Controls**: Virtual joystick and touch HUD
- **AI Bots**: 3 difficulty levels (Oson, O'rta, Qiyin)
- **Split Screen**: Local multiplayer on one device
- **Multiple Maps**: 5 different battle arenas
- **Game Modes**: Campaign, Survival, Deathmatch
- **Upgrades System**: Ship upgrades and power-ups
- **Uzbek Language**: Full UI localization
- **Main Menu**: Complete menu system with buttons

## Project Structure

```
app/
├── jni/
│   ├── Android.mk
│   ├── Application.mk
│   ├── main.cpp
│   ├── Game/
│   │   ├── Game.h/.cpp
│   │   ├── Renderer.h/.cpp
│   │   ├── Input.h/.cpp
│   │   └── Audio.h/.cpp
│   ├── Entities/
│   │   ├── Player.h/.cpp
│   │   ├── Bot.h/.cpp
│   │   └── Ship.h/.cpp
│   ├── AI/
│   │   └── BotAI.h/.cpp
│   ├── UI/
│   │   ├── Menu.h/.cpp
│   │   ├── HUD.h/.cpp
│   │   └── Button.h/.cpp
│   ├── World/
│   │   ├── Map.h/.cpp
│   │   └── Texture.h/.cpp
│   └── Utils/
│       ├── Logger.h/.cpp
│       └── Math3D.h/.cpp
├── res/
│   ├── layout/
│   ├── values/
│   │   └── strings.xml (Uzbek)
│   └── drawable/
└── AndroidManifest.xml
```

## Build Instructions

1. Install Android NDK
2. Run `ndk-build` in the jni directory
3. Build APK using Gradle or Android Studio

## Controls

- **Left Side**: Virtual joystick for movement
- **Right Side**: Touch buttons for actions (fire, boost, special)
- **Split Screen**: Player 1 (left), Player 2 (right)

## Difficulty Levels

1. **Oson** (Easy) - Slow reaction, low accuracy
2. **O'rta** (Medium) - Balanced gameplay
3. **Qiyin** (Hard) - Fast reaction, high accuracy

## Game Modes

1. **Kampaniya** (Campaign) - Story mode
2. **Yashash** (Survival) - Endless waves
3. **O'lim Match** (Deathmatch) - Free for all
