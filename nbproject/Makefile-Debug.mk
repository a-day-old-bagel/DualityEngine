#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Source/Box.o \
	${OBJECTDIR}/Source/ComponentBank.o \
	${OBJECTDIR}/Source/Engine.o \
	${OBJECTDIR}/Source/FileIO.o \
	${OBJECTDIR}/Source/Game.o \
	${OBJECTDIR}/Source/PhysCollide.o \
	${OBJECTDIR}/Source/PhysMove.o \
	${OBJECTDIR}/Source/Render.o \
	${OBJECTDIR}/Source/RenderOld.o \
	${OBJECTDIR}/Source/ShaderRepository.o \
	${OBJECTDIR}/Source/System.o \
	${OBJECTDIR}/Source/UserControl.o \
	${OBJECTDIR}/Source/lighting_technique.o \
	${OBJECTDIR}/Source/loadShaders.o \
	${OBJECTDIR}/Source/main.o \
	${OBJECTDIR}/Source/mesh.o \
	${OBJECTDIR}/Source/technique.o \
	${OBJECTDIR}/Source/texture.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 `pkg-config --cflags glew ImageMagick++ assimp freetype2` 
CXXFLAGS=-std=c++11 `pkg-config --cflags glew ImageMagick++ assimp freetype2` 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dualityengine

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dualityengine: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dualityengine ${OBJECTFILES} ${LDLIBSOPTIONS} `pkg-config --libs sdl2 gl glew ImageMagick++ assimp freetype2 fontconfig `

${OBJECTDIR}/Source/Box.o: Source/Box.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/Box.o Source/Box.cpp

${OBJECTDIR}/Source/ComponentBank.o: Source/ComponentBank.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/ComponentBank.o Source/ComponentBank.cpp

${OBJECTDIR}/Source/Engine.o: Source/Engine.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/Engine.o Source/Engine.cpp

${OBJECTDIR}/Source/FileIO.o: Source/FileIO.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/FileIO.o Source/FileIO.cpp

${OBJECTDIR}/Source/Game.o: Source/Game.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/Game.o Source/Game.cpp

${OBJECTDIR}/Source/PhysCollide.o: Source/PhysCollide.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/PhysCollide.o Source/PhysCollide.cpp

${OBJECTDIR}/Source/PhysMove.o: Source/PhysMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/PhysMove.o Source/PhysMove.cpp

${OBJECTDIR}/Source/Render.o: Source/Render.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/Render.o Source/Render.cpp

${OBJECTDIR}/Source/RenderOld.o: Source/RenderOld.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/RenderOld.o Source/RenderOld.cpp

${OBJECTDIR}/Source/ShaderRepository.o: Source/ShaderRepository.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/ShaderRepository.o Source/ShaderRepository.cpp

${OBJECTDIR}/Source/System.o: Source/System.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/System.o Source/System.cpp

${OBJECTDIR}/Source/UserControl.o: Source/UserControl.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/UserControl.o Source/UserControl.cpp

${OBJECTDIR}/Source/lighting_technique.o: Source/lighting_technique.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/lighting_technique.o Source/lighting_technique.cpp

${OBJECTDIR}/Source/loadShaders.o: Source/loadShaders.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/loadShaders.o Source/loadShaders.cpp

${OBJECTDIR}/Source/main.o: Source/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/main.o Source/main.cpp

${OBJECTDIR}/Source/mesh.o: Source/mesh.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/mesh.o Source/mesh.cpp

${OBJECTDIR}/Source/technique.o: Source/technique.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/technique.o Source/technique.cpp

${OBJECTDIR}/Source/texture.o: Source/texture.cpp 
	${MKDIR} -p ${OBJECTDIR}/Source
	${RM} "$@.d"
	$(COMPILE.cc) -g -IHeaders -ISource -IAssets/Shaders -I/usr/include/SDL2 -I/usr/local/include/SDL2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Source/texture.o Source/texture.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dualityengine

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
