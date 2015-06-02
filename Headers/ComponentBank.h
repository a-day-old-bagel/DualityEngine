/* 
 * File:   ComponentBank.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 5:19 PM
 */

#ifndef COMPONENTBANK_H
#define	COMPONENTBANK_H

#include <unordered_map>
#include <cstdint>
#include <sstream>

#include "BankDelegates.h"
#include "Settings.h"
#include "Soul.h"
#include "Model.h"
#include "Control.h"
#include "Position.h"
#include "PositionChild.h"
#include "PositionParent.h"
#include "PositionVeloc.h"
#include "Rotation.h"
#include "RotationVeloc.h"
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

        std::unordered_map<DUA_id, Soul>              components_soul;
        std::unordered_map<DUA_id, Model>             components_model;
        std::unordered_map<DUA_id, Position>          components_position;
        std::unordered_map<DUA_id, PositionChild>     components_positionChild;
        std::unordered_map<DUA_id, PositionParent>    components_positionParent;
        std::unordered_map<DUA_id, PositionVeloc>     components_positionVeloc;
        std::unordered_map<DUA_id, Collision>         components_collision;
        std::unordered_map<DUA_id, Rotation>          components_rotation;
        std::unordered_map<DUA_id, RotationVeloc>     components_rotationVeloc;
        std::unordered_map<DUA_id, Control>           components_control;
        std::unordered_map<DUA_id, PointLight>        components_pointLight;
        std::unordered_map<DUA_id, DirectionalLight>  components_directionalLight;
        std::unordered_map<DUA_id, AmbientLight>      components_ambientLight;
        std::unordered_map<DUA_id, Owner>             components_owner;
        std::unordered_map<DUA_id, Score>             components_score;
        std::unordered_map<DUA_id, CameraFree>        components_freeCam;
        
//        std::unordered_map<std::string, std::pair<CompCollect, DUA_compFlag>> compCollectIndex;
        
        /* BANK MANAGEMENT */
        BankDelegates* dlgt;
        
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
        bool tryRemoveComponent(const DUA_id &ID, const char* compName, std::unordered_map<DUA_id, componentType> &table);
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
        PositionVeloc* getPositionVelocPtr(const DUA_id &ID);
        Position* getPositionPtr(const DUA_id &ID);
        Rotation* getRotationPtr(const DUA_id &ID);
        RotationVeloc* getRotationVelocPtr(const DUA_id &ID);
        PositionChild* getPositionChildPtr(const DUA_id &ID);
        PositionParent* getPositionParentPtr(const DUA_id &ID);
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
        void addPositionVeloc(const DUA_id &ID, const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ);
        void addPosition(const DUA_id &ID, const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ);
        void addRotation(const DUA_id &ID, const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ);
        void addRotationVeloc(const DUA_id &ID, const DUA_dbl &angX, const DUA_dbl &angY, const DUA_dbl &angZ);
        void addPositionChild(const DUA_id &ID, const DUA_id &refID);
        void addPositionParent(const DUA_id &ID, const DUA_id &refID);
        void addControl(const DUA_id &ID);
        void addPointLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue,
                                  const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ);
        void addDirectionalLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue,
                                        const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ);
        void addAmbientLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue);
        void addOwner(const DUA_id &ID, const DUA_id &refID);
        void addScore(const DUA_id &ID);
        void addCollision(const DUA_id &ID);
        void addCameraFree(const DUA_id &ID, DUA_float fov, DUA_float zNear, DUA_float zFar);

        /* COMPONENT DELETION */
        void deleteModel(const DUA_id &ID);
        void deletePositionVeloc(const DUA_id &ID);
        void deletePosition(const DUA_id &ID);
        void deleteRotation(const DUA_id &ID);
        void deleteRotationVeloc(const DUA_id &ID);
        void deletePositionChild(const DUA_id &ID);
        void deletePositionParent(const DUA_id &ID);
        void deleteControl(const DUA_id &ID);
        void deletePointLight(const DUA_id &ID);
        void deleteDirectionalLight(const DUA_id &ID);
        void deleteAmbientLight(const DUA_id &ID);
        void deleteOwner(const DUA_id &ID);
        void deleteScore(const DUA_id &ID);
        void deleteCollision(const DUA_id &ID);
        void deleteCameraFree(const DUA_id &ID);
        
        /* ENTITY STATE GETTERS */
        DUA_compFlag getComponents(const DUA_id& ID);
        DUA_stateFlag getState(const DUA_id& ID);

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

