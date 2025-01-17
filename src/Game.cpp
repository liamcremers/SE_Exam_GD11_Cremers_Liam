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
Game::Game() { if (lua_start.valid())lua_start(); }

// TODO: FIX
Game::~Game() { if (lua_start.valid())lua_start(); }

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

        SetScript(scriptName);
        lua.script_file(scriptName);

        BindLuaFunctions();
        BindCppFunctions();
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
    // lua set macro RGB
    // lua["_T"] = [](const std::string& str) { return tstring(str.begin(), str.end()); };
    lua["RGB"] = [](int r, int g, int b) { return RGB(r, g, b); };

    lua.new_usertype<POINT>("POINT", sol::constructors<POINT(int, int)>());
    lua.new_usertype<RECT>("RECT", sol::constructors<RECT(int, int, int, int)>());
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
    lua.new_usertype<SIZE>("SIZE", sol::constructors<SIZE(int, int)>());
    lua.new_usertype<Font>("Font", sol::constructors<Font(const tstring&, bool, bool, bool, int)>(),
                           "GetHandle", &Font::GetHandle);

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

    // lua.set_function("DefaultIntPair", []() -> std::pair<int, int> { return {10, 20}; });
    // // Bind C++ functions to Lua
    //
    // // game_engine functions
    // // lua.set_function("SetTitle", [](const std::string& title) { GAME_ENGINE->SetTitle({title.begin(), title.end()}); });
    // lua.set_function("SetWindowPosition", [](int left, int top) { GAME_ENGINE->SetWindowPosition(left, top); });
    // lua.set_function("SetWindowRegion", [](const HitRegion* regionPtr)
    // {
    //     return GAME_ENGINE->SetWindowRegion(regionPtr);
    // });
    // lua.set_function("SetKeyList", [](const std::string& keyList)
    // {
    //     GAME_ENGINE->SetKeyList(tstring(keyList.begin(), keyList.end()));
    // });
    // lua.set_function("SetFrameRate", [](int frameRate) { GAME_ENGINE->SetFrameRate(frameRate); });
    // lua.set_function("SetWidth", [](int width) { GAME_ENGINE->SetWidth(width); });
    // lua.set_function("SetHeight", [](int height) { GAME_ENGINE->SetHeight(height); });
    // lua.set_function("GoFullscreen", []() { return GAME_ENGINE->GoFullscreen(); });
    // lua.set_function("GoWindowedMode", []() { return GAME_ENGINE->GoWindowedMode(); });
    // lua.set_function("ShowMousePointer", [](bool value) { GAME_ENGINE->ShowMousePointer(value); });
    // lua.set_function("Quit", []() { GAME_ENGINE->Quit(); });
    // lua.set_function("HasWindowRegion", []() { return GAME_ENGINE->HasWindowRegion(); });
    // lua.set_function("IsFullscreen", []() { return GAME_ENGINE->IsFullscreen(); });
    // lua.set_function("IsKeyDown", [](int vKey) -> bool
    // {
    //     return GAME_ENGINE->IsKeyDown(vKey);
    // });
    // lua.set_function("MessageBox", [](const std::string& message)
    // {
    //     GAME_ENGINE->MessageBox(tstring(message.begin(), message.end()));
    // });
    // lua.set_function("MessageContinue", [](const std::string& message)
    // {
    //     return GAME_ENGINE->MessageContinue(tstring(message.begin(), message.end()));
    // });
    // lua.set_function("CalculateTextDimensions", [](const std::string& text, const Font* fontPtr) -> std::pair<int, int>
    // {
    //     auto dimensions = GAME_ENGINE->CalculateTextDimensions(tstring(text.begin(), text.end()), fontPtr);
    //     return {dimensions.cx, dimensions.cy};
    // });
    // lua.set_function("CalculateTextDimensions", sol::overload(
    //                      [](const std::string& text) -> std::pair<int, int>
    //                      {
    //                          auto font = std::make_unique<Font>(_T("Arial"), false, false, false, 12);
    //                          auto dimensions = GAME_ENGINE->CalculateTextDimensions(
    //                              tstring(text.begin(), text.end()), font.get());
    //                          return {dimensions.cx, dimensions.cy};
    //                      },
    //                      [](const std::string& text, const std::string& fontName, int size) -> std::pair<int, int>
    //                      {
    //                          auto font = std::make_unique<Font>(tstring(fontName.begin(), fontName.end()), false, false,
    //                                                             false, size);
    //                          auto dimensions = GAME_ENGINE->CalculateTextDimensions(
    //                              tstring(text.begin(), text.end()), font.get());
    //                          return {dimensions.cx, dimensions.cy};
    //                      }
    //                  ));
    // //
    // // lua.set_function("CalculateTextDimensions", [](const std::string& text) -> std::pair<int, int>
    // // {
    // //     auto font = std::make_unique<Font>(_T("Arial"), false, false, false, 12);
    // //     auto dimensions = GAME_ENGINE->CalculateTextDimensions(tstring(text.begin(), text.end()), font.get());
    // //     return {dimensions.cx, dimensions.cy};
    // // });
    // // lua.set_function("CalculateTextDimensions",
    // //                  [](const std::string& text, const std::string& fontName, int size) -> std::pair<int, int>
    // //                  {
    // //                      auto font = std::make_unique<Font>(
    // //                          tstring(fontName.begin(), fontName.end()), false, false, false, size);
    // //                      auto dimensions = GAME_ENGINE->
    // //                          CalculateTextDimensions(tstring(text.begin(), text.end()), font.get());
    // //                      return {dimensions.cx, dimensions.cy};
    // //                  });
    // // lua.set_function("CalculateTextDimensions", [](const std::string& text, const Font* fontPtr) -> sol:std::pair<int,int>
    // // {
    // //     return GAME_ENGINE->CalculateTextDimensions(tstring(text.begin(), text.end()), fontPtr);
    // // });
    // // lua.set_function("CalculateTextRect", [](const std::string& text, const Font* fontPtr, int left, int top, int right,
    // //                                          int bottom) -> sol:std::pair<int, int>
    // // {
    // //     return GAME_ENGINE->CalculateTextDimensions(tstring(text.begin(), text.end()), fontPtr,
    // //                                                 RECT(left, top, right, bottom));
    // // });
    // // TODO: Implement these functions
    // lua.set_function("SetColor", [](const sol::variadic_args& args)
    // {
    //     try
    //     {
    //         if (args.size() == 3) // Expect 3 separate integers for RGB
    //         {
    //             // Get RGB values from the arguments
    //             int r = args[0];
    //             int g = args[1];
    //             int b = args[2];
    //
    //             // Create the packed COLORREF value using the RGB macro
    //             COLORREF color = RGB(r, g, b);
    //             GAME_ENGINE->SetColor(color);
    //         }
    //         else
    //         {
    //             throw std::invalid_argument("Expected 3 integers for RGB.");
    //         }
    //     }
    //     catch (const std::exception& e)
    //     {
    //         std::cout << "Error in SetColor: " << e.what() << std::endl;
    //     }
    // });
    // lua.set_function("SetFont", [](Font* fontPtr) { GAME_ENGINE->SetFont(fontPtr); });
    // lua.set_function("FillWindowRect", [](const sol::variadic_args& args)
    // {
    //     try
    //     {
    //         if (args.size() == 3) // Expect 3 separate integers for RGB
    //         {
    //             // Get RGB values from the arguments
    //             int r = args[0];
    //             int g = args[1];
    //             int b = args[2];
    //
    //             GAME_ENGINE->FillWindowRect(RGB(r, g, b));
    //         }
    //         else
    //         {
    //             throw std::invalid_argument("Expected 3 integers for RGB.");
    //         }
    //     }
    //     catch (const std::exception& e)
    //     {
    //         std::cout << "Error in FillWindowRect: " << e.what() << std::endl;
    //     }
    // });
    // lua.set_function("DrawLine", [](int x1, int y1, int x2, int y2)
    // {
    //     return GAME_ENGINE->DrawLine(x1, y1, x2, y2);
    // });
    // lua.set_function("DrawRect", [](int left, int top, int right, int bottom)
    // {
    //     return GAME_ENGINE->DrawRect(left, top, right, bottom);
    // });
    // lua.set_function("FillRect", [](int left, int top, int right, int bottom)
    // {
    //     return GAME_ENGINE->FillRect(left, top, right, bottom);
    // });
    // lua.set_function("FillRectWithOpacity", [](int left, int top, int right, int bottom, int opacity)
    // {
    //     return GAME_ENGINE->FillRect(left, top, right, bottom, opacity);
    // });
    // lua.set_function("DrawRoundRect", [](int left, int top, int right, int bottom, int radius)
    // {
    //     return GAME_ENGINE->DrawRoundRect(left, top, right, bottom, radius);
    // });
    // lua.set_function("FillRoundRect", [](int left, int top, int right, int bottom, int radius)
    // {
    //     return GAME_ENGINE->FillRoundRect(left, top, right, bottom, radius);
    // });
    // lua.set_function("DrawOval", [](int left, int top, int right, int bottom)
    // {
    //     return GAME_ENGINE->DrawOval(left, top, right, bottom);
    // });
    // lua.set_function("FillOval", [](int left, int top, int right, int bottom)
    // {
    //     return GAME_ENGINE->FillOval(left, top, right, bottom);
    // });
    // lua.set_function("FillOvalWithOpacity", [](int left, int top, int right, int bottom, int opacity)
    // {
    //     return GAME_ENGINE->FillOval(left, top, right, bottom, opacity);
    // });
    // lua.set_function("DrawArc", [](int left, int top, int right, int bottom, int startDegree, int angle)
    // {
    //     return GAME_ENGINE->DrawArc(left, top, right, bottom, startDegree, angle);
    // });
    // lua.set_function("FillArc", [](int left, int top, int right, int bottom, int startDegree, int angle)
    // {
    //     return GAME_ENGINE->FillArc(left, top, right, bottom, startDegree, angle);
    // });
    // // lua.set_function("DrawString", [](const std::string& text, int left, int top)
    // // {
    // //     return GAME_ENGINE->DrawString(tstring(text.begin(), text.end()), left, top);
    // // });
    // // lua.set_function("DrawStringInRect", [](const std::string& text, int left, int top, int right, int bottom)
    // // {
    // //     return GAME_ENGINE->DrawString(tstring(text.begin(), text.end()), left, top, right, bottom);
    // // });
    // lua.set_function("DrawBitmap", [](const Bitmap* bitmapPtr, int left, int top)
    // {
    //     return GAME_ENGINE->DrawBitmap(bitmapPtr, left, top);
    // });
    // lua.set_function("DrawBitmapWithSourceRect", [](const Bitmap* bitmapPtr, int left, int top, const RECT& sourceRect)
    // {
    //     return GAME_ENGINE->DrawBitmap(bitmapPtr, left, top, sourceRect);
    // });
    // lua.set_function("DrawPolygon", [](const std::vector<POINT>& ptsArr, bool close)
    // {
    //     return GAME_ENGINE->DrawPolygon(ptsArr.data(), static_cast<int>(ptsArr.size()), close);
    // });
    // lua.set_function("FillPolygon", [](const std::vector<POINT>& ptsArr, bool close)
    // {
    //     return GAME_ENGINE->FillPolygon(ptsArr.data(), static_cast<int>(ptsArr.size()), close);
    // });
    // lua.set_function("GetDrawColor", []() { return GAME_ENGINE->GetDrawColor(); });
    // lua.set_function("Repaint", []() { return GAME_ENGINE->Repaint(); });
    // lua.set_function("GetTitle", []() { return GAME_ENGINE->GetTitle(); });
    // lua.set_function("GetInstance", []() { return GAME_ENGINE->GetInstance(); });
    // lua.set_function("GetWindow", []() { return GAME_ENGINE->GetWindow(); });
    // lua.set_function("GetWidth", []() { return GAME_ENGINE->GetWidth(); });
    // lua.set_function("GetHeight", []() { return GAME_ENGINE->GetHeight(); });
    // lua.set_function("GetFrameRate", []() { return GAME_ENGINE->GetFrameRate(); });
    // lua.set_function("GetFrameDelay", []() { return GAME_ENGINE->GetFrameDelay(); });
    // lua.set_function("GetWindowPosition", []() { return GAME_ENGINE->GetWindowPosition(); });
    // lua.set_function("TabNext", [](HWND ChildWindow) { GAME_ENGINE->TabNext(ChildWindow); });
    // lua.set_function("TabPrevious", [](HWND ChildWindow) { GAME_ENGINE->TabPrevious(ChildWindow); });
    //
    // lua.set_function("ShowWarning", [this](const std::wstring& message) { this->ShowWarning(message); });
    // lua.set_function("whatVersion", [this]() { this->WhatVersion(); });
    // lua.set_function("whatFile", &Game::WhatFile, this);
    // lua.set_function("setScript", &Game::SetScript, this);
    // lua["print"] = [](const std::string& message) { std::cout << message << std::endl; };
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
