# cMakeLists.txt
# Author: Galen Cochrane
# Project: Duality Engine
# Date: 13 August 2015

# No lesser version of cMake may be used.
cmake_minimum_required( VERSION 3.3 )
set( LIBS ${LIBS} duality_extern)

# Declare the project in question
project( DualityEngine )

# Tell cMake where to look for the modules that search for SDL2 and Assimp (the others come with cMake)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" )

# Build and assemble third-party submodule dependencies
add_subdirectory( "./extern" )

# Find SDL 2.0
find_package( SDL2 REQUIRED )
include_directories( ${SDL2_INCLUDE_DIR} )
set( LIBS ${LIBS} ${SDL2_LIBRARY} )

# Find GLEW
find_package( GLEW REQUIRED )
include_directories( ${GLEW_INCLUDE_PATH} )
set( LIBS ${LIBS} ${GLEW_LIBRARY} )

# Find OpenGL
find_package( OpenGL REQUIRED )
include_directories( ${OPENGL_INCLUDE_DIR} )
set( LIBS ${LIBS} ${OPENGL_LIBRARIES} )

# Find Freetype2
set(FREETYPE_LIBRARY "$ENV{FREETYPE_DIR}/win64/freetype.lib")
find_package( Freetype REQUIRED )
include_directories( ${FREETYPE_INCLUDE_DIR_ft2build} )
include_directories( ${FREETYPE_INCLUDE_DIR_freetype2} )
set( LIBS ${LIBS} ${FREETYPE_LIBRARIES} )

# recursively add all the source directories
add_subdirectory( source )

# include all the directories recursively added by the above call
include_directories( ${INCLUDE_DIRS} )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES} "main.cpp" source/assetManagement/repos/SoundRepo.cpp source/assetManagement/repos/TextureRepo.cpp source/assetManagement/repos/FontRepo.cpp source/assetManagement/repos/FontRepo.h source/assetManagement/loaders/loadFont.cpp source/assetManagement/loaders/loadFont.h source/utilities/errorChecks.cpp source/utilities/errorChecks.h source/ui/ConsoleDrawer.cpp source/ui/ConsoleDrawer.h source/assetManagement/programmaticAssets/DebugFontView.cpp source/assetManagement/programmaticAssets/DebugFontView.h)

# Set compiler flags
if( NOT WIN32 )
    # these are the compiler flags to be used on gcc
    set( CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS} -ggdb3     -Wall -std=c++11 -D_REENTRANT" )
    set( CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS} -Ofast     -Wall -std=c++11 -D_REENTRANT" )
    set( CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS} -Og -ggdb3 -Wall -std=c++11 -D_REENTRANT" )
endif()

# Make an executable called DualityEngine out of all the source files
add_executable( ${PROJECT_NAME} ${SOURCE_FILES} )

# Link to external libraries that were found above
target_link_libraries( ${PROJECT_NAME} ${LIBS} )

# Copy asset tree into build tree
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)
