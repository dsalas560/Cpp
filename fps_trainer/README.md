# FPS Trainer

A click accuracy trainer built with C++ and SFML. Improve your aim and reaction speed by clicking on randomly spawning dots!

## Overview

This application helps train your mouse accuracy and speed for FPS (First-Person Shooter) games. Red dots spawn randomly on screen, and you must click them as quickly as possible. The game maintains between 3-6 dots on screen at all times, spawning 3 new dots whenever you click down to 3 remaining.

### Features

- **Dynamic spawning**: 3 new dots appear when you reduce the count to 3 (maintaining 3-6 dots on screen)
- **Random positioning**: Dots spawn at unpredictable locations to train muscle memory
- **Score tracking**: Keep track of how many dots you've successfully clicked
- **Real-time feedback**: Dot counter shows current number of targets on screen
- **Clean interface**: Minimalist design keeps focus on the targets

## How to Play

1. **Start**: The game begins with 6 red dots on screen
2. **Click**: Click on the red dots to destroy them and increase your score
3. **Respawn**: When you click down to 3 dots remaining, 3 new dots instantly spawn
4. **Continue**: Keep clicking to improve your accuracy and speed!

**Controls**:
- **Left Click**: Destroy dots
- **ESC**: Exit the game

## Requirements

- Windows 10/11
- SFML 3.0
- CMake 3.21+
- Visual Studio Build Tools 2019 or newer (MSVC compiler)
- vcpkg (for SFML dependency management)

## Building from Source

### Prerequisites

1. Install [CMake](https://cmake.org/download/)
2. Install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) with "Desktop development with C++"
3. Install [vcpkg](https://github.com/microsoft/vcpkg):
```powershell
   git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
   cd C:\vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg integrate install
```

4. Install SFML via vcpkg:
```powershell
   .\vcpkg install sfml:x64-windows
```

### Build Instructions

1. Clone this repository:
```powershell
   git clone https://github.com/dsalas560/fps_trainer.git
   cd fps_trainer
```

2. Configure and build with CMake:
```powershell
   cmd /c '"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" && cmake -B build -S . -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake && cmake --build build'
```

3. Run the application:
```powershell
   .\build\fps_trainer.exe
```

## Project Structure
```
fps_trainer/
├── CMakeLists.txt      # CMake build configuration
├── main.cpp            # Main application source code
├── README.md           # This file
└── build/              # Build output directory (generated)
    └── fps_trainer.exe
```

## Technologies Used

- **C++17**: Core programming language
- **SFML 3.0**: Simple and Fast Multimedia Library for graphics and window management
- **CMake**: Build system generator
- **vcpkg**: C++ library manager

## Game Mechanics

- **Minimum Dots**: 3 (when you reach this count, 3 more spawn)
- **Maximum Dots**: 6 (the game starts with this many)
- **Dot Size**: 20px radius circular targets
- **Window Size**: 1280x720 pixels


## Known Issues

- Font loading requires a valid Windows font path (currently uses Arial)
- Score resets when the application closes

## Future Improvements

- [ ] Difficulty levels (smaller dots, faster spawns, more dots)
- [ ] Timer mode (60 second challenge)
- [ ] Accuracy tracking (clicks vs hits ratio)
- [ ] Different dot colors or effects
- [ ] Sound effects for hits/misses
- [ ] Leaderboard and statistics
- [ ] Configurable dot size and spawn rate
- [ ] Moving targets


## Author

[dsalas560](https://github.com/dsalas560)

