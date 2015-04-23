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
#include "Model.h"
#include "Control.h"
#include "Motion.h"
#include "Spatial.h"
#include "Soul.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "AmbientLight.h"

class ComponentBank
{
    IDnumber nextID;
    
    std::unordered_map<IDnumber, Model*> components_model;
    std::unordered_map<IDnumber, Motion*> components_motion;
    std::unordered_map<IDnumber, Spatial*> components_spatial;
    std::unordered_map<IDnumber, Control*> components_control;
    std::unordered_map<IDnumber, Soul*> components_soul;
    std::unordered_map<IDnumber, PointLight*> components_pointLight;
    std::unordered_map<IDnumber, DirectionalLight*> components_directionalLight;
    std::unordered_map<IDnumber, AmbientLight*> components_ambientLight;
    
public:
    // Constructor for new states
    ComponentBank();
    // Constructor for loading saved states
    ComponentBank(IDnumber startingID);
    // Destructor
    ~ComponentBank();
    
    /* COMPONENT POINTER GETTERS */
    Model* getModelPtr(const IDnumber &ID);
    Motion* getMotionPtr(const IDnumber &ID);
    Spatial* getSpatialPtr(const IDnumber &ID);
    Control* getControlPtr(const IDnumber &ID);
    Soul* getSoulPtr(const IDnumber &ID);
    PointLight* getPointLightPtr(const IDnumber &ID);
    DirectionalLight* getDirectionalLightPtr(const IDnumber &ID);
    AmbientLight* getAmbientLightPtr(const IDnumber &ID);
    
    /* COMPONENT CREATION */
    void addModel(IDnumber ID);
    void addMotion(IDnumber ID, double velX, double velY, double velZ, double anvX, double anvY, double anvZ);
    void addSpatial(IDnumber ID, double posX, double posY, double posZ, double rotX, double rotY, double rotZ);
    void addControl(IDnumber ID);
    void addSoul(IDnumber ID, const char*, componentFlag components);
    void addPointLight(IDnumber ID, double posX, double posY, double posZ, colorByte red, colorByte green, colorByte blue);
    void addDirectionalLight(IDnumber ID, double rotX, double rotY, double rotZ, colorByte red, colorByte green, colorByte blue);
    void addAmbientLight(IDnumber ID, colorByte red, colorByte green, colorByte blue);
    
    /* COMPONENT DELETION */
    void deleteModel(const IDnumber &ID);
    void deleteMotion(const IDnumber &ID);
    void deleteSpatial(const IDnumber &ID);
    void deleteControl(const IDnumber &ID);
    void deleteSoul(const IDnumber &ID);
    void deletePointLight(const IDnumber &ID);
    void deleteDirectionalLight(const IDnumber &ID);
    void deleteAmbientLight(const IDnumber &ID);
    
    /* ENTITY CREATION */
    IDnumber generateID() { return nextID++; }
    IDnumber createBox(double posX, double posY, double posZ, double rotX, double rotY, double rotZ, double velX, double velY, double velZ, double anvX, double anvY, double anvZ);
    
    /* ENTITY GETTERS */
    std::string getName(IDnumber &ID);
    std::string nameComponents(IDnumber &ID);
};

#endif	/* COMPONENTSTORE_H */

