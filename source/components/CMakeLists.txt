
# Galen Cochrane
# Copyright (C) 2016
# This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)

# set DIR to point to current directory (relative to top level)
set( DIR ${CMAKE_CURRENT_LIST_DIR} )

# include the current directory
set( INCLUDE_DIRS ${INCLUDE_DIRS} ${DIR} PARENT_SCOPE )

# include all headers/source in current directory
set( SOURCE_FILES ${SOURCE_FILES}
        "${DIR}/AmbientLight.h"
        "${DIR}/AngularVelocity.h"
        "${DIR}/CameraFree.h"
        "${DIR}/Collision.h"
        "${DIR}/Component.h"
        "${DIR}/DirectionalLight.h"
        "${DIR}/LinearVelocity.h"
        "${DIR}/Model.h"
        "${DIR}/Orientation.h"
        "${DIR}/OrientationEuler.h"
        "${DIR}/Owner.h"
        "${DIR}/PointLight.h"
        "${DIR}/Position.h"
        "${DIR}/PositionChild.h"
        "${DIR}/PositionParent.h"
        "${DIR}/RotationVelocEuler.h"
        "${DIR}/Score.h"
        "${DIR}/Soul.h"
        "${DIR}/SpaceControl.h"
        PARENT_SCOPE
        )
