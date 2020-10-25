cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

set(PROJECT_NAME game_source)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDART	 17)

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h)

#set(CMAKE_C_COMPILER /C/"Program files (x86)"/"Microsoft Visual Studio"/2019/professional/VC/Tools/MSVC/14.27.29110/bin/hostx64/x64/cl.exe)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_iNSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)