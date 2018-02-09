cmake_minimum_required(VERSION 3.9)
project(DefendVancouver)

set(CMAKE_CXX_STANDARD 11)

add_executable(DefendVancouver src/main.cpp src/world.cpp src/world.hpp gl3w/include/GL/gl3w.h gl3w/include/GL/glcorearb.h  gl3w/src/gl3w.c stb-cmake/stb_image.h src/common.cpp src/common.hpp src/player.cpp src/player.hpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("glfw-3.2.1")
#add_subdirectory("stb-cmake")
#add_subdirectory("gl3w")


target_link_libraries(DefendVancouver glfw)
target_link_libraries(DefendVancouver gl3w)
#target_link_libraries(DefendVancouver stb)

find_package(OpenGL REQUIRED)
find_package(gl3w REQUIRED)
#find_package(stb REQUIRED)

#[[target_include_directories(DefendVancouver ${OPENGL_INCLUDE_DIR})]]
target_link_libraries(DefendVancouver ${OPENGL_gl_LIBRARY})

target_link_libraries(DefendVancouver ${OPENGL_glu_LIBRARY})