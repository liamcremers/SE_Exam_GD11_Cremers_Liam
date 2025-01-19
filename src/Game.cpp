//-----------------------------------------------------------------
// Main Game File
// C++ Source - Game.cpp - version v8_01
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------
#include <filesystem>
#include <iostream>
#include <string>
namespace fs = std::filesystem;
#include <sol/sol.hpp>
#include <Windows.h>

#include "Game.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static void AllocateConsole()
{
    if (AllocConsole()) // Allocate a new console for the application
    {
        FILE* fp; // Redirect STDOUT to the console
        freopen_s(&fp, "CONOUT$", "w", stdout);
        setvbuf(stdout, nullptr, _IONBF, 0); // Disable buffering for stdout

        freopen_s(&fp, "CONOUT$", "w", stderr); // Redirect STDERR to the console
        setvbuf(stderr, nullptr, _IONBF, 0); // Disable buffering for stderr

        freopen_s(&fp, "CONIN$", "r", stdin); // Redirect STDIN to the console
        setvbuf(stdin, nullptr, _IONBF, 0); // Disable buffering for stdin

        std::ios::sync_with_stdio(true); // Sync C++ streams with the console
    }
}

//https://sol2.readthedocs.io/en/latest/exceptions.html#exception-handling
int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception,
                         sol::string_view description)
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
// Game Member Functions
//-----------------------------------------------------------------

// TODO: FIX
Game::Game()
{
}

// TODO: FIX
Game::~Game()
{
}

void Game::Initialize()
{
    AbstractGame::Initialize();
    AllocateConsole();
    LuaParseScript();
    // WhatVersion();
    // WhatFile(m_ScriptName);
    if (lua_initialize.valid())
        lua_initialize();

    // Set the keys that the game needs to listen to
    //tstringstream buffer;
    //buffer << _T("KLMO");
    //buffer << (char) VK_LEFT;
    //GAME_ENGINE->SetKeyList(buffer.str());
    //buffer << (char) VK_RIGHT;
}

void Game::Start() { if (lua_start.valid()) lua_start(); }

void Game::End() { if (lua_end.valid())lua_end(); }

void Game::Paint(RECT rect) const
{
    if (lua_paint.valid())lua_paint();
}

void Game::Tick() { if (lua_tick.valid())lua_tick(); }


void Game::MouseButtonAction(bool isLeft, bool isDown, int x, int y, WPARAM wParam)
{
    /* if (lua_mouse_button_action.valid()) lua_mouse_button_action(isLeft, isDown, x, y);*/
}

void Game::MouseWheelAction(int x, int y, int distance, WPARAM wParam)
{
    /* if (lua_mouse_wheel_action.valid())lua_mouse_wheel_action(x, y, distance);*/
}

void Game::MouseMove(int x, int y, WPARAM wParam)
{
    /* if (lua_mouse_move.valid())lua_mouse_move(x, y);*/
}

void Game::CheckKeyboard()
{
    /* if (lua_check_keyboard.valid())lua_check_keyboard();*/
}

void Game::KeyPressed(TCHAR key)
{
    if (lua_key_pressed.valid())lua_key_pressed(static_cast<char>(key));
}

void Game::CallAction(Caller* callerPtr)
{
    /* if (lua_call_action.valid())lua_call_action(callerPtr->GetType());*/
}

/// <summary>
/// PRIVATE FUNCTIONS
/// </summary>
void Game::LuaParseScript()
{
    try
    {
        luaL_openlibs(lua.lua_state());
        lua.open_libraries(); // open everything
        lua.set_exception_handler(&my_exception_handler);

        std::string scriptName = "script_breakout.lua";

        int argc;
        LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        if (argc == 2)
        {
            const fs::path scriptPath(argv[1]);
            if (!fs::exists(scriptPath))
            {
                std::cerr << ("File does not exist: ") << std::endl;
                return;
            }
            scriptName = scriptPath.filename().string();
            std::cerr << ("Using Script: ") << std::endl;
        }

        BindCppFunctions();

        SetScript(scriptName);
        lua.script_file(scriptName);

        BindLuaFunctions();
        std::cout << ("Lua script loaded successfully\n") << std::endl;
    }
    catch ([[maybe_unused]] const sol::error& e)
    {
        std::cerr << ("Lua Error: ") << std::endl;
    }
    catch ([[maybe_unused]] const std::exception& e)
    {
        std::cerr << ("Exception: ") << std::endl;
    }
}

void Game::BindLuaFunctions()
{
    // Bind Lua functions to lifecycle events
    lua_initialize = lua["initialize"];
    lua_start = lua["start"];
    lua_end = lua["endGame"];
    lua_paint = lua["paint"];
    lua_tick = lua["tick"];
    lua_mouse_button_action = lua["mouse_button_action"];
    lua_mouse_wheel_action = lua["mouse_wheel_action"];
    lua_mouse_move = lua["mouse_move"];
    lua_check_keyboard = lua["check_keyboard"];
    lua_key_pressed = lua["key_pressed"];
    lua_call_action = lua["call_action"];

    if (lua_initialize.valid())
        std::cerr << ("Lua function initialize is valid\n") << std::endl;
    else
        std::cerr << ("Lua function initialize is not valid\n") << std::endl;
}


void Game::BindCppFunctions()
{
    lua["RGB"] = [](int r, int g, int b) { return RGB(r, g, b); };

    lua.new_usertype<SIZE>("SIZE",
                           sol::constructors<SIZE(LONG, LONG), SIZE()>(),
                           "cx", &SIZE::cx,
                           "cy", &SIZE::cy);
    lua.new_usertype<POINT>("POINT",
                            sol::constructors<POINT(double, double), POINT()>(),
                            "x", &POINT::x,
                            "y", &POINT::y);
    lua.new_usertype<RECT>("RECT",
                           sol::constructors<RECT(LONG, LONG, LONG, LONG)>(),
                           "left", &RECT::left,
                           "top", &RECT::top,
                           "right", &RECT::right,
                           "bottom", &RECT::bottom);
    lua.new_usertype<Bitmap>("Bitmap", sol::constructors<Bitmap(const tstring&)>(),
                             "SetTransparencyColor", &Bitmap::SetTransparencyColor,
                             "SetOpacity", &Bitmap::SetOpacity,
                             "Exists", &Bitmap::Exists,
                             "GetWidth", &Bitmap::GetWidth,
                             "GetHeight", &Bitmap::GetHeight,
                             "GetTransparencyColor", &Bitmap::GetTransparencyColor,
                             "GetOpacity", &Bitmap::GetOpacity,
                             "HasAlphaChannel", &Bitmap::HasAlphaChannel,
                             "SaveToFile", &Bitmap::SaveToFile,
                             "GetHandle", &Bitmap::GetHandle);
    lua.new_usertype<Font>("Font", sol::constructors<Font(const tstring&, bool, bool, bool, int)>());
    lua.new_usertype<GameEngine>(
        "GAME_ENGINE", sol::no_constructor,
        "SetTitle", &GameEngine::SetTitle,
        "SetWindowPosition", &GameEngine::SetWindowPosition,
        "SetWindowRegion", &GameEngine::SetWindowRegion,
        "SetKeyList", &GameEngine::SetKeyList,
        "SetFrameRate", &GameEngine::SetFrameRate,
        "SetWidth", &GameEngine::SetWidth,
        "SetHeight", &GameEngine::SetHeight,
        "GoFullscreen", &GameEngine::GoFullscreen,
        "GoWindowedMode", &GameEngine::GoWindowedMode,
        "ShowMousePointer", &GameEngine::ShowMousePointer,
        "Quit", &GameEngine::Quit,
        "HasWindowRegion", &GameEngine::HasWindowRegion,
        "IsFullscreen", &GameEngine::IsFullscreen,
        "IsKeyDown", &GameEngine::IsKeyDown,
        "MessageBox", sol::overload(
            static_cast<void(GameEngine::*)(const tstring&) const>(&GameEngine::MessageBox),
            sol::resolve<void(int) const>(&GameEngine::MessageBox<int>),
            sol::resolve<void(double) const>(&GameEngine::MessageBox<double>),
            sol::resolve<void(bool) const>(&GameEngine::MessageBox<bool>)
        ),
        "MessageContinue", &GameEngine::MessageContinue,
        "CalculateTextDimensions", sol::overload(
            sol::resolve<SIZE(const tstring&, const Font*) const>(
                &GameEngine::CalculateTextDimensions),
            sol::resolve<SIZE(const tstring&, const Font*, RECT) const>(
                &GameEngine::CalculateTextDimensions)
        ),
        "SetColor", &GameEngine::SetColor,
        "SetFont", &GameEngine::SetFont,
        "FillWindowRect", &GameEngine::FillWindowRect,
        "DrawLine", &GameEngine::DrawLine,
        "DrawRect", &GameEngine::DrawRect,
        "FillRect", sol::overload(
            sol::resolve<bool(int, int, int, int) const>(&GameEngine::FillRect),
            sol::resolve<bool(int, int, int, int, int) const>(&GameEngine::FillRect)
        ),
        "DrawOval", &GameEngine::DrawOval,
        "FillOval", sol::overload(
            sol::resolve<bool(int, int, int, int) const>(&GameEngine::FillOval),
            sol::resolve<bool(int, int, int, int, int) const>(&GameEngine::FillOval)
        ),
        "DrawArc", &GameEngine::DrawArc,
        "FillArc", &GameEngine::FillArc,
        "DrawString", sol::overload(
            sol::resolve<int(const tstring&, int, int) const>(&GameEngine::DrawString),
            sol::resolve<int(const tstring&, int, int, int, int) const>(
                &GameEngine::DrawString)
        ),
        "DrawBitmap", sol::overload(
            sol::resolve<bool(const Bitmap*, int, int) const>(&GameEngine::DrawBitmap),
            sol::resolve<bool(const Bitmap*, int, int, RECT) const>(&GameEngine::DrawBitmap)
        ),
        "DrawPolygon", sol::overload(
            sol::resolve<bool(const POINT [], int) const>(&GameEngine::DrawPolygon),
            sol::resolve<bool(const POINT [], int, bool) const>(&GameEngine::DrawPolygon)
        ),
        "FillPolygon", sol::overload(
            sol::resolve<bool(const POINT [], int) const>(&GameEngine::FillPolygon),
            sol::resolve<bool(const POINT [], int, bool) const>(&GameEngine::FillPolygon)
        ),
        "GetDrawColor", &GameEngine::GetDrawColor,
        "Repaint", &GameEngine::Repaint,
        "GetTitle", &GameEngine::GetTitle,
        "GetInstance", &GameEngine::GetInstance,
        "GetWindow", &GameEngine::GetWindow,
        "GetWidth", &GameEngine::GetWidth,
        "GetHeight", &GameEngine::GetHeight,
        "GetFrameRate", &GameEngine::GetFrameRate,
        "GetFrameDelay", &GameEngine::GetFrameDelay,
        "GetWindowPosition", &GameEngine::GetWindowPosition
    );
    lua["GAME_ENGINE"] = GAME_ENGINE;

    lua.new_usertype<Audio>(
        "Audio", sol::constructors<Audio(const tstring&)>(),
        "Tick", &Audio::Tick,
        "Play", sol::overload(
            [](Audio& audio) { audio.Play(); },
            [](Audio& audio, int msecStart) { audio.Play(msecStart); },
            [](Audio& audio, int msecStart, int msecStop) { audio.Play(msecStart, msecStop); }
        ),
        "Pause", &Audio::Pause,
        "Stop", &Audio::Stop,
        "SetVolume", &Audio::SetVolume,
        "SetRepeat", &Audio::SetRepeat,
        "GetName", &Audio::GetName,
        "GetAlias", &Audio::GetAlias,
        "GetDuration", &Audio::GetDuration,
        "IsPlaying", &Audio::IsPlaying,
        "IsPaused", &Audio::IsPaused,
        "GetRepeat", &Audio::GetRepeat,
        "Exists", &Audio::Exists,
        "GetVolume", &Audio::GetVolume,
        "GetType", &Audio::GetType
    );

    lua.new_usertype<HitRegion>(
        "HitRegion", sol::constructors<
            HitRegion(HitRegion::Shape, int, int, int, int),
            HitRegion(const POINT*, int),
            HitRegion(const Bitmap*, COLORREF, COLORREF)
        >(),
        "Move", &HitRegion::Move,
        "HitTest", sol::overload(
            sol::resolve<bool(int, int) const>(&HitRegion::HitTest),
            sol::resolve<bool(const HitRegion*) const>(&HitRegion::HitTest)
        ),
        "CollisionTest", &HitRegion::CollisionTest,
        "GetBounds", &HitRegion::GetBounds,
        "Exists", &HitRegion::Exists,
        "GetHandle", &HitRegion::GetHandle
    );
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
        ShowWarning(tmessage); // This assumes you're using _UNICODE
    }
    else
    {
        ShowWarning(_T("Lua script did not define 'message'"));
    }
    // Clean up Lua
    lua_close(L);
}
