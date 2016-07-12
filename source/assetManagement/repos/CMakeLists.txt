
# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/FontRepo.cpp"           "${DIR}/FontRepo.h"
        "${DIR}/MeshRepo.cpp"           "${DIR}/MeshRepo.h"
        "${DIR}/ShaderRepo.cpp"         "${DIR}/ShaderRepo.h"
        "${DIR}/SoundRepo.cpp"          "${DIR}/SoundRepo.h"
        "${DIR}/TextureRepo.cpp"        "${DIR}/TextureRepo.h"
        PARENT_SCOPE
        )
