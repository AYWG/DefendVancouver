cmake_minimum_required(VERSION 3.9)
project(DefendVancouver)

set(CMAKE_CXX_STANDARD 11)


add_executable(DefendVancouver src/main.cpp src/world.cpp src/world.hpp gl3w/include/GL/gl3w.h gl3w/include/GL/glcorearb.h  gl3w/src/gl3w.c stb-cmake/stb_image.h src/common.cpp src/common.hpp src/player.cpp src/player.hpp src/camera.cpp src/camera.hpp src/enemies/shooter.cpp src/enemies/shooter.hpp src/enemies/chaser.cpp src/enemies/chaser.hpp src/background.cpp src/background.hpp src/pbullet.cpp src/pbullet.hpp src/enemies/enemy.cpp src/enemies/enemy.hpp src/ai/behaviourTreeNode.cpp src/ai/behaviourTreeNode.hpp src/ai/compositeNode.cpp src/ai/compositeNode.hpp src/ai/leafNode.cpp src/ai/leafNode.hpp src/ai/sequenceNode.cpp src/ai/sequenceNode.hpp src/ai/selectorNode.cpp src/ai/selectorNode.hpp src/ai/conditionNode.cpp src/ai/conditionNode.hpp src/ai/actionNode.cpp src/ai/actionNode.hpp src/ai/actions/moveToLocation.cpp src/ai/actions/moveToLocation.hpp src/ai/actions/rotateToOrientation.cpp src/ai/actions/rotateToOrientation.hpp src/ai/conditions/isPlayerNearBomb.cpp src/ai/conditions/isPlayerNearBomb.hpp src/ai/conditions/isPlayerInVision.cpp src/ai/conditions/isPlayerInVision.hpp src/ai/actions/attack.cpp src/ai/actions/attack.hpp src/enemies/shooterBullet.cpp src/enemies/shooterBullet.hpp src/ai/aI.cpp src/ai/aI.hpp src/ai/shooterAI.cpp src/ai/shooterAI.hpp src/ai/decoratorNode.cpp src/ai/decoratorNode.hpp src/ai/repeatUntilSuccess.cpp src/ai/repeatUntilSuccess.hpp src/bomb.cpp src/bomb.hpp)




set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("glfw-3.2.1")


target_link_libraries(DefendVancouver glfw)
target_link_libraries(DefendVancouver gl3w)

find_package(OpenGL REQUIRED)
find_package(gl3w REQUIRED)

target_link_libraries(DefendVancouver ${OPENGL_gl_LIBRARY})
target_link_libraries(DefendVancouver ${OPENGL_glu_LIBRARY})