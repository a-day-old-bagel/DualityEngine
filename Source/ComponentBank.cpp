/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

/* CONSTRUCTOR */
ComponentBank::ComponentBank(){
    nextID = 1; // IDnumber of 0 is an error code, so count up from 1.
}

/* CONSTRUCTOR FOR LOADING SAVED STATES */
ComponentBank::ComponentBank(const IDNUM &startingID){
    nextID = startingID;
}

ComponentBank::~ComponentBank(){
    
}

/* COMPONENT POINTER GETTERS */
Model* ComponentBank::getModelPtr(const IDNUM &ID){
    return &(components_model.at(ID));
}
Motion* ComponentBank::getMotionPtr(const IDNUM &ID){
    return &(components_motion.at(ID));
}
Spatial* ComponentBank::getSpatialPtr(const IDNUM &ID){
    return &(components_spatial.at(ID));
}
Control* ComponentBank::getControlPtr(const IDNUM &ID){
    return &(components_control.at(ID));
}
Soul* ComponentBank::getSoulPtr(const IDNUM &ID){
    return &(components_soul.at(ID));
}
PointLight* ComponentBank::getPointLightPtr(const IDNUM &ID){
    return &(components_pointLight.at(ID));
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const IDNUM &ID){
    return &(components_directionalLight.at(ID));
}
AmbientLight* ComponentBank::getAmbientLightPtr(const IDNUM &ID){
    return &(components_ambientLight.at(ID));
}
    
/* COMPONENT CREATION */
bool ComponentBank::tryAddFlagToSoul(const COMPFLAG &flag,const IDNUM &ID){
    try{
        components_soul.at(ID).components |= flag;
    } catch(const std::out_of_range& oorException) {
        printf("Permission denied to add component(s) at ID %lu: no soul yet exists at that ID\n", ID);
        return false;
    }
    return true;
}
#ifdef VARIADIC_ENABLED
template<class componentType, typename ... types>
bool ComponentBank::tryAddComponent(const IDNUM &ID, const char* compName, std::unordered_map<IDNUM, componentType> &table, const types& ... args){
    if ((table.emplace(std::piecewise_construct, std::forward_as_tuple(ID), std::forward_as_tuple(args...))).second == false){
        printf("Could not add %s component at ID %lu: %s component already exists at that ID.\n", compName, ID, compName);
        return false;
    }
    return true;
}
#else
void ComponentBank::errComponentAlreadyExists(const char* compName, const IDNUM &ID){
    printf("Could not add %s component at ID %lu: %s component already exists at that ID.\n", compName, ID, compName);
}
#endif
bool ComponentBank::addSoul(const IDNUM &ID, const char* name){
    #ifdef VARIADIC_ENABLED
    return tryAddComponent(ID, "soul", components_soul, name, defaultComponents, defaultState);
    #else    
    if ((components_soul.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(name, defaultComponents, defaultState))).second == true)
        return true;
    else {
        errComponentAlreadyExists("soul", ID);
        return false;
    }
    #endif
}
void ComponentBank::addModel(const IDNUM &ID){
    if (tryAddFlagToSoul(MODEL, ID)){
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "model", components_model);
        #else    
        if ((components_model.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple())).second == false)
            errComponentAlreadyExists("model", ID);
        #endif
    }
}
void ComponentBank::addMotion(const IDNUM &ID, const FLOAT &velX, const FLOAT &velY, const FLOAT &velZ,
                                           const FLOAT &anvX, const FLOAT &anvY, const FLOAT &anvZ){
    if (tryAddFlagToSoul(MOTION, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "motion", components_motion, velX, velY, velZ, anvX, anvY, anvZ);
        #else    
        if ((components_motion.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(velX, velY, velZ, anvX, anvY, anvZ))).second == false)
            errComponentAlreadyExists("motion", ID);
        #endif
}
void ComponentBank::addSpatial(const IDNUM &ID, const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ,
                                            const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ){
    if (tryAddFlagToSoul(SPATIAL, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "spatial", components_spatial, posX, posY, posZ, rotX, rotY, rotZ);
        #else    
        if ((components_spatial.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(posX, posY, posZ, rotX, rotY, rotZ))).second == false)
            errComponentAlreadyExists("spatial", ID);
        #endif
}
void ComponentBank::addControl(const IDNUM &ID){
    if (tryAddFlagToSoul(CONTROL, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "control", components_control);
        #else    
        if ((components_control.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple())).second == false)
            errComponentAlreadyExists("control", ID);
        #endif
}
void ComponentBank::addPointLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue,
                                  const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ){
    if (tryAddFlagToSoul(LPOINT, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "point light", components_pointLight, red, green, blue, posX, posY, posZ);
        #else    
        if ((components_pointLight.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(red, green, blue, posX, posY, posZ))).second == false)
            errComponentAlreadyExists("point light", ID);
        #endif
}
void ComponentBank::addDirectionalLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue,
                                        const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ){
    if (tryAddFlagToSoul(LDIRECT, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "directional light", components_directionalLight, red, green, blue, rotX, rotY, rotZ);
        #else    
        if ((components_directionalLight.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(red, green, blue, rotX, rotY, rotZ))).second == false)
            errComponentAlreadyExists("directional light", ID);
        #endif
}
void ComponentBank::addAmbientLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue){    
    if (tryAddFlagToSoul(LAMBIENT, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "ambient light", components_ambientLight, red, green, blue);
        #else    
        if ((components_ambientLight.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(red, green, blue))).second == false)
            errComponentAlreadyExists("ambient light", ID);
        #endif
}

/* COMPONENT DELETION */
void ComponentBank::deleteSoul(const IDNUM &ID){
    components_soul.erase(ID);
}
void ComponentBank::deleteModel(const IDNUM &ID){
    components_model.erase(ID);
    components_soul.at(ID).components &= ~MODEL;
}
void ComponentBank::deleteMotion(const IDNUM &ID){
    components_motion.erase(ID);
    components_soul.at(ID).components &= ~MOTION;
}
void ComponentBank::deleteSpatial(const IDNUM &ID){
    components_spatial.erase(ID);
    components_soul.at(ID).components &= ~SPATIAL;
}
void ComponentBank::deleteControl(const IDNUM &ID){
    components_control.erase(ID);
    components_soul.at(ID).components &= ~CONTROL;
}
void ComponentBank::deletePointLight(const IDNUM &ID){
    components_pointLight.erase(ID);
    components_soul.at(ID).components &= ~LPOINT;
}
void ComponentBank::deleteDirectionalLight(const IDNUM &ID){
    components_directionalLight.erase(ID);
    components_soul.at(ID).components &= ~LDIRECT;
}
void ComponentBank::deleteAmbientLight(const IDNUM &ID){
    components_ambientLight.erase(ID);
    components_soul.at(ID).components &= ~LAMBIENT;
}

/* ENTITY CREATION */
IDNUM ComponentBank::generateID(){
    return nextID++;
}
IDNUM ComponentBank::createEntity(const char* name){
    IDNUM ID = generateID();
    if (addSoul(ID, name))
        return ID;
    else {
        printf("New entity '%s' not created!\n", name);
        return NULL_ID;        
    }    
}
bool ComponentBank::notifySystemsOfAdditions(const IDNUM &ID){
    return true;
}

/* CONVENIENCE GETTERS */
std::string ComponentBank::getName(IDNUM &ID){
    return components_soul.at(ID).name;
}
std::string ComponentBank::listComponents(IDNUM &ID){
    std::ostringstream output;
    COMPFLAG components = components_soul.at(ID).components;
    if (components & MODEL)
        output << "MODEL ";
    if (components & SPATIAL)
        output << "SPATIAL ";
    if (components & MOTION)
        output << "MOTION ";
    if (components & COLLISION)
        output << "COLLISION ";
    if (components & CONTROL)
        output << "CONTROL ";
    if (components & LAMBIENT)
        output << "LAMBIENT ";
    if (components & LDIRECT)
        output << "LDIRECT ";
    if (components & LPOINT)
        output << "LPOINT ";
    
    return output.str();
}