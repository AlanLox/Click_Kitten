cmake_minimum_required (VERSION 3.8)

project("eeeh" CXX)

add_library (${PROJECT_NAME}
    "include/shader_program.h"
    "src/shader_program.cpp")

target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} glew_s glfw)
