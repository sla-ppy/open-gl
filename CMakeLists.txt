cmake_minimum_required(VERSION 3.22)
project(open_gl)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glfw)

# add flags for safer code
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(open_gl src/main.cpp src/glad.c)

target_include_directories (${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)

target_link_libraries(${CMAKE_PROJECT_NAME} glfw)