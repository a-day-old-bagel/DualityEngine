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
ComponentBank::ComponentBank(const IDnumber &startingID){
    nextID = startingID;
}

ComponentBank::~ComponentBank(){
    
}

/* COMPONENT POINTER GETTERS */
Model* ComponentBank::getModelPtr(const IDnumber &ID){
    return &(components_model.at(ID));
}
Motion* ComponentBank::getMotionPtr(const IDnumber &ID){
    return &(components_motion.at(ID));
}
Spatial* ComponentBank::getSpatialPtr(const IDnumber &ID){
    return &(components_spatial.at(ID));
}
Control* ComponentBank::getControlPtr(const IDnumber &ID){
    return &(components_control.at(ID));
}
Soul* ComponentBank::getSoulPtr(const IDnumber &ID){
    return &(components_soul.at(ID));
}
PointLight* ComponentBank::getPointLightPtr(const IDnumber &ID){
    return &(components_pointLight.at(ID));
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const IDnumber &ID){
    return &(components_directionalLight.at(ID));
}
AmbientLight* ComponentBank::getAmbientLightPtr(const IDnumber &ID){
    return &(components_ambientLight.at(ID));
}
    
/* COMPONENT CREATION */
bool ComponentBank::tryAddFlagToSoul(const componentFlag &flag,const IDnumber &ID){
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
bool ComponentBank::tryAddComponent(const IDnumber &ID, const char* compName, std::unordered_map<IDnumber, componentType> &table, const types& ... args){
    if ((table.emplace(std::piecewise_construct, std::forward_as_tuple(ID), std::forward_as_tuple(args...))).second == false){
        printf("Could not add %s component at ID %lu: %s component already exists at that ID.\n", compName, ID, compName);
        return false;
    }
    return true;
}
#else
void ComponentBank::errComponentAlreadyExists(const char* compName, const IDnumber &ID){
    printf("Could not add %s component at ID %lu: %s component already exists at that ID.\n", compName, ID, compName);
}
#endif
bool ComponentBank::addSoul(const IDnumber &ID, const char* name){
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
void ComponentBank::addModel(const IDnumber &ID){
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
void ComponentBank::addMotion(const IDnumber &ID, const double &velX, const double &velY, const double &velZ,
                                           const double &anvX, const double &anvY, const double &anvZ){
    if (tryAddFlagToSoul(MOTION, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "motion", components_motion, velX, velY, velZ, anvX, anvY, anvZ);
        #else    
        if ((components_motion.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(velX, velY, velZ, anvX, anvY, anvZ))).second == false)
            errComponentAlreadyExists("motion", ID);
        #endif
}
void ComponentBank::addSpatial(const IDnumber &ID, const double &posX, const double &posY, const double &posZ,
                                            const double &rotX, const double &rotY, const double &rotZ){
    if (tryAddFlagToSoul(SPATIAL, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "spatial", components_spatial, posX, posY, posZ, rotX, rotY, rotZ);
        #else    
        if ((components_spatial.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(posX, posY, posZ, rotX, rotY, rotZ))).second == false)
            errComponentAlreadyExists("spatial", ID);
        #endif
}
void ComponentBank::addControl(const IDnumber &ID){
    if (tryAddFlagToSoul(CONTROL, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "control", components_control);
        #else    
        if ((components_control.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple())).second == false)
            errComponentAlreadyExists("control", ID);
        #endif
}
void ComponentBank::addPointLight(const IDnumber &ID, const colorByte &red, const colorByte &green, const colorByte &blue,
                                  const double &posX, const double &posY, const double &posZ){
    if (tryAddFlagToSoul(LPOINT, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "point light", components_pointLight, red, green, blue, posX, posY, posZ);
        #else    
        if ((components_pointLight.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(red, green, blue, posX, posY, posZ))).second == false)
            errComponentAlreadyExists("point light", ID);
        #endif
}
void ComponentBank::addDirectionalLight(const IDnumber &ID, const colorByte &red, const colorByte &green, const colorByte &blue,
                                        const double &rotX, const double &rotY, const double &rotZ){
    if (tryAddFlagToSoul(LDIRECT, ID))
        #ifdef VARIADIC_ENABLED
        tryAddComponent(ID, "directional light", components_directionalLight, red, green, blue, rotX, rotY, rotZ);
        #else    
        if ((components_directionalLight.emplace(std::piecewise_construct, std::forward_as_tuple(ID),
        std::forward_as_tuple(red, green, blue, rotX, rotY, rotZ))).second == false)
            errComponentAlreadyExists("directional light", ID);
        #endif
}
void ComponentBank::addAmbientLight(const IDnumber &ID, const colorByte &red, const colorByte &green, const colorByte &blue){    
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
void ComponentBank::deleteSoul(const IDnumber &ID){
    components_soul.erase(ID);
}
void ComponentBank::deleteModel(const IDnumber &ID){
    components_model.erase(ID);
    components_soul.at(ID).components &= ~MODEL;
}
void ComponentBank::deleteMotion(const IDnumber &ID){
    components_motion.erase(ID);
    components_soul.at(ID).components &= ~MOTION;
}
void ComponentBank::deleteSpatial(const IDnumber &ID){
    components_spatial.erase(ID);
    components_soul.at(ID).components &= ~SPATIAL;
}
void ComponentBank::deleteControl(const IDnumber &ID){
    components_control.erase(ID);
    components_soul.at(ID).components &= ~CONTROL;
}
void ComponentBank::deletePointLight(const IDnumber &ID){
    components_pointLight.erase(ID);
    components_soul.at(ID).components &= ~LPOINT;
}
void ComponentBank::deleteDirectionalLight(const IDnumber &ID){
    components_directionalLight.erase(ID);
    components_soul.at(ID).components &= ~LDIRECT;
}
void ComponentBank::deleteAmbientLight(const IDnumber &ID){
    components_ambientLight.erase(ID);
    components_soul.at(ID).components &= ~LAMBIENT;
}

/* ENTITY CREATION */
IDnumber ComponentBank::generateID(){
    return nextID++;
}
IDnumber ComponentBank::createEntity(const char* name){
    IDnumber ID = generateID();
    if (addSoul(ID, name))
        return ID;
    else {
        printf("New entity '%s' not created!\n", name);
        return NULL_ID;        
    }    
}
bool ComponentBank::notifySystems(const IDnumber &ID){
    return true;
}

/* CONVENIENCE GETTERS */
std::string ComponentBank::getName(IDnumber &ID){
    return components_soul.at(ID).name;
}
std::string ComponentBank::listComponents(IDnumber &ID){
    std::ostringstream output;
    componentFlag components = components_soul.at(ID).components;
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