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

#include <unordered_map>
#include <cstdint>
#include <sstream>
#include <iostream>

#include "BankDelegates.h"
#include "Settings.h"
#include "Soul.h"
#include "Model.h"
#include "Control.h"
#include "Position.h"
#include "PositionChild.h"
#include "PositionParent.h"
#include "PositionVeloc.h"
#include "Orientation.h"
#include "RotationVeloc.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Owner.h"
#include "Score.h"
#include "Collision.h"
#include "CameraFree.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace DualityEngine {

    class ComponentBank
    {
        DUA_id nextID;
        
        std::unordered_map<DUA_id, Soul>              components_soul;
        std::unordered_map<DUA_id, Model>             components_model;
        std::unordered_map<DUA_id, Position>          components_position;
        std::unordered_map<DUA_id, SpatialChild>      components_spatialChild;
        std::unordered_map<DUA_id, SpatialParent>     components_spatialParent;
        std::unordered_map<DUA_id, LinearVelocity>    components_linearVeloc;
        std::unordered_map<DUA_id, Collision>         components_collision;
        std::unordered_map<DUA_id, Orientation>       components_orientation;
        std::unordered_map<DUA_id, AngularVelocity>   components_angularVeloc;
        std::unordered_map<DUA_id, Control>           components_control;
        std::unordered_map<DUA_id, PointLight>        components_pointLight;
        std::unordered_map<DUA_id, DirectionalLight>  components_directionalLight;
        std::unordered_map<DUA_id, AmbientLight>      components_ambientLight;
        std::unordered_map<DUA_id, Owner>             components_owner;
        std::unordered_map<DUA_id, Score>             components_score;
        std::unordered_map<DUA_id, CameraFree>        components_freeCam;
        
        
        
        /* COMPONENT POINTER GETTERS - I KNOW THESE ARE A BAD IDEA... */
        template<class componentType>
        componentType* getComponentPtr(const DUA_id&, const char*, std::unordered_map<DUA_id, componentType>&);
        
        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const DUA_compFlag &flag,const DUA_id &ID);
        template<class componentType, typename ... types>
        bool tryAddComponent(const DUA_id &ID, const char* compName, std::unordered_map<DUA_id, componentType> &table, const types& ... args);
        bool addSoul(const DUA_id &ID, const char* name);
        
        /* COMPONENT DELETION */
        void tryRemoveFlagFromSoul(const DUA_compFlag &flag, const DUA_id &ID);
        template<class componentType>
        bool tryRemoveComponent(const DUA_id &ID, const char* compName, const DUA_compFlag& compFlag, std::unordered_map<DUA_id, componentType> &table);
        bool deleteSoul(const DUA_id &ID);
        
        /* ENTITY CREATION */
        DUA_id generateID();

    public:
        
        /* BANK MANAGEMENT */
        BankDelegates* dlgt;
        
        #define DUA_COMP_TUPLE std::tuple<std::string, std::string, DUA_compFlag>
        #define DUA_COMPCOLL(x, y) std::get<y>(bank->componentCollections[x])
        const std::array<const std::tuple<std::string, std::string, DUA_compFlag>, 16> componentCollections = {{
            DUA_COMP_TUPLE{"soul", "soul", 0},
            DUA_COMP_TUPLE{"model", "model", MODEL},
            DUA_COMP_TUPLE{"position", "position", POSITION},
            DUA_COMP_TUPLE{"spatial child", "spatchild", SPATCHILD},
            DUA_COMP_TUPLE{"spatial parent", "spatparent", SPATPARENT},
            DUA_COMP_TUPLE{"linear velocity", "linveloc", LINVELOC},
            DUA_COMP_TUPLE{"collision", "collision", COLLISION},
            DUA_COMP_TUPLE{"orientation", "orientation", ORIENTATION},
            DUA_COMP_TUPLE{"angular velocity", "angveloc", ANGVELOC},
            DUA_COMP_TUPLE{"control", "control", CONTROL},
            DUA_COMP_TUPLE{"point light", "lpoint", LPOINT},
            DUA_COMP_TUPLE{"directional light", "ldirect", LDIRECT},
            DUA_COMP_TUPLE{"ambient light", "lambient", LAMBIENT},
            DUA_COMP_TUPLE{"owner", "owner", OWNER},
            DUA_COMP_TUPLE{"score", "score", SCORE},
            DUA_COMP_TUPLE{"free camera", "freecam", FREECAM}
        }};
        
        
        DUA_id activeControlID = DUA_NULL_ID;
        bool switchToControl(const DUA_id &id);
        
        DUA_id activeCameraID = DUA_NULL_ID;
        bool switchToCam(const DUA_id &id);
        
        glm::mat4 getPosMat(const DUA_id&);
        glm::mat4 getRotMat(const DUA_id&);
        glm::mat4 getModMat(const DUA_id&);
        
        
        // Constructor for new states
        ComponentBank(BankDelegates* dlgt);
        // Destructor
        ~ComponentBank();
        
        /* BANK MANAGEMENT */
        void clean();
        void save(const char* saveName);
        void load(const char* saveName);

//        void* tempCompPtr = NULL;
//        const Position zeroPosition = Position(0, 0, 0);
//        const LinearVelocity zeroLinVeloc = LinearVelocity(0, 0, 0);
//        const Orientation zeroOrientation = Orientation(0, 0, 0);
//        const AngularVelocity zeroAngVeloc = AngularVelocity(0, 0, 0);
        
        /* COMPONENT POINTER GETTERS - I KNOW THESE ARE A BAD IDEA... */
        Model* getModelPtr(const DUA_id &ID);
        LinearVelocity* getLinearVelocPtr(const DUA_id &ID);
        Position* getPositionPtr(const DUA_id &ID);
        Orientation* getOrientationPtr(const DUA_id &ID);
        AngularVelocity* getAngularVelocPtr(const DUA_id &ID);
        SpatialChild* getSpatialChildPtr(const DUA_id &ID);
        SpatialParent* getSpatialParentPtr(const DUA_id &ID);
        Control* getControlPtr(const DUA_id &ID);
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
        void addControl(const DUA_id &ID);
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
        void deleteControl(const DUA_id &ID);
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
        std::string listComponents(const DUA_id &ID);
    };

}

#endif	/* COMPONENTSTORE_H */

