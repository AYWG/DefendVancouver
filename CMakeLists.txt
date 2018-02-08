cmake_minimum_required(VERSION 3.9)
project(DefendVancouver)

set(CMAKE_CXX_STANDARD 11)

add_executable(DefendVancouver main.cpp src/world.cpp src/world.hpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("glfw-3.2.1")

target_link_libraries(DefendVancouver glfw)

find_package(OpenGL REQUIRED)

#[[target_include_directories(DefendVancouver ${OPENGL_INCLUDE_DIR})]]
target_link_libraries(DefendVancouver ${OPENGL_gl_LIBRARY})

target_link_libraries(DefendVancouver ${OPENGL_glu_LIBRARY})