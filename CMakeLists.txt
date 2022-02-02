cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_C_FLAGS "-Wall -lglfw -lGLEW -lGLU -lGL -lm")

project(OpenglTemplate VERSION 1.0)

file(GLOB_RECURSE SRC_FILES 
	src/*.c 
	src/core/*.c 
	src/core/gfx/*.c
	vendor/stb_image/*.c)

add_executable(OpenglTemplate ${SRC_FILES})

target_include_directories(OpenglTemplate PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include 
	${CMAKE_CURRENT_SOURCE_DIR}/vendor
	/home/david/Desktop/David/Programming/assimp/include)

target_link_directories(OpenglTemplate PUBLIC 
	/home/david/Desktop/David/Programming/assimp/build/bin)

target_link_libraries(OpenglTemplate assimp)
