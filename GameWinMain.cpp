//-----------------------------------------------------------------
// Game Engine WinMain Function
// C++ Source - GameWinMain.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <iostream>
#include <filesystem>
#include <string>
#include <windows.h>
#include "GameWinMain.h"
#include "GameEngine.h"

#include "Game.h"	

//-----------------------------------------------------------------
// Create GAME_ENGINE global (singleton) object and pointer
//-----------------------------------------------------------------
GameEngine myGameEngine;
GameEngine* GAME_ENGINE{ &myGameEngine };

//-----------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    std::string scriptName = "script_tetris.lua"; // default script

    // Convert command line arguments to a more manageable format
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

    if (argc == 2)
    {
        const std::filesystem::path scriptPath(argv[1]);

        // Extract the filename
        scriptName = scriptPath.filename().string();
        GAME_ENGINE->MessageBox(_T("Using Script: ") + std::basic_string<TCHAR>(scriptName.begin(), scriptName.end()));
    }
    else
    {
        auto tsName = std::basic_string<TCHAR>(scriptName.begin(), scriptName.end());
        GAME_ENGINE->MessageBox(_T("No script provided, using default: ") + tsName);
    }

    // Initialize the game with the script
    GAME_ENGINE->SetGame(new Game(scriptName)); // Assuming Game class can take script name as parameter

    return GAME_ENGINE->Run(hInstance, nCmdShow);
}