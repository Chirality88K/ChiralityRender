cmake_minimum_required(VERSION 3.20)
set (CMAKE_C_COMPILER "D:/mingw64/bin/gcc.exe")
set (CMAKE_CXX_COMPILER "D:/mingw64/bin/g++.exe")
project(ChiralityRender VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_DEBUG)

include_directories(${PROJECT_SOURCE_DIR}/inc)
include_directories(${PROJECT_SOURCE_DIR}/glm)
include_directories(${PROJECT_SOURCE_DIR}/OpenGL/include)
include_directories(${PROJECT_SOURCE_DIR}/OpenGL/include/GL)
include_directories(${PROJECT_SOURCE_DIR}/OpenGL/include/GLFW)
include_directories(${PROJECT_SOURCE_DIR}/assimp_inc)

link_directories(${PROJECT_SOURCE_DIR}/lib)
link_directories(${PROJECT_SOURCE_DIR}/OpenGL/Libs)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
add_executable(ChiralityRender_Debug ChiralityDemo.cpp ${SRC_LIST})

target_link_libraries(ChiralityRender_Debug glew32s.lib glfw3.lib opengl32.lib)
target_link_libraries(ChiralityRender_Debug assimp-vc142-mtd.lib)
target_link_libraries(ChiralityRender_Debug zlibstaticd.lib)

file(COPY Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})