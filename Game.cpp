//-----------------------------------------------------------------
// Main Game File
// C++ Source - Game.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include "Game.h"
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//-----------------------------------------------------------------
// Game Member Functions																				
//-----------------------------------------------------------------

Game::Game(const std::string& scriptName)
	: m_ScriptName(scriptName)
{
	// nothing to create
}


Game::~Game()
{
	// nothing to destroy
}

void Game::Initialize()
{
	// Code that needs to execute (once) at the start of the game, before the game window is created

	AbstractGame::Initialize();
	GAME_ENGINE->SetTitle(_T("Game Engine version 8_01"));

	GAME_ENGINE->SetWidth(1024);
	GAME_ENGINE->SetHeight(768);
	GAME_ENGINE->SetFrameRate(50);
	// Set the keys that the game needs to listen to
	//tstringstream buffer;
	//buffer << _T("KLMO");
	//buffer << (char) VK_LEFT;
	//buffer << (char) VK_RIGHT;
	//GAME_ENGINE->SetKeyList(buffer.str());
#if defined(_DEBUG)
#if defined(_WIN64)
	ShowWarning(_T("Running in x64 Debug mode"));
#else
	ShowWarning(_T("Running in x86 Debug mode"));
#endif
#else
#if defined(_WIN64)
	ShowWarning(_T("Running in x64 Release mode"));
#else
	ShowWarning(_T("Running in x86 Release mode"));
#endif
#endif
	TestLua();
	WhatFile(m_ScriptName);
}

void Game::Start()
{
	// Insert code that needs to execute (once) at the start of the game, after the game window is created
}

void Game::End()
{
	// Insert code that needs to execute when the game ends
}

void Game::Paint(RECT rect) const
{
	// Insert paint code 
}

void Game::Tick()
{
	// Insert non-paint code that needs to execute each tick 
}

void Game::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{
	// Insert code for a mouse button action

	/* Example:
	if (isLeft == true && isDown == true) // is it a left mouse click?
	{
		if ( x > 261 && x < 261 + 117 ) // check if click lies within x coordinates of choice
		{
			if ( y > 182 && y < 182 + 33 ) // check if click also lies within y coordinates of choice
			{
				GAME_ENGINE->MessageBox(_T("Clicked."));
			}
		}
	}
	*/
}

void Game::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{
	// Insert code for a mouse wheel action
}

void Game::MouseMove(int x, int y, WPARAM wParam)
{
	// Insert code that needs to execute when the mouse pointer moves across the game window

	/* Example:
	if ( x > 261 && x < 261 + 117 ) // check if mouse position is within x coordinates of choice
	{
		if ( y > 182 && y < 182 + 33 ) // check if mouse position also is within y coordinates of choice
		{
			GAME_ENGINE->MessageBox("Mouse move.");
		}
	}
	*/
}

void Game::CheckKeyboard()
{
	// Here you can check if a key is pressed down
	// Is executed once per frame 

	/* Example:
	if (GAME_ENGINE->IsKeyDown(_T('K'))) xIcon -= xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('L'))) yIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('M'))) xIcon += xSpeed;
	if (GAME_ENGINE->IsKeyDown(_T('O'))) yIcon -= ySpeed;
	*/
}

void Game::KeyPressed(TCHAR key)
{
	// DO NOT FORGET to use SetKeyList() !!

	// Insert code that needs to execute when a key is pressed
	// The function is executed when the key is *released*
	// You need to specify the list of keys with the SetKeyList() function

	/* Example:
	switch (key)
	{
	case _T('K'): case VK_LEFT:
		GAME_ENGINE->MessageBox("Moving left.");
		break;
	case _T('L'): case VK_DOWN:
		GAME_ENGINE->MessageBox("Moving down.");
		break;
	case _T('M'): case VK_RIGHT:
		GAME_ENGINE->MessageBox("Moving right.");
		break;
	case _T('O'): case VK_UP:
		GAME_ENGINE->MessageBox("Moving up.");
		break;
	case VK_ESCAPE:
		GAME_ENGINE->MessageBox("Escape menu.");
	}
	*/
}

void Game::CallAction(Caller* callerPtr)
{
	// Insert the code that needs to execute when a Caller (= Button, TextBox, Timer, Audio) executes an action
}

/// <summary>
/// PRIVATE FUNCTIONS
/// </summary>
void Game::ShowWarning(const tstring& message)
{
	GAME_ENGINE->MessageBox(message);
}

void Game::TestLua()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L); // Open Lua standard libraries

	// Load the Lua script
	if (luaL_dofile(L, "test.lua") != LUA_OK)
	{
		ShowWarning(_T("Error loading Lua script"));
		lua_close(L);
		return;
	}

	// Retrieve the message from Lua
	lua_getglobal(L, "message");
	if (lua_isstring(L, -1))
	{
		const char* message = lua_tostring(L, -1);
		// Convert message to TCHAR and show it
		auto tmessage = std::basic_string<TCHAR>(message, message + strlen(message));
		ShowWarning(tmessage);  // This assumes you're using _UNICODE
	}
	else
	{
		ShowWarning(_T("Lua script did not define 'message'"));
	}

	// Clean up Lua
	lua_close(L);
}

void Game::TestLua(const std::basic_string<TCHAR>& scriptName)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L); // Open Lua standard libraries
	// Load the Lua script
	if (luaL_dofile(L, std::string(scriptName.begin(), scriptName.end()).c_str()) != LUA_OK)
	{
		ShowWarning(_T("Error loading Lua script"));
		lua_close(L);
		return;
	}
	// Retrieve the message from Lua
	lua_getglobal(L, "message");
	if (lua_isstring(L, -1))
	{
		const char* message = lua_tostring(L, -1);
		// Convert message to TCHAR and show it
		auto tmessage = std::basic_string<TCHAR>(message, message + strlen(message));
		ShowWarning(tmessage);  // This assumes you're using _UNICODE
	}
	else
	{
		ShowWarning(_T("Lua script did not define 'message'"));
	}
	// Clean up Lua
	lua_close(L);
}

void Game::WhatFile(const std::string& scriptName)
{
	std::ifstream file(scriptName);
	auto tScriptName = std::basic_string<TCHAR>(scriptName.begin(), scriptName.end());
	if (file.is_open())
	{
		ShowWarning(_T("File ") + tScriptName + _T(" exists."));
		TestLua(tScriptName);
		file.close();
	}
	else
	{
		ShowWarning(_T("File ") + tScriptName + _T(" does not exist."));
	}
}