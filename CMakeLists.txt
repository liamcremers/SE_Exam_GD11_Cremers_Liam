cmake_minimum_required(VERSION 3.20)
project(SE_Exam_GD11_Cremers_Liam)

# Language
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/Game")

# Set warning level 3
if (MSVC)
    add_compile_options(/W3 /permissive-)
endif ()

# Source files
set(ENGINE_FILES
        src/AbstractGame.cpp
        src/AbstractGame.h
        src/GameDefines.h
        src/GameEngine.cpp
        src/GameEngine.h
        src/GameWinMain.cpp
        src/GameWinMain.h
)

set(GAME_FILES
        src/Game.cpp
        src/Game.h
)

file(GLOB LUA_FILES lua/*.lua)

set(RESOURCE_FILES
        src/resource.h
        src/Game.rc
        src/small.ico
        src/big.ico
        src/paddle.mp3
        src/breakBlock.mp3
        src/wall.mp3
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
if (MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
endif ()

# Definitions for Unicode and Windows
target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(FetchContent)
include(Lua)
include(Sol2)

add_custom_target(CopyScripts ALL
        COMMENT "Copying Lua scripts to output directory"
)
set_target_properties(CopyScripts PROPERTIES FOLDER "ThirdParty/dae")

foreach (LUA_FILE IN LISTS LUA_FILES)
    get_filename_component(LUA_FILE_NAME ${LUA_FILE} NAME)
    add_custom_command(TARGET CopyScripts POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LUA_FILE}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${LUA_FILE_NAME})
endforeach ()

foreach (RESOURCE_FILE IN LISTS RESOURCE_FILES)
    get_filename_component(RESOURCE_FILE_NAME ${RESOURCE_FILE} NAME)
    add_custom_command(TARGET CopyScripts POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_FILE}
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${RESOURCE_FILE_NAME})
endforeach ()

add_dependencies(${PROJECT_NAME} CopyScripts)