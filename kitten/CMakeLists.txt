cmake_minimum_required (VERSION 3.8)

project("kitten" CXX)

add_executable (${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} glew_s glfw)
target_link_libraries(${PROJECT_NAME} eeeh)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory
    "${CMAKE_CURRENT_BINARY_DIR}/shader"
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/shader"
    "${CMAKE_CURRENT_BINARY_DIR}/shader"
)


if(WIN32)
    set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true)
endif(WIN32)
