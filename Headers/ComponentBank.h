/* 
 * File:   ComponentBank.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 5:19 PM
 */

#ifndef COMPONENTBANK_H
#define	COMPONENTBANK_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <cstdint>
#include <sstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

// It's pointless to do separate compilation on HashMap at this point, but I do it
// anyway for style consistency
#include "HashMap.h"
#include "../Source/HashMap.cpp"

#include "BankDelegates.h"
#include "Settings.h"

#include "Soul.h"
#include "Model.h"
#include "ControlBase.h"
#include "SpaceControl.h"
#include "Position.h"
#include "PositionChild.h"
#include "PositionParent.h"
#include "LinearVelocity.h"
#include "Orientation.h"
#include "AngularVelocity.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Owner.h"
#include "Score.h"
#include "Collision.h"
#include "CameraFree.h"

namespace DualityEngine {

    class ComponentBank
    {
        DUA_id nextID;
        
        HashMap<DUA_id, Soul>              components_soul;
        HashMap<DUA_id, Model>             components_model;
        HashMap<DUA_id, Position>          components_position;
        HashMap<DUA_id, SpatialChild>      components_spatialChild;
        HashMap<DUA_id, SpatialParent>     components_spatialParent;
        HashMap<DUA_id, LinearVelocity>    components_linearVeloc;
        HashMap<DUA_id, Collision>         components_collision;
        HashMap<DUA_id, Orientation>       components_orientation;
        HashMap<DUA_id, AngularVelocity>   components_angularVeloc;
        HashMap<DUA_id, SpaceControl>      components_spacecontrol;
        HashMap<DUA_id, PointLight>        components_pointLight;
        HashMap<DUA_id, DirectionalLight>  components_directionalLight;
        HashMap<DUA_id, AmbientLight>      components_ambientLight;
        HashMap<DUA_id, Owner>             components_owner;
        HashMap<DUA_id, Score>             components_score;
        HashMap<DUA_id, CameraFree>        components_freeCam;       
        
        /* COMPONENT POINTER GETTERS - A NECESSARY EVIL (SO SAYETH THE OPTIMIZER) */
        template<class componentType>
        componentType* getComponentPtr(const DUA_id&, const char*, HashMap<DUA_id, componentType>&);
        
        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const DUA_compFlag &flag, const DUA_id &ID);
        template<class componentType, typename ... types>
        bool tryAddComponent(const DUA_id &ID, const char* compName, HashMap<DUA_id, componentType> &, const types& ... args);
        bool addSoul(const DUA_id &ID, const char* name);
        
        /* COMPONENT DELETION */
        void tryRemoveFlagFromSoul(const DUA_compFlag &flag, const DUA_id &ID);
        template<class componentType>
        bool tryRemoveComponent(const DUA_id &ID, const char* compName, const DUA_compFlag& compFlag, HashMap<DUA_id, componentType> &table);
        bool deleteSoul(const DUA_id &ID);
        
        /* ENTITY CREATION */
        DUA_id generateID();

    public:        
        
        // Constructor for new states
        ComponentBank(BankDelegates* dlgt);
        // Destructor
        ~ComponentBank();
        
        /* BANK MANAGEMENT */
        void clean();
        void save(const char* saveName);
        void load(const char* saveName);
        
        /* COMPONENT POINTER GETTERS - I KNOW THESE ARE A BAD IDEA... */
        Model* getModelPtr(const DUA_id &ID);
        LinearVelocity* getLinearVelocPtr(const DUA_id &ID);
        Position* getPositionPtr(const DUA_id &ID);
        Orientation* getOrientationPtr(const DUA_id &ID);
        AngularVelocity* getAngularVelocPtr(const DUA_id &ID);
        SpatialChild* getSpatialChildPtr(const DUA_id &ID);
        SpatialParent* getSpatialParentPtr(const DUA_id &ID);
        SpaceControl* getSpaceControlPtr(const DUA_id &ID);
        PointLight* getPointLightPtr(const DUA_id &ID);
        DirectionalLight* getDirectionalLightPtr(const DUA_id &ID);
        AmbientLight* getAmbientLightPtr(const DUA_id &ID);
        Owner* getOwnerPtr(const DUA_id &ID);
        Score* getScorePtr(const DUA_id &ID);
        Collision* getCollisionPtr(const DUA_id &ID);
        CameraFree* getCameraFreePtr(const DUA_id &ID);
        
        /* COMPONENT CREATION */
//        template<typename ... types>
//        bool addComponent(const DUA_id &ID, const char* compType, const types& ... args);
        
        void addModel(const DUA_id &ID, const char* fileName);
        void addLinearVeloc(const DUA_id &ID, const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ);
        void addPosition(const DUA_id &ID, const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ);
        void addOrientation(const DUA_id &ID, const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ);
        void addAngularVeloc(const DUA_id &ID, const DUA_dbl &angX, const DUA_dbl &angY, const DUA_dbl &angZ);
        void addSpatialChild(const DUA_id &ID, const DUA_id &refID);
        void addSpatialParent(const DUA_id &ID, const DUA_id &refID);
        void addSpaceControl(const DUA_id &ID, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&, const DUA_dbl&);
        void addPointLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue,
                                  const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ);
        void addDirectionalLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue,
                                        const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ);
        void addAmbientLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue);
        void addOwner(const DUA_id &ID, const DUA_id &refID);
        void addScore(const DUA_id &ID);
        void addCollision(const DUA_id &ID);
        void addCameraFree(const DUA_id &ID, DUA_float fov, DUA_float zNear, DUA_float zFar, DUA_dbl eyeX, DUA_dbl eyeY, DUA_dbl eyeZ, DUA_dbl focusX, DUA_dbl focusY, DUA_dbl focusZ, DUA_dbl upX, DUA_dbl upY, DUA_dbl upZ);

        /* COMPONENT DELETION */
        void deleteModel(const DUA_id &ID);
        void deleteLinearVeloc(const DUA_id &ID);
        void deletePosition(const DUA_id &ID);
        void deleteOrientation(const DUA_id &ID);
        void deleteAngularVeloc(const DUA_id &ID);
        void deleteSpatialChild(const DUA_id &ID);
        void deleteSpatialParent(const DUA_id &ID);
        void deleteSpaceControl(const DUA_id &ID);
        void deletePointLight(const DUA_id &ID);
        void deleteDirectionalLight(const DUA_id &ID);
        void deleteAmbientLight(const DUA_id &ID);
        void deleteOwner(const DUA_id &ID);
        void deleteScore(const DUA_id &ID);
        void deleteCollision(const DUA_id &ID);
        void deleteCameraFree(const DUA_id &ID);
        
        /* ENTITY STATE GETTERS / SETTERS */
        DUA_compFlag getComponents(const DUA_id& ID);
        DUA_stateFlag getState(const DUA_id& ID);
        void stateOn(const DUA_id& ID, const DUA_stateFlag& flag);
        void stateOff(const DUA_id& ID, const DUA_stateFlag& flag);

        /* ENTITY CREATION */
        DUA_id createEntity(const char* name);
        DUA_id createBox(const char* name,
                        const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ,
                        const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ,   
                        const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ,
                        const DUA_dbl &angX, const DUA_dbl &angY, const DUA_dbl &angZ);
        
        /* ENTITY DELETION */
        bool deleteEntity(const DUA_id &ID);  // orphans children
        bool purgeEntity(const DUA_id &ID);   // deletes children too.

        /* CONVENIENCE GETTERS */
        bool getIDs(std::string& name, std::vector<DUA_id>& IDs);
        std::string getName(const DUA_id &ID);
        std::string listComponentsVerbose(const DUA_id &ID);
        std::string listComponents(const DUA_compFlag& flag);
        
        
        
        /*****************************
         * HEREAFTER FOLLOW VARIOUS HELPFUL CONSTRUCTS NOT FUNDAMENTAL TO COMPONENT BANK.
         * MOST OF THEM ARE HERE TO BE SHARED BY ALL SYSTEMS, AND ARE THINGS UNSUITED FOR
         * SETTINGS.H
         *****************************/
        
        /* PUBLIC DELEGATES */
        BankDelegates* dlgt;        
        
        /* HANDY-DANDY COMPONENT DICTIONARY */        
        // this is a wierd multi-type dictionary thing to hold associations between
        // (full name [index 0], abbreviated/command name [index 1], and enumerator [index 2])
        // for any given type of component. Used predominantly in scripting system.
        // It makes it much easier to change these values globally.
        
        // this is what you use to access an element from a system. x for component type, y for which attribute
        #define DUA_COMPCOLL(x, y) std::get<y>(bank->componentCollections[x])
        // unfortunately a typedef won't do what I want here...
        #define DUA_COMP_TUPLE std::tuple<std::string, std::string, DUA_compFlag>
        
        const std::array<const DUA_COMP_TUPLE, 16> componentCollections = {{
            DUA_COMP_TUPLE{"soul", "soul", 0},
            DUA_COMP_TUPLE{"model", "model", MODEL},
            DUA_COMP_TUPLE{"position", "position", POSITION},
            DUA_COMP_TUPLE{"spatial child", "spatchild", SPATCHILD},
            DUA_COMP_TUPLE{"spatial parent", "spatparent", SPATPARENT},
            DUA_COMP_TUPLE{"linear velocity", "linveloc", LINVELOC},
            DUA_COMP_TUPLE{"collision", "collision", COLLISION},
            DUA_COMP_TUPLE{"orientation", "orientation", ORIENTATION},
            DUA_COMP_TUPLE{"angular velocity", "angveloc", ANGVELOC},
            DUA_COMP_TUPLE{"space-y control", "spacecontrol", CONTROLSS},
            DUA_COMP_TUPLE{"point light", "lpoint", LPOINT},
            DUA_COMP_TUPLE{"directional light", "ldirect", LDIRECT},
            DUA_COMP_TUPLE{"ambient light", "lambient", LAMBIENT},
            DUA_COMP_TUPLE{"owner", "owner", OWNER},
            DUA_COMP_TUPLE{"score", "score", SCORE},
            DUA_COMP_TUPLE{"free camera", "freecam", FREECAM}
        }};
        
        
        /* CONTROL STUFF */
        
        bool switchToControl(const DUA_id &id, ControlTypes::type);
        void scrutinizeControl(const DUA_id &id, ControlTypes::type);
        DUA_id activeControlID = DUA_NULL_ID;     
        LinearVelocity* pCtrlLinVelocCurrent;
        LinearVelocity* pCtrlLinVelocDummy;
        Orientation* pCtrlOrientCurrent;       
        Orientation* pCtrlOrientDummy;        
        Delegate<void()> defocusControl;
        DUA_compFlag requiredControlComponents = 0;
        ControlTypes::type currentControlType = ControlTypes::NONE;
        
        void assignSpaceControl(const DUA_id& ID);        
        void defocusSpaceControl();        
        SpaceControl* pSpaceControlCurrent;   
        SpaceControl* pSpaceControlDummy;
        
        
        /* CAM STUFF */
        
        DUA_id activeFreeCameraID = DUA_NULL_ID;
        CameraFree* pFreeCameraCurrent;
        CameraFree* pFreeCameraDummy;
        bool updateActiveCamera();
        bool switchToCam(const DUA_id &id);
        void scrutinizeCam(const DUA_id &id);
        void defaultCam();
        
        
        /* TRANSFORM MATRIX GETTERS */
        
        glm::mat4 getPosMat(const DUA_id&);     // translation matrix
        glm::mat4 getRotMat(const DUA_id&);     // rotation matrix
        glm::mat4 getModMat(const DUA_id&);     // full model matrix (combines above two)
    };

}

#endif	/* COMPONENTSTORE_H */

