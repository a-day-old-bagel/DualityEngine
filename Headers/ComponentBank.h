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
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

namespace DualityEngine {

    class ComponentBank
    {
        IDnumber nextID;

        std::unordered_map<IDnumber, Soul>              components_soul;
        std::unordered_map<IDnumber, Model>             components_model;
        std::unordered_map<IDnumber, Motion>            components_motion;
        std::unordered_map<IDnumber, Spatial>           components_spatial;
        std::unordered_map<IDnumber, Control>           components_control;
        std::unordered_map<IDnumber, PointLight>        components_pointLight;
        std::unordered_map<IDnumber, DirectionalLight>  components_directionalLight;
        std::unordered_map<IDnumber, AmbientLight>      components_ambientLight;
        
        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const componentFlag &flag,const IDnumber &ID);
        #ifdef VARIADIC_ENABLED
        template<class componentType, typename ... types>
        bool tryAddComponent(const IDnumber &ID, const char* compName, std::unordered_map<IDnumber, componentType> &table, const types& ... args);
        #else
        void errComponentAlreadyExists(const char* compName, const IDnumber &ID);
        #endif
        bool addSoul(const IDnumber &ID, const char* name);
        
        /* COMPONENT DELETION */
        void deleteSoul(const IDnumber &ID);

    public:
        // Constructor for new states
        ComponentBank();
        // Constructor for loading saved states
        ComponentBank(const IDnumber &startingID);
        // Destructor
        ~ComponentBank();

        /* COMPONENT POINTER GETTERS */
        Soul* getSoulPtr(const IDnumber &ID);
        Model* getModelPtr(const IDnumber &ID);
        Motion* getMotionPtr(const IDnumber &ID);
        Spatial* getSpatialPtr(const IDnumber &ID);
        Control* getControlPtr(const IDnumber &ID);
        PointLight* getPointLightPtr(const IDnumber &ID);
        DirectionalLight* getDirectionalLightPtr(const IDnumber &ID);
        AmbientLight* getAmbientLightPtr(const IDnumber &ID);

        /* COMPONENT CREATION */
        void addModel(const IDnumber &ID);
        void addMotion(const IDnumber &ID, const double &velX, const double &velY, const double &velZ,
                                           const double &anvX, const double &anvY, const double &anvZ);
        void addSpatial(const IDnumber &ID, const double &posX, const double &posY, const double &posZ,
                                            const double &rotX, const double &rotY, const double &rotZ);
        void addControl(const IDnumber &ID);
        void addPointLight(const IDnumber &ID, const colorByte &red, const colorByte &green, const colorByte &blue,
                                  const double &posX, const double &posY, const double &posZ);
        void addDirectionalLight(const IDnumber &ID, const colorByte &red, const colorByte &green, const colorByte &blue,
                                        const double &rotX, const double &rotY, const double &rotZ);
        void addAmbientLight(const IDnumber &ID, const colorByte &red, const colorByte &green, const colorByte &blue);

        /* COMPONENT DELETION */
        void deleteModel(const IDnumber &ID);
        void deleteMotion(const IDnumber &ID);
        void deleteSpatial(const IDnumber &ID);
        void deleteControl(const IDnumber &ID);
        void deletePointLight(const IDnumber &ID);
        void deleteDirectionalLight(const IDnumber &ID);
        void deleteAmbientLight(const IDnumber &ID);

        /* ENTITY CREATION */
        IDnumber generateID();
        IDnumber createEntity(const char* name);
        bool     notifySystems(const IDnumber &ID);
        IDnumber createBox(const char* name,
                           const double posX, const double posY, const double posZ,
                           const double rotX, const double rotY, const double rotZ,
                           const double velX, const double velY, const double velZ,
                           const double anvX, const double anvY, const double anvZ);
        
        /* ENTITY DELETION */
        bool deleteEntity(const IDnumber &ID);  // orphans children
        bool purgeEntity(const IDnumber &ID);   // deletes children too.

        /* CONVENIENCE GETTERS */
        std::string getName(IDnumber &ID);
        std::string listComponents(IDnumber &ID);
    };

}

#endif	/* COMPONENTSTORE_H */

