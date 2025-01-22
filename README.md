# Game Engine Lua Integration Project

## Overview
This project is an exam assignment for the Game Development / Software Engineering course (2024-2025). The main goal is to integrate Lua bindings into a Win32 Game Engine using SOL2, allowing full control of the Game Engine through Lua scripting. The project is managed using CMake and demonstrates the Lua bindings by creating a simple 2D game.

## Features
- **Lua Bindings**: Exposed as much functionality of the Game Engine as possible to Lua, enabling game logic to be written entirely in Lua.
- **CMake Integration**: The Game Engine project is configured using CMake for streamlined project generation and builds.
- **2D Game in Lua**: Includes a simple 2D game implemented in Lua to demonstrate the engine's capabilities and Lua bindings. The game logic is cleanly separated into Lua scripts.
- **Drag-and-Drop System**: Allows loading of Lua scripts dynamically for additional games, if available.
- **Annotations**: EmmyLua annotations are added for all classes and functions exposed to Lua, improving code readability and auto-completion support in IDEs.

### Lua Scripts
- Lua scripts resemble the structure of the C++ Game Engine, with functions like `Initialize`, `GameStart`, `GameEnd`, `Tick`, and `Paint`.
- Annotations make sure that I could maintain typesafety during my lua coding journey

## Deliverables
- A zipped package with the following structure:
  - `SE_Exam_GD11_Cremers_Liam.rar`
  - **Contents**:
  - `Game`: Release build, Lua scripts, and all dependencies/resources.
  - `Project` Project folder (includes project source files)
    - `src`: Source files and project structure.
    - `Game`: Release build, Lua scripts, and all dependencies/resources.
    - `Lua`: Lua files are kept in a seperate folder
    - `cmake`: Cmake files to enhance the clarity of my CMakeLists.txt
    - `NOTE`: i used the following source_groups/filters ('Visual Studio' specific)
      - `Engine Files`: Here are the files used in Kevin's Engine except for Game.cpp and Game.h
        - `Source Files`: Source Files included with Kevin's Engine
      - `Game Files`: Game.h & Game.cpp
        - `Lua`: All Lua-files found in Lua folder in Project
      - `Resource Files`: Resources used specifically for the game

## How to Run
1. Unzip the provided package.
2. Navigate to the `Game Folder`.
3. Run the executable to launch the default game.
4. (Optional) Use the drag-and-drop system to load other Lua scripts if available.

## Notes
- Focus is placed on clean, maintainable code in both C++ and Lua, with extensibility and performance optimization considered.
- EmmyLua annotations are provided to enhance usability when editing Lua scripts.
