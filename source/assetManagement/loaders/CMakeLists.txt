
# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/loadCubeMap.cpp"        "${DIR}/loadCubeMap.h"
                                        "${DIR}/loadMesh.h"
        "${DIR}/loadShaders.cpp"        "${DIR}/loadShaders.h"
                                        "${DIR}/loadTexture.h"
                                        "${DIR}/stb_image.h"
        PARENT_SCOPE
        )
