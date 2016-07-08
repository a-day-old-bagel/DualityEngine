# cMakeLists.txt
# Author: Galen Cochrane
# Project: Duality Engine
# Date: 13 August 2015

# Script that allows cMake to find sources and compile Duality Engine on (hopefully) any platform...

# No lesser version of cMake may be used.
cmake_minimum_required(VERSION 3.3)

# Declare the project in question
project(DualityEngine)

# Make sure the project compiles with the c++11 and thread safety flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -D_REENTRANT -Wall -O3")

# Tell cMake where to look for the modules that search for SDL2 and Assimp (the others come with cMake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Find SDL 2.0
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
set(LIBS ${LIBS} ${SDL2_LIBRARY})

# Find GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_PATH})
set(LIBS ${LIBS} ${GLEW_LIBRARY})

# Find OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

# Find Assimp
find_package(Assimp REQUIRED)
include_directories(assimp_INCLUDE_DIRS)
set(LIBS ${LIBS} ${assimp_LIBRARIES})

# Find Freetype2
find_package(Freetype REQUIRED)
include_directories(${FREETYPE_INCLUDE_DIR_ft2build})
include_directories(${FREETYPE_INCLUDE_DIR_freetype2})
set(LIBS ${LIBS} ${FREETYPE_LIBRARIES})

set(DUA_INCLUDE_DIR
		source/assetManagement/ProgrammaticAssets/GUI
		source/assetManagement/ProgrammaticAssets
		source/components
		source/bank
		source/Admin
		source/systems
		source/utilities
		source/math
		source/assetManagement
		source/
    )
include_directories(${DUA_INCLUDE_DIR})

# Find all the source files
set(SOURCE_FILES
		main.cpp
		source/components/AmbientLight.h
		source/components/AngularVelocity.h
		source/bank/BankDelegates.h
		source/components/CameraFree.h
		source/components/Collision.h
		source/components/Component.h
		source/bank/controlTypes.h
		source/utilities/Delegate.h
		source/components/DirectionalLight.h
		source/utilities/Engine.h
		source/bank/KvMap.h
		source/components/LinearVelocity.h
		source/assetManagement/loadMesh.h
		source/assetManagement/loadTexture.h
		source/math/Matrix.h
		source/components/Model.h
		source/components/Orientation.h
		source/components/OrientationEuler.h
		source/components/Owner.h
		source/components/PointLight.h
		source/components/Position.h
		source/components/PositionChild.h
		source/components/PositionParent.h
		source/components/RotationVelocEuler.h
		source/components/Score.h
		source/components/Soul.h
		source/assetManagement/SoundRepository.h
		source/components/SpaceControl.h
		source/assetManagement/stb_image.h
		source/systems/System.h
		source/assetManagement/TextureRepository.h
		source/utilities/concurrentqueue.h
		source/utilities/readerwriterqueue.h
		source/entities/Box.cpp
		source/bank/Bank.cpp source/bank/Bank.h
		source/utilities/Console.cpp source/utilities/Console.h
		source/systems/Control_SS.cpp source/systems/Control_SS.h
		source/assetManagement/fileIO.cpp source/assetManagement/fileIO.h
		source/math/functions.cpp source/math/functions.h
		source/Game.cpp source/Game.h
		source/assetManagement/loadCubeMap.cpp source/assetManagement/loadCubeMap.h
		source/assetManagement/loadShaders.cpp source/assetManagement/loadShaders.h
		source/systems/PhysCollide.cpp source/systems/PhysCollide.h
		source/systems/PhysMove.cpp source/systems/PhysMove.h
		source/systems/Render_Background.cpp source/systems/Render_Background.h
		source/systems/Render_BiTcubes.cpp source/systems/Render_BiTcubes.h
		source/systems/Render_Console.cpp source/systems/Render_Console.h
		source/systems/Render_Master.cpp source/systems/Render_Master.h
		source/systems/Render_Models.cpp source/systems/Render_Models.h
		source/systems/Scripting.cpp source/systems/Scripting.h
		source/utilities/settings.cpp source/utilities/settings.h
		source/assetManagement/ShaderRepository.cpp source/assetManagement/ShaderRepository.h
		source/systems/UserInput.cpp source/systems/UserInput.h
		source/assetManagement/ProgrammaticAssets/DebugBackground.cpp source/assetManagement/ProgrammaticAssets/DebugBackground.h
		source/utilities/helpDocs.cpp source/utilities/helpDocs.h
		source/utilities/toString.cpp source/utilities/toString.h
		source/assetManagement/MeshRepository.cpp source/assetManagement/MeshRepository.h
		source/utilities/Event.cpp source/utilities/Event.h
		source/math/extendGlm.cpp source/math/extendGlm.h
		source/utilities/TimePiece.cpp source/utilities/TimePiece.h
		)

# Make an executable called DualityEngine out of all the above source files
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link to external libraries that were found above
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Copy asset tree into build tree
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)