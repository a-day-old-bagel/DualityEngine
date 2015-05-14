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
        DU_ID nextID;

        std::unordered_map<DU_ID, Soul>              components_soul;
        std::unordered_map<DU_ID, Model>             components_model;
        std::unordered_map<DU_ID, Position>          components_position;
        std::unordered_map<DU_ID, PositionChild>     components_positionChild;
        std::unordered_map<DU_ID, PositionParent>    components_positionParent;
        std::unordered_map<DU_ID, PositionVeloc>     components_positionVeloc;
        std::unordered_map<DU_ID, Rotation>          components_rotation;
        std::unordered_map<DU_ID, RotationVeloc>     components_rotationVeloc;
        std::unordered_map<DU_ID, Control>           components_control;
        std::unordered_map<DU_ID, PointLight>        components_pointLight;
        std::unordered_map<DU_ID, DirectionalLight>  components_directionalLight;
        std::unordered_map<DU_ID, AmbientLight>      components_ambientLight;
        std::unordered_map<DU_ID, Owner>             components_owner;
        std::unordered_map<DU_ID, Score>             components_score;
        
        /* BANK MANAGEMENT */
        BankDelegates* dlgt;
        
        /* COMPONENT POINTER GETTERS - I KNOW THESE ARE A BAD IDEA... */
        template<class componentType>
        componentType* getComponentPtr(const DU_ID&, const char*, std::unordered_map<DU_ID, componentType>&);
        
        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const DU_COMPFLAG &flag,const DU_ID &ID);
        template<class componentType, typename ... types>
        bool tryAddComponent(const DU_ID &ID, const char* compName, std::unordered_map<DU_ID, componentType> &table, const types& ... args);
        bool addSoul(const DU_ID &ID, const char* name);
        
        /* COMPONENT DELETION */
        void tryRemoveFlagFromSoul(const DU_COMPFLAG &flag, const DU_ID &ID);
        template<class componentType>
        bool tryRemoveComponent(const DU_ID &ID, const char* compName, std::unordered_map<DU_ID, componentType> &table);
        bool deleteSoul(const DU_ID &ID);
        
        /* ENTITY CREATION */
        DU_ID generateID();

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
        Soul* getSoulPtr(const DU_ID &ID);
        Model* getModelPtr(const DU_ID &ID);
        PositionVeloc* getPositionVelocPtr(const DU_ID &ID);
        Position* getPositionPtr(const DU_ID &ID);
        Rotation* getRotationPtr(const DU_ID &ID);
        RotationVeloc* getRotationVelocPtr(const DU_ID &ID);
        PositionChild* getPositionChildPtr(const DU_ID &ID);
        PositionParent* getPositionParentPtr(const DU_ID &ID);
        Control* getControlPtr(const DU_ID &ID);
        PointLight* getPointLightPtr(const DU_ID &ID);
        DirectionalLight* getDirectionalLightPtr(const DU_ID &ID);
        AmbientLight* getAmbientLightPtr(const DU_ID &ID);
        Owner* getOwnerPtr(const DU_ID &ID);
        Score* getScorePtr(const DU_ID &ID);

        /* COMPONENT CREATION */
        void addModel(const DU_ID &ID);
        void addPositionVeloc(const DU_ID &ID, const DU_FLOAT &velX, const DU_FLOAT &velY, const DU_FLOAT &velZ);
        void addPosition(const DU_ID &ID, const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ);
        void addRotation(const DU_ID &ID, const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ);
        void addRotationVeloc(const DU_ID &ID, const DU_FLOAT &angX, const DU_FLOAT &angY, const DU_FLOAT &angZ);
        void addPositionChild(const DU_ID &ID, const DU_ID &refID);
        void addPositionParent(const DU_ID &ID, const DU_ID &refID);
        void addControl(const DU_ID &ID);
        void addPointLight(const DU_ID &ID, const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue,
                                  const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ);
        void addDirectionalLight(const DU_ID &ID, const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue,
                                        const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ);
        void addAmbientLight(const DU_ID &ID, const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue);
        void addOwner(const DU_ID &ID, const DU_ID &refID);
        void addScore(const DU_ID &ID);

        /* COMPONENT DELETION */
        void deleteModel(const DU_ID &ID);
        void deletePositionVeloc(const DU_ID &ID);
        void deletePosition(const DU_ID &ID);
        void deleteRotation(const DU_ID &ID);
        void deleteRotationVeloc(const DU_ID &ID);
        void deletePositionChild(const DU_ID &ID);
        void deletePositionParent(const DU_ID &ID);
        void deleteControl(const DU_ID &ID);
        void deletePointLight(const DU_ID &ID);
        void deleteDirectionalLight(const DU_ID &ID);
        void deleteAmbientLight(const DU_ID &ID);
        void deleteOwner(const DU_ID &ID);
        void deleteScore(const DU_ID &ID);

        /* ENTITY CREATION */
        DU_ID createEntity(const char* name);
        DU_ID createBox(const char* name,
                        const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ,
                        const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ,   
                        const DU_FLOAT &velX, const DU_FLOAT &velY, const DU_FLOAT &velZ,
                        const DU_FLOAT &angX, const DU_FLOAT &angY, const DU_FLOAT &angZ);
        
        /* ENTITY DELETION */
        bool deleteEntity(const DU_ID &ID);  // orphans children
        bool purgeEntity(const DU_ID &ID);   // deletes children too.

        /* CONVENIENCE GETTERS */
        std::string getName(DU_ID &ID);
        std::string listComponents(DU_ID &ID);
    };

}

#endif	/* COMPONENTSTORE_H */

