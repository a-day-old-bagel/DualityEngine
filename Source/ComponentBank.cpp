/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

/* CONSTRUCTOR */
ComponentBank::ComponentBank()
{
    nextID = 0;
}

/* CONSTRUCTOR FOR LOADING SAVED STATES */
ComponentBank::ComponentBank(IDnumber startingID)
{
    nextID = startingID;
}

ComponentBank::~ComponentBank()
{
    for (auto model : components_model)
        deleteModel(model.first);
    for (auto motion : components_motion)
        deleteMotion(motion.first);
    for (auto spatial : components_spatial)
        deleteSpatial(spatial.first);
    for (auto control : components_control)
        deleteControl(control.first);
    for (auto soul : components_soul)
        deleteSoul(soul.first);
    for (auto pointLight : components_pointLight)
        deletePointLight(pointLight.first);
    for (auto directionalLight : components_directionalLight)
        deleteDirectionalLight(directionalLight.first);
    for (auto ambientLight : components_ambientLight)
        deleteAmbientLight(ambientLight.first);
}

/* COMPONENT POINTER GETTERS */
Model* ComponentBank::getModelPtr(const IDnumber &ID)
{
    return components_model.at(ID);
}
Motion* ComponentBank::getMotionPtr(const IDnumber &ID)
{
    return components_motion.at(ID);
}
Spatial* ComponentBank::getSpatialPtr(const IDnumber &ID)
{
    return components_spatial.at(ID);
}
Control* ComponentBank::getControlPtr(const IDnumber &ID)
{
    return components_control.at(ID);
}
Soul* ComponentBank::getSoulPtr(const IDnumber &ID)
{
    return components_soul.at(ID);
}
PointLight* ComponentBank::getPointLightPtr(const IDnumber &ID)
{
    return components_pointLight.at(ID);
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const IDnumber &ID)
{
    return components_directionalLight.at(ID);
}
AmbientLight* ComponentBank::getAmbientLightPtr(const IDnumber &ID)
{
    return components_ambientLight.at(ID);
}
    
/* COMPONENT CREATION */
void ComponentBank::addModel(IDnumber ID)
{
    Model* model = new Model();
    components_model.emplace(ID, model);
    model = NULL;
}
void ComponentBank::addMotion(IDnumber ID, double velX, double velY, double velZ, double anvX, double anvY, double anvZ)
{
    Motion* motion = new Motion();
    
    motion->velRegular.x = velX;
    motion->velRegular.y = velY;
    motion->velRegular.z = velZ;
    motion->velAngular.x = anvX;
    motion->velAngular.y = anvY;
    motion->velAngular.z = anvZ;
    
    components_motion.emplace(ID, motion);
    motion = NULL;
}
void ComponentBank::addSpatial(IDnumber ID, double posX, double posY, double posZ, double rotX, double rotY, double rotZ)
{
    Spatial* spatial = new Spatial();
    
    spatial->position.x = posX;
    spatial->position.y = posX;
    spatial->position.z = posX;
    spatial->rotation.x = rotX;
    spatial->rotation.y = rotY;
    spatial->rotation.z = rotZ;
    
    components_spatial.emplace(ID, spatial);
    spatial = NULL;
}
void ComponentBank::addControl(IDnumber ID)
{
    Control* control = new Control();
    components_control.emplace(ID, control);
    control = NULL;
}
void ComponentBank::addSoul(IDnumber ID, const char* name, componentFlag components)
{
    Soul* soul = new Soul();
    
    soul->name = name;
    soul->components = components;
    
    components_soul.emplace(ID, soul);
    soul = NULL;
}
void ComponentBank::addPointLight(IDnumber ID, double posX, double posY, double posZ, colorByte red, colorByte green, colorByte blue)
{
    PointLight* pointLight = new PointLight();
    
    pointLight->color.r = red;
    pointLight->color.g = green;
    pointLight->color.b = blue;
    
    components_pointLight.emplace(ID, pointLight);
    pointLight = NULL;
}
void ComponentBank::addDirectionalLight(IDnumber ID, double rotX, double rotY, double rotZ, colorByte red, colorByte green, colorByte blue)
{
    DirectionalLight* directionalLight = new DirectionalLight();
    
    directionalLight->color.r = red;
    directionalLight->color.g = green;
    directionalLight->color.b = blue;
    
    components_directionalLight.emplace(ID, directionalLight);
    directionalLight = NULL;
}
void ComponentBank::addAmbientLight(IDnumber ID, colorByte red, colorByte green, colorByte blue)
{
    AmbientLight* ambientLight = new AmbientLight();
    
    ambientLight->color.r = red;
    ambientLight->color.g = green;
    ambientLight->color.b = blue;
    
    components_ambientLight.emplace(ID, ambientLight);
    ambientLight = NULL;
}

/* COMPONENT DELETION */
void ComponentBank::deleteModel(const IDnumber &ID)
{
    delete components_model.at(ID);
    components_model.at(ID) = NULL;
}
void ComponentBank::deleteMotion(const IDnumber &ID)
{
    delete components_motion.at(ID);
    components_motion.at(ID) = NULL;
}
void ComponentBank::deleteSpatial(const IDnumber &ID)
{
    delete components_spatial.at(ID);
    components_spatial.at(ID) = NULL;
}
void ComponentBank::deleteControl(const IDnumber &ID)
{
    delete components_control.at(ID);
    components_control.at(ID) = NULL;
}
void ComponentBank::deleteSoul(const IDnumber &ID)
{
    delete components_soul.at(ID);
    components_soul.at(ID) = NULL;
}
void ComponentBank::deletePointLight(const IDnumber &ID)
{
    delete components_pointLight.at(ID);
    components_pointLight.at(ID) = NULL;
}
void ComponentBank::deleteDirectionalLight(const IDnumber &ID)
{
    delete components_directionalLight.at(ID);
    components_directionalLight.at(ID) = NULL;
}
void ComponentBank::deleteAmbientLight(const IDnumber &ID)
{
    delete components_ambientLight.at(ID);
    components_ambientLight.at(ID) = NULL;
}

std::string ComponentBank::getName(IDnumber &ID)
{
    return components_soul.at(ID)->name;
}
std::string ComponentBank::nameComponents(IDnumber &ID)
{
    std::ostringstream output;
    componentFlag components = components_soul.at(ID)->components;
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