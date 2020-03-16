cmake_minimum_required(VERSION 3.15)
project(Engine3D)

set(CMAKE_CXX_STANDARD 17)

add_executable(Engine3D src/main.cpp src/app/application.cpp src/app/application.hpp src/base/object.cpp src/base/object.hpp src/graphics/shaderprogram.cpp src/graphics/shaderprogram.hpp src/graphics/texture.cpp src/graphics/texture.hpp src/graphics/buffer.cpp src/graphics/buffer.hpp src/graphics/material.hpp src/graphics/mesh.hpp src/graphics/node.hpp src/components/camera.hpp src/components/light.hpp src/base/resourcemanager.hpp src/graphics/vertexarray.cpp src/graphics/vertexarray.hpp src/sys/messagemanager.cpp src/sys/messagemanager.hpp src/sys/message.hpp src/sys/messagehandler.cpp src/sys/messagehandler.hpp src/sys/events.hpp src/base/pool.hpp src/graphics/material.cpp src/graphics/meshbatch.cpp src/graphics/meshbatch.hpp src/graphics/shader.hpp src/components/camera.cpp src/graphics/bone.hpp src/graphics/meshbuilder.cpp src/graphics/meshbuilder.hpp src/animations/animation.hpp src/animations/animation.cpp src/graphics/bitmap.cpp src/graphics/bitmap.hpp src/graphics/texturebuilder.cpp src/graphics/texturebuilder.hpp src/sys/input.cpp src/sys/input.hpp src/base/singleton.hpp src/sys/inputstates.hpp src/animations/animstate.hpp src/math/transform.hpp src/app/appinfo.hpp src/app/config.cpp src/app/config.hpp src/sys/log.cpp src/sys/log.hpp src/components/animator.cpp src/components/animator.hpp src/lua/luamanager.cpp src/lua/luamanager.hpp src/scenes/scene.cpp src/scenes/scene.hpp src/graphics/node.cpp src/graphics/skeleton.hpp src/components/light.cpp src/components/component.cpp src/components/component.hpp src/controllers/controller.cpp src/controllers/controller.hpp src/controllers/playercontroller.cpp src/controllers/playercontroller.hpp src/controllers/maincameracontroller.cpp src/controllers/maincameracontroller.hpp src/components/rigidbody.cpp src/components/rigidbody.hpp src/physics/physics.cpp src/physics/physics.hpp src/physics/body.cpp src/physics/body.hpp src/physics/raycast.hpp src/physics/debugdraw.cpp src/physics/debugdraw.hpp src/graphics/gizmos.cpp src/graphics/gizmos.hpp src/physics/character.cpp src/physics/character.hpp src/components/charactermovement.cpp src/components/charactermovement.hpp)

find_package(OpenGL)
target_link_libraries(Engine3D ${OPENGL_gl_LIBRARY})
target_link_libraries(Engine3D ${OPENGL_glu_LIBRARY})
include_directories(src)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
target_link_libraries(Engine3D glfw)
include_directories(lib/glfw/include)

add_definitions(-DGLEW_STATIC)
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(lib/glew/build/cmake)
set(GLEW_INCLUDE_PATH lib/glew/include)
set(GLEW_LIBRARY glew_s)
target_link_libraries(Engine3D ${GLEW_LIBRARY})
include_directories(${GLEW_INCLUDE_PATH})

set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
add_subdirectory(lib/assimp)
target_link_libraries(Engine3D assimp)
include_directories(lib/assimp/include)

add_subdirectory(lib/lua)
target_link_libraries(Engine3D lualib)
include_directories(lib/lua)

set(USE_DOUBLE_PRECISION OFF CACHE BOOL "" FORCE)
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(USE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(USE_GLUT OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET3 OFF CACHE BOOL "" FORCE)
set(BUILD_ENET OFF CACHE BOOL "" FORCE)
set(BUILD_CLSOCKET OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/bullet3)
include_directories(lib/bullet3/src)
target_link_libraries(Engine3D
        LinearMath
        BulletInverseDynamics
        BulletCollision
        BulletDynamics)



add_definitions(-DGLM_FORCE_RADIANS -DGLM_ENABLE_EXPERIMENTAL)
include_directories(lib/glm)
include_directories(lib/filesystem)
include_directories(lib/stb)
include_directories(lib/sol2/include)