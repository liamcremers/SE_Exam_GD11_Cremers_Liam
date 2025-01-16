# Game Engine Lua Integration Project

## Overview
This project is an exam assignment for the Game Development / Software Engineering course (2024-2025). The main goal is to integrate Lua bindings into a Win32 Game Engine using SOL2, allowing full control of the Game Engine through Lua scripting. The project is managed using CMake and demonstrates the Lua bindings by creating a simple 2D game.

## Features
- **Lua Bindings**: Exposed as much functionality of the Game Engine as possible to Lua, enabling game logic to be written entirely in Lua.
- **CMake Integration**: The Game Engine project is configured using CMake for streamlined project generation and builds.
- **2D Game in Lua**: Includes a simple 2D game implemented in Lua to demonstrate the engine's capabilities and Lua bindings. The game logic is cleanly separated into Lua scripts.
- **Drag-and-Drop System**: Allows loading of Lua scripts dynamically for additional games, if available.
- **Annotations**: EmmyLua annotations are added for all classes and functions exposed to Lua, improving code readability and auto-completion support in IDEs.

## Structure
### Root Folder
- `Project Folder`: Contains all the project source files, organized logically. Does not include unnecessary or redundant files.
- `Game Folder`: Includes the release build of the project and all necessary scripts and resources to run the game out-of-the-box.

### Lua Scripts
- Lua scripts resemble the structure of the C++ Game Engine, with functions like `Initialize`, `GameStart`, `GameEnd`, `Tick`, and `Paint`.
- Game logic is modular and spans multiple Lua files for better maintainability.

## Deliverables
- A zipped package with the following structure:
  - `SE_Exam_class_last-name_first-name.rar` or `.zip`
  - **Contents**:
    - `Project Folder`: Source files and project structure.
    - `Game Folder`: Release build, Lua scripts, and all dependencies/resources.

## How to Run
1. Unzip the provided package.
2. Navigate to the `Game Folder`.
3. Run the executable to launch the default game.
4. (Optional) Use the drag-and-drop system to load other Lua scripts if available.

## Notes
- Ensure that the package is copied to a fresh folder to verify its functionality. Dependencies and resources are included to ensure it runs out-of-the-box.
- Focus is placed on clean, maintainable code in both C++ and Lua, with extensibility and performance optimization considered.
- EmmyLua annotations are provided to enhance usability when editing Lua scripts.

## Grading Focus
1. **CMake Setup**: Proper configuration and logical project structure.
2. **Lua Bindings**: Quality, scope, and ease of extensibility.
3. **Code Quality**: Maintainability and readability of both C++ and Lua code.
4. **Game Complexity**: Creativity and logic in the Lua game.
5. **Polish**: Testing to ensure the project runs smoothly and dependencies are handled correctly.

## Recommendations
- Start with bindings required for the Lua game and expand gradually.
- Consider simple games like Snake, Breakout, or Tetris, or implement a creative game idea for higher grading potential.
- Test thoroughly to ensure smooth out-of-the-box execution.

