FetchContent_Declare(
        lua
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG v5.4.7
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(lua)

add_library(lua STATIC ${lua_SOURCE_DIR}/onelua.c)
target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE lua)

# move Lua targets to a separate folder in the IDE
set_target_properties(lua PROPERTIES FOLDER "ThirdParty/Lua")
#set_target_properties(lua_lib PROPERTIES FOLDER "ThirdParty/Lua")