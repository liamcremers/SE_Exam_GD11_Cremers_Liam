FetchContent_Declare(
 lua
 URL https://github.com/marovira/lua/archive/refs/tags/5.4.4.tar.gz
 GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(lua)

target_link_libraries(${PROJECT_NAME} PRIVATE lua::lua)

# move Lua targets to a separate folder in the IDE
set_target_properties(lua_lib PROPERTIES FOLDER "ThirdParty/Lua")