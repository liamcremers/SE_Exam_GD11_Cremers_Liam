FetchContent_Declare(
    SDL2
    URL https://github.com/libsdl-org/SDL/releases/download/release-2.30.9/SDL2-2.30.9.tar.gz
    SOURCE_DIR ${CMAKE_BINARY_DIR}/third_party/SDL2
    BINARY_DIR ${CMAKE_BINARY_DIR}/third_party/SDL2-build
)
set(SDL_TEST OFF CACHE BOOL "Disable SDL2 tests" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Disable SDL2 shared library" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
FetchContent_MakeAvailable(SDL2)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_SOURCE_DIR}/include)

# move SDL2 targets to a separate folder in the IDE
set_target_properties(SDL2-static PROPERTIES FOLDER "ThirdParty/SDL2")
set_target_properties(SDL2main PROPERTIES FOLDER "ThirdParty/SDL2")
set_target_properties(sdl_headers_copy PROPERTIES FOLDER "ThirdParty/SDL2")
set_target_properties(uninstall PROPERTIES FOLDER "ThirdParty/SDL2")