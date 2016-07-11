
# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
                                    "${DIR}/concurrentqueue.h"
        "${DIR}/Console.cpp"        "${DIR}/Console.h"
                                    "${DIR}/Delegate.h"
                                    "${DIR}/Engine.h"
        "${DIR}/Event.cpp"          "${DIR}/Event.h"
        "${DIR}/helpDocs.cpp"       "${DIR}/helpDocs.h"
                                    "${DIR}/readerwriterqueue.h"
        "${DIR}/settings.cpp"       "${DIR}/settings.h"
        "${DIR}/TimePiece.cpp"      "${DIR}/TimePiece.h"
        "${DIR}/toString.cpp"       "${DIR}/toString.h"
        PARENT_SCOPE
        )
