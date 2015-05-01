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

#include "Settings.h"
#include "Soul.h"
#include "Model.h"
#include "Control.h"
#include "Motion.h"
#include "Spatial.h"
#include "SpatialChild.h"
#include "SpatialParent.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"
#include "Owner.h"
#include "Score.h"

namespace DualityEngine {

    class ComponentBank
    {
        IDNUM nextID;

        std::unordered_map<IDNUM, Soul>              components_soul;
        std::unordered_map<IDNUM, Model>             components_model;
        std::unordered_map<IDNUM, Motion>            components_motion;
        std::unordered_map<IDNUM, Spatial>           components_spatial;
        std::unordered_map<IDNUM, SpatialChild>      components_spatialChild;
        std::unordered_map<IDNUM, SpatialParent>     components_spatialParent;
        std::unordered_map<IDNUM, Control>           components_control;
        std::unordered_map<IDNUM, PointLight>        components_pointLight;
        std::unordered_map<IDNUM, DirectionalLight>  components_directionalLight;
        std::unordered_map<IDNUM, AmbientLight>      components_ambientLight;
        std::unordered_map<IDNUM, Owner>             components_owner;
        std::unordered_map<IDNUM, Score>             components_score;
        
        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const COMPFLAG &flag,const IDNUM &ID);
        template<class componentType, typename ... types>
        bool tryAddComponent(const IDNUM &ID, const char* compName, std::unordered_map<IDNUM, componentType> &table, const types& ... args);
        bool addSoul(const IDNUM &ID, const char* name);
        
        /* COMPONENT DELETION */
        void tryRemoveFlagFromSoul(const COMPFLAG &flag, const IDNUM &ID);
        template<class componentType>
        bool tryRemoveComponent(const IDNUM &ID, const char* compName, std::unordered_map<IDNUM, componentType> &table);
        bool deleteSoul(const IDNUM &ID);
        
        /* ENTITY CREATION */
        IDNUM generateID();

    public:
        // Constructor for new states
        ComponentBank();
        // Constructor for loading saved states
        ComponentBank(const IDNUM &startingID);
        // Destructor
        ~ComponentBank();

        /* COMPONENT POINTER GETTERS - I KNOW THESE ARE A BAD IDEA... */
        Soul* getSoulPtr(const IDNUM &ID);
        Model* getModelPtr(const IDNUM &ID);
        Motion* getMotionPtr(const IDNUM &ID);
        Spatial* getSpatialPtr(const IDNUM &ID);
        SpatialChild* getSpatialChildPtr(const IDNUM &ID);
        SpatialParent* getSpatialParentPtr(const IDNUM &ID);
        Control* getControlPtr(const IDNUM &ID);
        PointLight* getPointLightPtr(const IDNUM &ID);
        DirectionalLight* getDirectionalLightPtr(const IDNUM &ID);
        AmbientLight* getAmbientLightPtr(const IDNUM &ID);
        Owner* getOwnerPtr(const IDNUM &ID);
        Score* getScorePtr(const IDNUM &ID);

        /* COMPONENT CREATION */
        void addModel(const IDNUM &ID);
        void addMotion(const IDNUM &ID, const FLOAT &velX, const FLOAT &velY, const FLOAT &velZ,
                                           const FLOAT &anvX, const FLOAT &anvY, const FLOAT &anvZ);
        void addSpatial(const IDNUM &ID, const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ,
                                            const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ);
        void addSpatialChild(const IDNUM &ID, const IDNUM &refID);
        void addSpatialParent(const IDNUM &ID, const IDNUM &refID);
        void addControl(const IDNUM &ID);
        void addPointLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue,
                                  const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ);
        void addDirectionalLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue,
                                        const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ);
        void addAmbientLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue);
        void addOwner(const IDNUM &ID, const IDNUM &refID);
        void addScore(const IDNUM &ID);

        /* COMPONENT DELETION */
        void deleteModel(const IDNUM &ID);
        void deleteMotion(const IDNUM &ID);
        void deleteSpatial(const IDNUM &ID);
        void deleteSpatialChild(const IDNUM &ID);
        void deleteSpatialParent(const IDNUM &ID);
        void deleteControl(const IDNUM &ID);
        void deletePointLight(const IDNUM &ID);
        void deleteDirectionalLight(const IDNUM &ID);
        void deleteAmbientLight(const IDNUM &ID);
        void deleteOwner(const IDNUM &ID);
        void deleteScore(const IDNUM &ID);

        /* ENTITY CREATION */
        IDNUM createEntity(const char* name);
        bool  notifySystemsOfAdditions(const IDNUM &ID);
        IDNUM createBox(const char* name,
                           const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ,
                           const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ,
                           const FLOAT &velX, const FLOAT &velY, const FLOAT &velZ,
                           const FLOAT &anvX, const FLOAT &anvY, const FLOAT &anvZ);
        
        /* ENTITY DELETION */
        bool deleteEntity(const IDNUM &ID);  // orphans children
        bool purgeEntity(const IDNUM &ID);   // deletes children too.
        bool notifySystemsOfDeletions(const IDNUM &ID);

        /* CONVENIENCE GETTERS */
        std::string getName(IDNUM &ID);
        std::string listComponents(IDNUM &ID);
    };

}

#endif	/* COMPONENTSTORE_H */

