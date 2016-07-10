
# Galen Cochrane
# Copyright (C) 2016
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/Control_SS.cpp"         "${DIR}/Control_SS.h"
        "${DIR}/PhysCollide.cpp"        "${DIR}/PhysCollide.h"
        "${DIR}/PhysMove.cpp"           "${DIR}/PhysMove.h"
        "${DIR}/Render_Background.cpp"  "${DIR}/Render_Background.h"
        "${DIR}/Render_BiTcubes.cpp"    "${DIR}/Render_BiTcubes.h"
        "${DIR}/Render_Console.cpp"     "${DIR}/Render_Console.h"
        "${DIR}/Render_Master.cpp"      "${DIR}/Render_Master.h"
        "${DIR}/Render_Models.cpp"      "${DIR}/Render_Models.h"
        "${DIR}/Scripting.cpp"          "${DIR}/Scripting.h"
                                        "${DIR}/System.h"
        "${DIR}/UserInput.cpp"          "${DIR}/UserInput.h"
        PARENT_SCOPE
        )
