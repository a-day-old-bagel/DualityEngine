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

namespace DualityEngine {

    class ComponentBank
    {
        DUA_ID nextID;

        std::unordered_map<DUA_ID, Soul>              components_soul;
        std::unordered_map<DUA_ID, Model>             components_model;
        std::unordered_map<DUA_ID, Position>          components_position;
        std::unordered_map<DUA_ID, PositionChild>     components_positionChild;
        std::unordered_map<DUA_ID, PositionParent>    components_positionParent;
        std::unordered_map<DUA_ID, PositionVeloc>     components_positionVeloc;
        std::unordered_map<DUA_ID, Rotation>          components_rotation;
        std::unordered_map<DUA_ID, RotationVeloc>     components_rotationVeloc;
        std::unordered_map<DUA_ID, Control>           components_control;
        std::unordered_map<DUA_ID, PointLight>        components_pointLight;
        std::unordered_map<DUA_ID, DirectionalLight>  components_directionalLight;
        std::unordered_map<DUA_ID, AmbientLight>      components_ambientLight;
        std::unordered_map<DUA_ID, Owner>             components_owner;
        std::unordered_map<DUA_ID, Score>             components_score;
        
        /* BANK MANAGEMENT */
        BankDelegates* dlgt;
        
        /* COMPONENT POINTER GETTERS - I KNOW THESE ARE A BAD IDEA... */
        template<class componentType>
        componentType* getComponentPtr(const DUA_ID&, const char*, std::unordered_map<DUA_ID, componentType>&);
        
        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const DUA_COMPFLAG &flag,const DUA_ID &ID);
        template<class componentType, typename ... types>
        bool tryAddComponent(const DUA_ID &ID, const char* compName, std::unordered_map<DUA_ID, componentType> &table, const types& ... args);
        bool addSoul(const DUA_ID &ID, const char* name);
        
        /* COMPONENT DELETION */
        void tryRemoveFlagFromSoul(const DUA_COMPFLAG &flag, const DUA_ID &ID);
        template<class componentType>
        bool tryRemoveComponent(const DUA_ID &ID, const char* compName, std::unordered_map<DUA_ID, componentType> &table);
        bool deleteSoul(const DUA_ID &ID);
        
        /* ENTITY CREATION */
        DUA_ID generateID();

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
        Model* getModelPtr(const DUA_ID &ID);
        PositionVeloc* getPositionVelocPtr(const DUA_ID &ID);
        Position* getPositionPtr(const DUA_ID &ID);
        Rotation* getRotationPtr(const DUA_ID &ID);
        RotationVeloc* getRotationVelocPtr(const DUA_ID &ID);
        PositionChild* getPositionChildPtr(const DUA_ID &ID);
        PositionParent* getPositionParentPtr(const DUA_ID &ID);
        Control* getControlPtr(const DUA_ID &ID);
        PointLight* getPointLightPtr(const DUA_ID &ID);
        DirectionalLight* getDirectionalLightPtr(const DUA_ID &ID);
        AmbientLight* getAmbientLightPtr(const DUA_ID &ID);
        Owner* getOwnerPtr(const DUA_ID &ID);
        Score* getScorePtr(const DUA_ID &ID);
        
        /* COMPONENT CREATION */
        void addModel(const DUA_ID &ID);
        void addPositionVeloc(const DUA_ID &ID, const DUA_DBL &velX, const DUA_DBL &velY, const DUA_DBL &velZ);
        void addPosition(const DUA_ID &ID, const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ);
        void addRotation(const DUA_ID &ID, const DUA_DBL &rotX, const DUA_DBL &rotY, const DUA_DBL &rotZ);
        void addRotationVeloc(const DUA_ID &ID, const DUA_DBL &angX, const DUA_DBL &angY, const DUA_DBL &angZ);
        void addPositionChild(const DUA_ID &ID, const DUA_ID &refID);
        void addPositionParent(const DUA_ID &ID, const DUA_ID &refID);
        void addControl(const DUA_ID &ID);
        void addPointLight(const DUA_ID &ID, const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue,
                                  const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ);
        void addDirectionalLight(const DUA_ID &ID, const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue,
                                        const DUA_DBL &rotX, const DUA_DBL &rotY, const DUA_DBL &rotZ);
        void addAmbientLight(const DUA_ID &ID, const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue);
        void addOwner(const DUA_ID &ID, const DUA_ID &refID);
        void addScore(const DUA_ID &ID);

        /* COMPONENT DELETION */
        void deleteModel(const DUA_ID &ID);
        void deletePositionVeloc(const DUA_ID &ID);
        void deletePosition(const DUA_ID &ID);
        void deleteRotation(const DUA_ID &ID);
        void deleteRotationVeloc(const DUA_ID &ID);
        void deletePositionChild(const DUA_ID &ID);
        void deletePositionParent(const DUA_ID &ID);
        void deleteControl(const DUA_ID &ID);
        void deletePointLight(const DUA_ID &ID);
        void deleteDirectionalLight(const DUA_ID &ID);
        void deleteAmbientLight(const DUA_ID &ID);
        void deleteOwner(const DUA_ID &ID);
        void deleteScore(const DUA_ID &ID);
        
        /* ENTITY STATE GETTERS */
        DUA_COMPFLAG getComponents(const DUA_ID& ID);
        DUA_STATEFLAG getState(const DUA_ID& ID);

        /* ENTITY CREATION */
        DUA_ID createEntity(const char* name);
        DUA_ID createBox(const char* name,
                        const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ,
                        const DUA_DBL &rotX, const DUA_DBL &rotY, const DUA_DBL &rotZ,   
                        const DUA_DBL &velX, const DUA_DBL &velY, const DUA_DBL &velZ,
                        const DUA_DBL &angX, const DUA_DBL &angY, const DUA_DBL &angZ);
        
        /* ENTITY DELETION */
        bool deleteEntity(const DUA_ID &ID);  // orphans children
        bool purgeEntity(const DUA_ID &ID);   // deletes children too.

        /* CONVENIENCE GETTERS */
        std::string getName(DUA_ID &ID);
        std::string listComponents(DUA_ID &ID);
    };

}

#endif	/* COMPONENTSTORE_H */

