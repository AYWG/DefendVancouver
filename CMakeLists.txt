cmake_minimum_required(VERSION 3.9)
project(DefendVancouver)

set(CMAKE_CXX_STANDARD 11)



file(GLOB_RECURSE SOURCE_FILES "src/*.hpp" "src/*.cpp")
add_executable(DefendVancouver gl3w/include/GL/gl3w.h gl3w/include/GL/glcorearb.h  gl3w/src/gl3w.c stb-cmake/stb_image.h  ${SOURCE_FILES})

message(INFO ${SOURCE_FILES})
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("glfw-3.2.1")
#add_subdirectory("glm-0.9.7.1")


target_link_libraries(DefendVancouver glfw)
target_link_libraries(DefendVancouver gl3w)
#target_link_libraries(DefendVancouver glew)

find_package(OpenGL REQUIRED)
find_package(gl3w REQUIRED)
#find_package(glew REQUIRED)

target_link_libraries(DefendVancouver ${OPENGL_gl_LIBRARY})
target_link_libraries(DefendVancouver ${OPENGL_glu_LIBRARY})
#target_link_libraries(DefendVancouver ${OPENGL_glew_LIBRARY})