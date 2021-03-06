#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
#
# The user may set the environment variable GLEW_ROOT_DIR to the root of a GLEW installation

IF (WIN32)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		$ENV{PROGRAMFILES}/GLEW/include
		$ENV{GLEW_ROOT_DIR}/include
		${GLEW_ROOT_DIR}/include
		DOC "The directory where GL/glew.h resides")

    FIND_LIBRARY( GLEW_LIBRARY
        NAMES glew GLEW glew32 glew32s
        PATHS
        $ENV{PROGRAMFILES}/GLEW/lib
				$ENV{GLEW_ROOT_DIR}/lib/Release/x64
        ${GLEW_ROOT_DIR}/lib
        ${PROJECT_SOURCE_DIR}/src/nvgl/glew/bin
        ${PROJECT_SOURCE_DIR}/src/nvgl/glew/lib
        DOC "The GLEW library")
ELSE (WIN32)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		${GLEW_ROOT_DIR}/include
		DOC "The directory where GL/glew.h resides")

	# Prefer the static library.
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES libGLEW.a GLEW
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		${GLEW_ROOT_DIR}/lib
		DOC "The GLEW library")
ENDIF (WIN32)

IF (GLEW_INCLUDE_PATH)
	MESSAGE(STATUS "Found GLEW include path: ${GLEW_INCLUDE_PATH}")
ENDIF(GLEW_INCLUDE_PATH)

IF (GLEW_LIBRARY)
	MESSAGE(STATUS "Found GLEW lib path: ${GLEW_LIBRARY}")
ENDIF(GLEW_LIBRARY)

SET(GLEW_FOUND "NO")
IF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)
	SET(GLEW_LIBRARIES ${GLEW_LIBRARY})
	SET(GLEW_FOUND "YES")
ENDIF (GLEW_INCLUDE_PATH AND GLEW_LIBRARY)

INCLUDE(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLEW DEFAULT_MSG GLEW_LIBRARY)
