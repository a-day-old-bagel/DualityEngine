
# Galen Cochrane
# Copyright (C) 2016
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

add_subdirectory( assetManagement )
add_subdirectory( bank )
add_subdirectory( components )
add_subdirectory( entities )
add_subdirectory( math )
add_subdirectory( systems )
add_subdirectory( utilities )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/Game.cpp" "${DIR}/Game.h"
        PARENT_SCOPE
        )
