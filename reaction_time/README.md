
# Reaction Time Test

A simple reaction time tester built with C++ and SFML. Test how fast you can click when the screen turns green!

## Overview

This application measures your reaction time by displaying a red "Wait..." screen for a random duration (0-7 seconds), then turning green. Click as quickly as possible when it turns green to measure your reaction time in seconds.

### Features

- **Random wait time**: Prevents anticipation by randomly varying the wait period (0-7 seconds)
- **Best score tracking**: Automatically tracks and displays your fastest reaction time
- **Precision measurement**: Displays reaction time to the thousandth of a second (e.g., 0.234 seconds)
- **Simple interface**: Clean, full-screen color changes make it easy to use

## How to Play

1. **Start**: Click anywhere in the window to begin
2. **Wait**: The screen turns red - wait for it to turn green (don't click early!)
3. **Click**: When the screen turns green, click as fast as you can
4. **Result**: Your reaction time is displayed in seconds
5. **Retry**: Click again to try and beat your best score

**Note**: If you click during the red "Wait..." screen, you'll need to start over!

## Requirements

- Windows 10/11
- SFML 3.0.2
- CMake 3.21+
- Visual Studio Build Tools 2019 or newer (MSVC compiler)
- vcpkg (for SFML dependency management)
- - **C++17**: Core programming language

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
   git clone https://github.com/dsalas560/Cpp.git
   cd Cpp/reaction_time
```

2. Build with CMake:
```powershell
   cmd /c '"C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" && cmake -B build -S . -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake && cmake --build build'
```

3. Run the application:
```powershell
   .\build\reaction_time.exe
```

## Project Structure
```
reaction_time/
├── CMakeLists.txt      # CMake build configuration
├── main.cpp            # Main application source code
├── README.md           # This file
└── build/              # Build output directory (generated)
    └── reaction_time.exe
```

## Acknowledgments

- Built with [SFML](https://www.sfml-dev.org/) - Simple and Fast Multimedia Library
- Inspired by classic reaction time tests

## Author
[dsalas560](https://github.com/dsalas560)

