
# Galen Cochrane
# Copyright (C) 2016
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/Bank.cpp"   "${DIR}/Bank.h"
                            "${DIR}/BankDelegates.h"
                            "${DIR}/controlTypes.h"
                            "${DIR}/KvMap.h"
        PARENT_SCOPE
        )
