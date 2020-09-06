cmake_minimum_required(VERSION 3.8)

project("ClickKitten")

if (MSVC)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    add_link_options(/IGNORE:4281)
endif ()

set(OpenGL_GL_PREFERENCE GLVND)
add_subdirectory("glew-2.1.0/build/cmake")
target_compile_definitions(glew_s PUBLIC GLEW_NO_GLU)
target_include_directories(glew_s PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/glew-2.1.0/include>")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("glfw-3.3.2")

if (MSVC)
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif ()
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

add_subdirectory("kitten")
add_subdirectory("eeeh")

