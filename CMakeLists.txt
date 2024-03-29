cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

set(PROJECT_NAME game_source)
project(${PROJECT_NAME})


add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/Renderer/ShaderProgram.cpp
	src/Renderer/ShaderProgram.h
	src/Renderer/Sprite.cpp
	src/Renderer/Sprite.h
	src/Resources/ResourceManager.cpp
	src/Resources/ResourceManager.h
	src/Resources/stb_image.h
	src/Renderer/Texture2D.cpp
	src/Renderer/Texture2D.h
	src/Renderer/VertexBuffer.cpp
	src/Renderer/VertexBuffer.h
	src/Renderer/IndexBuffer.cpp
	src/Renderer/IndexBuffer.h
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexArray.h
	src/Renderer/VertexBufferLayout.cpp
	src/Renderer/VertexBufferLayout.h
	src/Renderer/Renderer.cpp
	src/Renderer/Renderer.h
	src/Renderer/SpriteAnimator.cpp
	src/Renderer/SpriteAnimator.h
	src/Game/Game.cpp
	src/Game/Game.h
	src/Game/GameObjects/Tank.cpp
	src/Game/GameObjects/Tank.h
	src/Game/GameObjects/IGameObject.cpp
	src/Game/GameObjects/IGameObject.h
	src/Game/GameObjects/BrickWall.h
	src/Game/GameObjects/BrickWall.cpp
	src/Game/Level.cpp
	src/Game/Level.h
 )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#set(CMAKE_C_COMPILER /C/"Program files (x86)"/"Microsoft Visual Studio"/2019/professional/VC/Tools/MSVC/14.27.29110/bin/hostx64/x64/cl.exe)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_iNSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)
include_directories(external/rapidjson/include)

#add_subdirectory(external/glm)
#target_link_libraries(${PROJECT_NAME} glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)