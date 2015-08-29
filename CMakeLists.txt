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

set(DUA_INCLUDE_DIR
    Assets/GUI/NoCamBackground
    Assets/GUI
    Assets/Models/DebugCube

    Headers/Components
    Headers/Bank
    Headers/Admin
    Headers/Systems
    Headers/Utilities
    Headers/Math
    Headers/AssetManagement
    )
include_directories(${DUA_INCLUDE_DIR})

# Find all the source files
set(SOURCE_FILES
    Assets/GUI/NoCamBackground/DebugBackground.h
    Assets/GUI/GUI_Console.h
    Assets/Models/DebugCube/DebugCube.h

        Headers/Components/AmbientLight.h
        Headers/Components/AngularVelocity.h
        Headers/Bank/BankDelegates.h
        Headers/Components/CameraFree.h
        Headers/Components/Collision.h
        Headers/Components/Component.h
        Headers/Bank/ComponentBank.h
        Headers/Components/ComponentCollection.h
        Headers/Utilities/Console.h
        Headers/Systems/Control_SS.h
        Headers/Bank/ControlTypes.h
        Headers/Utilities/Delegate.h
        Headers/Components/DirectionalLight.h
        Headers/Admin/Engine.h
        Headers/AssetManagement/FileIO.h
        Headers/Math/Functions.h
        Headers/Admin/Game.h
        Headers/Bank/HashMap.h
        Headers/Components/LinearVelocity.h
        Headers/AssetManagement/loadCubeMap.h
        Headers/AssetManagement/loadMesh.h
        Headers/AssetManagement/loadShaders.h
        Headers/AssetManagement/loadTexture.h
        Headers/Math/Matrix.h
        Headers/AssetManagement/MeshRepository.h
        Headers/Components/Model.h
        Headers/Components/Orientation.h
        Headers/Components/OrientationEuler.h
        Headers/Components/Owner.h
        Headers/Systems/PhysCollide.h
        Headers/Systems/PhysMove.h
        Headers/Components/PointLight.h
        Headers/Components/Position.h
        Headers/Components/PositionChild.h
        Headers/Components/PositionParent.h
        Headers/Systems/Render_Background.h
        Headers/Systems/Render_BiTcubes.h
        Headers/Systems/Render_Console.h
        Headers/Systems/Render_Master.h
        Headers/Systems/Render_Models.h
        Headers/Components/RotationVelocEuler.h
        Headers/Components/Score.h
        Headers/Systems/Scripting.h
        Headers/Utilities/Settings.h
        Headers/AssetManagement/ShaderRepository.h
        Headers/Components/Soul.h
        Headers/AssetManagement/SoundRepository.h
        Headers/Components/SpaceControl.h
        Headers/AssetManagement/stb_image.h
        Headers/Systems/System.h
        Headers/AssetManagement/TextureRepository.h
        Headers/Systems/UserInput.h

        Source/Entities/Box.cpp
        Source/Bank/ComponentBank.cpp
        Source/Utilities/Console.cpp
        Source/Systems/Control_SS.cpp
        Source/Admin/Engine.cpp
        Source/AssetManagement/FileIO.cpp
        Source/Math/Functions.cpp
        Source/Admin/Game.cpp
        Source/Bank/HashMap.cpp
        Source/AssetManagement/loadCubeMap.cpp
        Source/AssetManagement/loadShaders.cpp
        Source/Admin/main.cpp
        Source/Systems/PhysCollide.cpp
        Source/Systems/PhysMove.cpp
        Source/Systems/Render_Background.cpp
        Source/Systems/Render_BiTcubes.cpp
        Source/Systems/Render_Console.cpp
        Source/Systems/Render_Master.cpp
        Source/Systems/Render_Models.cpp
        Source/Systems/Scripting.cpp
        Source/Utilities/Settings.cpp
        Source/AssetManagement/ShaderRepository.cpp
        Source/Systems/System.cpp
        Source/Systems/UserInput.cpp)

# Make an executable called DualityEngine out of all the above source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link to external libraries that were found above
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy asset tree into build tree
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets)