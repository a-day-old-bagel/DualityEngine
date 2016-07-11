
# Galen Cochrane
# Copyright (C) 2016
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

add_subdirectory(programmaticAssets)

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/fileIO.cpp"             "${DIR}/fileIO.h"
        "${DIR}/loadCubeMap.cpp"        "${DIR}/loadCubeMap.h"
                                        "${DIR}/loadMesh.h"
        "${DIR}/loadShaders.cpp"        "${DIR}/loadShaders.h"
                                        "${DIR}/loadTexture.h"
        "${DIR}/MeshRepository.cpp"     "${DIR}/MeshRepository.h"
        "${DIR}/ShaderRepository.cpp"   "${DIR}/ShaderRepository.h"
                                        "${DIR}/SoundRepository.h"
                                        "${DIR}/stb_image.h"
                                        "${DIR}/TextureRepository"
        PARENT_SCOPE
        )
