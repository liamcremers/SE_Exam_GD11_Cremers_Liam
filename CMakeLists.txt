cmake_minimum_required(VERSION 3.20)
project(SE_Exam_GD11_Cremers_Liam)

# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the executable name
set(EXE_NAME SE_Exam_GD11_Cremers_Liam)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Game")

# Set warning level 3
if(MSVC)
    add_compile_options(/W3 /permissive- /sdl)
endif()

# Source files
set(ENGINE_FILES
    AbstractGame.cpp
    AbstractGame.h
    GameDefines.h
    GameEngine.cpp
    GameEngine.h
    GameWinMain.cpp
    GameWinMain.h
)

set(GAME_FILES
    Game.cpp
    Game.h
)

file(GLOB LUA_FILES Lua/*.lua)

set(RESOURCE_FILES
    resource.h
    Game.rc
    small.ico
    big.ico
)

# Combine all source files
set(ALL_FILES
    ${LUA_FILES}
    ${ENGINE_FILES}
    ${GAME_FILES}
    ${RESOURCE_FILES}
)

# Add executable target
add_executable(${PROJECT_NAME} ${ALL_FILES})

# Source groups for filters
source_group("Engine Files" FILES ${ENGINE_FILES})
source_group("Game Files" FILES ${GAME_FILES})
source_group("Game Files\\Lua" FILES ${LUA_FILES})
source_group("Resource Files" FILES ${RESOURCE_FILES})

# Link options
if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
endif()

# Definitions for Unicode and Windows
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(FetchContent)
include(Lua)
include(Sol2)

# Force Lua Script Copy
add_custom_target(CopyScripts ALL
    COMMENT "Copying Lua scripts to output directory"
)
set_target_properties(CopyScripts PROPERTIES FOLDER "ThirdParty/dae")

FOREACH(LUA_FILE IN LISTS LUA_FILES)
    get_filename_component(LUA_FILE_NAME ${LUA_FILE} NAME)
    add_custom_command(TARGET CopyScripts POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/Lua/${LUA_FILE_NAME}
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LUA_FILE_NAME})
ENDFOREACH()

add_dependencies(${EXE_NAME} CopyScripts)