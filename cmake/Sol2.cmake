FetchContent_Declare(
  sol2
  GIT_REPOSITORY https://github.com/ThePhD/sol2.git
  GIT_TAG        v3.3.0 # Replace with the version you want
  GIT_SHALLOW    TRUE
)
set(BUILD_TESTING OFF CACHE BOOL "Disable testing for sol2" FORCE)
set(CTEST_TARGETS_ADDED OFF CACHE BOOL "Avoid adding CTest targets" FORCE)
FetchContent_MakeAvailable(sol2)

target_link_libraries(${PROJECT_NAME} PRIVATE sol2)
