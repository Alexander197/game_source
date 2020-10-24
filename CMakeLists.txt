cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

set(PROFECT_NAME game_source)
project(${PROFECT_NAME})

add_executable(${PROFECT_NAME} src/main.cpp)

#set(CMAKE_C_COMPILER /C/"Program files (x86)"/"Microsoft Visual Studio"/2019/professional/VC/Tools/MSVC/14.27.29110/bin/hostx64/x64/cl.exe)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_iNSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROFECT_NAME} glfw)