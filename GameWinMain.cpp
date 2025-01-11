//-----------------------------------------------------------------
// Game Engine WinMain Function
// C++ Source - GameWinMain.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <filesystem>
#include <sol/sol.hpp>
#include <windows.h>
#include <iostream>
#include <string>

#include "SetupContext.h"
#include "ToyPainter.h"

#include "GameWinMain.h"
#include "GameEngine.h"
#include "Game.h"	
namespace fs = std::filesystem;

//-----------------------------------------------------------------
// Create GAME_ENGINE global (singleton) object and pointer
//-----------------------------------------------------------------
GameEngine myGameEngine;
GameEngine* GAME_ENGINE{ &myGameEngine };

void CreateBindings(sol::state& lua)
{
	lua.new_usertype<Color>(
		"Color",
		sol::constructors<Color(float, float, float)>(),
		"random", &Color::Random,
		"r", &Color::r,
		"g", &Color::g,
		"b", &Color::b,
		"WHITE", sol::readonly_property([]() { return Color::White; }),
		"BLACK", sol::readonly_property([]() { return Color::Black; }),
		"RED", sol::readonly_property([]() { return Color::Red; }),
		"BLUE", sol::readonly_property([]() { return Color::Blue; }),
		"GREEN", sol::readonly_property([]() { return Color::Green; })
	);

	lua.new_usertype<SetupContext>(
		"SetupContext",
		"set_window_size", &SetupContext::setWindowSize,
		"set_window_title", &SetupContext::setWindowTitle,
		"get_window_title", &SetupContext::getWindowTitle,
		"get_window_width", &SetupContext::getWindowWidth,
		"get_window_height", &SetupContext::getWindowHeight
	);

	lua.new_usertype<ToyPainter>(
		"Painter",
		"fill_circle", sol::overload(
		sol::resolve<void(int, int, int, const Color&, int) const>(&ToyPainter::FillCircle),
		sol::resolve<void(int, int, int, const Color&) const>(&ToyPainter::FillCircle)),
		"fill_rect", &ToyPainter::FillRect,
		"clear", &ToyPainter::Clear
	);
}


//https://sol2.readthedocs.io/en/latest/exceptions.html#exception-handling
int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description)
{
	// L is the lua state, which you can wrap in a state_view if necessary
	// maybe_exception will contain exception, if it exists
	// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
	std::cout << "An exception occurred in a function, here's what it says ";
	if (maybe_exception)
	{
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	}
	else
	{
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
		std::cout << std::endl;
	}

	// you must push 1 element onto the stack to be
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
	// so we push a single string (in our case, the description of the error)
	return sol::stack::push(L, description);
}
//-----------------------------------------------------------------
// Main Function
//-----------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	sol::state lua;
	lua.set_exception_handler(&my_exception_handler);
	try
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
		GAME_ENGINE->SetGame(new Game(scriptName)); // Assuming Game class can take script name as parameter

		return GAME_ENGINE->Run(hInstance, nCmdShow);
	}
	catch (const std::exception& e)
	{
		SDL_Log("Error: %s", e.what());
	}
	return 0;
}