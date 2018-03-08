cmake_minimum_required(VERSION 3.9)
project(DefendVancouver)

set(CMAKE_CXX_STANDARD 11)



add_executable(DefendVancouver src/main.cpp src/world.cpp src/world.hpp gl3w/include/GL/gl3w.h gl3w/include/GL/glcorearb.h  gl3w/src/gl3w.c stb-cmake/stb_image.h src/common.cpp src/common.hpp src/player.cpp src/player.hpp src/camera.cpp src/camera.hpp src/enemies/shooter.cpp src/enemies/shooter.hpp src/enemies/chaser.cpp src/enemies/chaser.hpp src/background.cpp src/background.hpp src/pbullet.cpp src/pbullet.hpp src/enemies/enemy.cpp src/enemies/enemy.hpp src/ai/behaviourTree/behaviourTreeNode.cpp src/ai/behaviourTree/behaviourTreeNode.hpp src/ai/behaviourTree/compositeNode.cpp src/ai/behaviourTree/compositeNode.hpp src/ai/behaviourTree/leafNode.cpp src/ai/behaviourTree/leafNode.hpp src/ai/behaviourTree/sequenceNode.cpp src/ai/behaviourTree/sequenceNode.hpp src/ai/behaviourTree/selectorNode.cpp src/ai/behaviourTree/selectorNode.hpp src/ai/behaviourTree/conditionNode.cpp src/ai/behaviourTree/conditionNode.hpp src/ai/behaviourTree/actionNode.cpp src/ai/behaviourTree/actionNode.hpp src/ai/behaviourTree/actions/moveIntoRange.cpp src/ai/behaviourTree/actions/moveIntoRange.hpp src/ai/behaviourTree/actions/rotateToTarget.cpp src/ai/behaviourTree/actions/rotateToTarget.hpp src/ai/behaviourTree/conditions/IsPlayerNearBomb.cpp src/ai/behaviourTree/conditions/IsPlayerNearBomb.hpp src/ai/behaviourTree/conditions/isPlayerInVision.cpp src/ai/behaviourTree/conditions/isPlayerInVision.hpp src/ai/behaviourTree/actions/attack.cpp src/ai/behaviourTree/actions/attack.hpp src/enemies/shooterBullet.cpp src/enemies/shooterBullet.hpp src/ai/aI.cpp src/ai/aI.hpp src/ai/shooterAI.cpp src/ai/shooterAI.hpp src/ai/behaviourTree/decorators/decoratorNode.cpp src/ai/behaviourTree/decorators/decoratorNode.hpp src/ai/behaviourTree/decorators/repeatUntilFailure.cpp src/ai/behaviourTree/decorators/repeatUntilFailure.hpp src/ai/chaserAI.cpp src/ai/chaserAI.hpp src/bomb.cpp src/bomb.hpp src/ai/behaviourTree/conditions/areBombsInVision.cpp src/ai/behaviourTree/conditions/areBombsInVision.hpp src/ai/behaviourTree/decorators/succeeder.cpp src/ai/behaviourTree/decorators/succeeder.hpp)





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