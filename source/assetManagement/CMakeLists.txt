
# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

add_subdirectory( loaders )
add_subdirectory( programmaticAssets )
add_subdirectory( repos )
add_subdirectory( savers )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/fileIO.cpp"             "${DIR}/fileIO.h"
        PARENT_SCOPE
        )
