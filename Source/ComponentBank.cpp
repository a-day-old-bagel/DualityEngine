/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

/* CONSTRUCTOR */
ComponentBank::ComponentBank(){
    nextID = 0;
}

/* CONSTRUCTOR FOR LOADING SAVED STATES */
ComponentBank::ComponentBank(IDnumber startingID){
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
void ComponentBank::addModel(IDnumber ID){
    components_model.emplace(std::make_pair(ID, Model{ }));
}
void ComponentBank::addMotion(IDnumber ID, double velX, double velY, double velZ,
                                           double anvX, double anvY, double anvZ){
    components_motion.emplace(std::make_pair(ID, Motion{
        {velX, velY, velZ}, {anvX, anvY, anvZ}}));
}
void ComponentBank::addSpatial(IDnumber ID, double posX, double posY, double posZ,
                                            double rotX, double rotY, double rotZ){
    components_spatial.emplace(std::make_pair(ID, Spatial{
        {posX, posY, posZ}, {rotX, rotY, rotZ}}));
}
void ComponentBank::addControl(IDnumber ID){
    components_control.emplace(std::make_pair(ID, Control{ }));
}
void ComponentBank::addSoul(IDnumber ID, const char* name, componentFlag components){    
    components_soul.emplace(std::make_pair(ID, Soul{
        name, components, defaultState}));
}
void ComponentBank::addPointLight(IDnumber ID, double posX, double posY, double posZ,
                                  colorByte red, colorByte green, colorByte blue){    
    components_pointLight.emplace(std::make_pair(ID, PointLight{
        {red, green, blue}, {posX, posY, posZ}}));
}
void ComponentBank::addDirectionalLight(IDnumber ID, double rotX, double rotY, double rotZ,
                                        colorByte red, colorByte green, colorByte blue){    
    components_directionalLight.emplace(std::make_pair(ID, DirectionalLight{
        {red, green, blue}, {rotX, rotY, rotZ}}));
}
void ComponentBank::addAmbientLight(IDnumber ID, colorByte red, colorByte green, colorByte blue){    
    components_ambientLight.emplace(std::make_pair(ID, AmbientLight{
        {red, green, blue}}));
}

/* COMPONENT DELETION */
void ComponentBank::deleteModel(const IDnumber &ID){
    components_model.erase(ID);
}
void ComponentBank::deleteMotion(const IDnumber &ID){
    components_motion.erase(ID);
}
void ComponentBank::deleteSpatial(const IDnumber &ID){
    components_spatial.erase(ID);
}
void ComponentBank::deleteControl(const IDnumber &ID){
    components_control.erase(ID);
}
void ComponentBank::deleteSoul(const IDnumber &ID){
    components_soul.erase(ID);
}
void ComponentBank::deletePointLight(const IDnumber &ID){
    components_pointLight.erase(ID);
}
void ComponentBank::deleteDirectionalLight(const IDnumber &ID){
    components_directionalLight.erase(ID);
}
void ComponentBank::deleteAmbientLight(const IDnumber &ID){
    components_ambientLight.erase(ID);
}

/* CONVENIENCE GETTERS */
std::string ComponentBank::getName(IDnumber &ID){
    return components_soul.at(ID).name;
}
std::string ComponentBank::nameComponents(IDnumber &ID){
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