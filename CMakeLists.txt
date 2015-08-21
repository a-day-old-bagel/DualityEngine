# cMakeLists.txt
# Author: Galen Cochrane
# Project: Duality Engine
# Date: 13 August 2015

# Script that allows cMake to find sources and compile Duality Engine on (hopefully) any platform...

# No lesser version of cMake may be used.
cmake_minimum_required(VERSION 3.3)

# Declare the project in question
project(DualityEngine)

# Tell cMake where to put the generated libraries and executable...
# Evidently cLion ignores these next three sets, but they should be useful outside of cLion.
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Make sure the project compiles with the c++11 and thread safety flags
if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_REENTRANT -Wall -g -O3")
endif()

# Tell cMake where to look for the modules that search for SDL2 and Assimp (the others come with cMake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Find SDL 2.0
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY})

# Find GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
set(LIBS ${LIBS} ${GLEW_LIBRARIES})

# Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

# Find Assimp
find_package(Assimp REQUIRED)
include_directories(${Assimp_INCLUDE_DIR}) # why can't 'Assimp' be 'ASSIMP' here? Throws weird error...
set(LIBS ${LIBS} ${ASSIMP_LIBRARIES})

# Find Freetype2
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_ft2build})
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})

# Find all the source files
set(SOURCE_FILES
    Assets/GUI/NoCamBackground/DebugBackground.h
    Assets/GUI/GUI_Console.h
    Assets/Models/DebugCube/DebugCube.h
    Headers/AmbientLight.h
    Headers/AngularVelocity.h
    Headers/BankDelegates.h
    Headers/CameraFree.h
    Headers/Collision.h
    Headers/Component.h
    Headers/ComponentBank.h
    Headers/ComponentCollection.h
    Headers/Console.h
    Headers/Control_SS.h
    Headers/ControlTypes.h
    Headers/Delegate.h
    Headers/DirectionalLight.h
    Headers/Engine.h
    Headers/FileIO.h
    Headers/Functions.h
    Headers/Game.h
    Headers/Game0.h
    Headers/HashMap.h
    Headers/LinearVelocity.h
    Headers/loadCubeMap.h
    Headers/loadMesh.h
    Headers/loadShaders.h
    Headers/loadTexture.h
    Headers/Matrix.h
    Headers/MeshRepository.h
    Headers/Model.h
    Headers/Orientation.h
    Headers/OrientationEuler.h
    Headers/Owner.h
    Headers/PhysCollide.h
    Headers/PhysMove.h
    Headers/PointLight.h
    Headers/Position.h
    Headers/PositionChild.h
    Headers/PositionParent.h
    Headers/Render_Background.h
    Headers/Render_BiTcubes.h
    Headers/Render_Console.h
    Headers/Render_Master.h
    Headers/Render_Models.h
    Headers/RotationVelocEuler.h
    Headers/Score.h
    Headers/Scripting.h
    Headers/Settings.h
    Headers/ShaderRepository.h
    Headers/Soul.h
    Headers/SoundRepository.h
    Headers/SpaceControl.h
    Headers/stb_image.h
    Headers/System.h
    Headers/technique.h
    Headers/texture.h
    Headers/TextureRepository.h
    Headers/UserInput.h
    Source/Box.cpp
    Source/ComponentBank.cpp
    Source/Console.cpp
    Source/Control_SS.cpp
    Source/Engine.cpp
    Source/FileIO.cpp
    Source/Functions.cpp
    Source/Game.cpp
    Source/HashMap.cpp
    Source/loadCubeMap.cpp
    Source/loadShaders.cpp
    Source/main.cpp
    Source/PhysCollide.cpp
    Source/PhysMove.cpp
    Source/Render_Background.cpp
    Source/Render_BiTcubes.cpp
    Source/Render_Console.cpp
    Source/Render_Master.cpp
    Source/Render_Models.cpp
    Source/Scripting.cpp
    Source/Settings.cpp
    Source/ShaderRepository.cpp
    Source/System.cpp
    Source/technique.cpp
    Source/texture.cpp
    Source/UserInput.cpp)

# Make an executable called DualityEngine out of all the above source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link to external libraries that were found above
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy asset tree into build tree
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets)