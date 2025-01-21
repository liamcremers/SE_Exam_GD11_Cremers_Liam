//-----------------------------------------------------------------
// Main Game  File
// C++ Header - Game.h - version v8_01			
//-----------------------------------------------------------------

#pragma once

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <sol/sol.hpp>

#include "Resource.h"
#include "GameEngine.h"
#include "AbstractGame.h"

//-----------------------------------------------------------------
// Game Class																
//-----------------------------------------------------------------
class Game : public AbstractGame, public Callable
{
public:				
	//---------------------------
	// Constructor(s) and Destructor
	//---------------------------
	Game();

	virtual ~Game() override;

	//---------------------------
	// Disabling copy/move constructors and assignment operators   
	//---------------------------
	Game(const Game& other)					= delete;
	Game(Game&& other) noexcept				= delete;
	Game& operator=(const Game& other)		= delete;
	Game& operator=(Game&& other) noexcept	= delete;

	//---------------------------
	// General Member Functions
	//---------------------------
	void Initialize			()															override;
	void Start				()															override;
	void End				()															override;
	void Paint				(RECT rect)	const											override;
	void Tick				()															override;
	void MouseButtonAction	(bool isLeft, bool isDown, int x, int y, WPARAM wParam)		override;
	void MouseWheelAction	(int x, int y, int distance, WPARAM wParam)					override;
	void MouseMove			(int x, int y, WPARAM wParam)								override;
	void CheckKeyboard		()															override;
	void KeyPressed			(TCHAR key)													override;
	void CallAction			(Caller* callerPtr)											override;

private:
	/// <summary>
	/// PRIVATE FUNCTIONS
	/// </summary>
	void LuaParseScript();
	void BindLuaFunctions();
	void BindCppFunctions();
	// -------------------------
	// Datamembers
	// -------------------------
	std::unique_ptr<Audio> m_AudioPtr{};

	sol::state lua; // Lua state
	sol::function lua_initialize;
	sol::function lua_start;
	sol::function lua_end;
	sol::function lua_paint;
	sol::function lua_tick;
	sol::function lua_mouse_button_action;
	sol::function lua_mouse_wheel_action;
	sol::function lua_mouse_move;
	sol::function lua_check_keyboard;
	sol::function lua_key_pressed;
	sol::function lua_call_action;
};
