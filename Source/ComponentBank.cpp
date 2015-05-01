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
Soul* ComponentBank::getSoulPtr(const IDNUM &ID){
    return &(components_soul.at(ID));
}
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
PointLight* ComponentBank::getPointLightPtr(const IDNUM &ID){
    return &(components_pointLight.at(ID));
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const IDNUM &ID){
    return &(components_directionalLight.at(ID));
}
AmbientLight* ComponentBank::getAmbientLightPtr(const IDNUM &ID){
    return &(components_ambientLight.at(ID));
}
    
/*******************************************************************************
 * COMPONENT CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * TRY ADD FLAG TO SOUL
 * if soul component exists at given ID, given component bit flag is OR'ed to
 * that soul's component flags.  Otherwise an error message is output.
 ******************************************************************************/
bool ComponentBank::tryAddFlagToSoul(const COMPFLAG &flag,const IDNUM &ID){
    try{
        components_soul.at(ID).components |= flag;
    } catch(const std::out_of_range& oorException) {
        printf("Permission denied to add component(s) at ID %lu: no soul yet exists at that ID\n", ID);
        return false;
    }
    return true;
}
/*******************************************************************************
 * TRY ADD COMPONENT [VARIADIC TEMPLATED]
 * adds a component of type 'componentType' to the appropriate collection at 'ID',
 * initializing it with the given arguments, as long as a component of that
 * type doesn't already exist at that ID.  In that case, an error message is output.
 * 
 * format for a call is:
 * tryAddComponent([ID at which to add the component],
 * [string describing the component, like "model" or "motion"],
 * [collection to which to add the component, like 'components_model'],
 *      and now whatever arguments the component in question takes...
 * [arg0], [arg1], ... ,[lastArg]);
 ******************************************************************************/
template<class componentType, typename ... types>
bool ComponentBank::tryAddComponent(const IDNUM &ID, const char* compName, std::unordered_map<IDNUM, componentType> &table, const types& ... args){
    if ((table.emplace(std::piecewise_construct, std::forward_as_tuple(ID), std::forward_as_tuple(args...))).second == false){
        printf("Could not add %s component at ID %lu: %s component already exists at that ID.\n", compName, ID, compName);
        return false;
    }
    return true;
}
/*******************************************************************************
 * ADD SOUL
 * is a little different than the rest below because it doesn't need to add any
 * flags to a soul component (for obvious reasons). You won't need to call it
 * anyway - it's private and wrapped into "createEntity."
 ******************************************************************************/
bool ComponentBank::addSoul(const IDNUM &ID, const char* name){
    return tryAddComponent(ID, "soul", components_soul, name, defaultComponents, defaultState);
}
/*******************************************************************************
 * ADD [COMPONENT] FUNCTIONS:
 * all of these are formatted the same way, but still separate so that they can
 * be unwrapped at compile time instead of run time (performance thing).
 * Anyway, this is how you lay out a function for adding a given component:
 * 
 * step0: call "tryAddFlagToSoul" passing the component enumerator and desired ID.
 * if this returns true, it means that there exists a soul component with that ID,
 * and that the bit flag you gave it has been OR'ed to that soul's 'componentFlags.'
 * This is a necessary condition to add components, so if it returns true, move
 * on to:
 * 
 * step1: call "tryAddComponent" passing the desired ID, a string describing the
 * component (for error output), the collection in which to insert the component
 * (make sure these all match up), and finally whatever arguments the component
 * takes in its constructor.  This last bit is possible thanks to variadic
 * templates, as seen in the declaration of "tryAddComponent".
 * If there's already a component of that type at the given ID, an error message
 * will be output, and the add will be ineffective.
 * 
 * And that's it - two steps. Just follow the format of the below if you're confused.
 ******************************************************************************/
void ComponentBank::addModel(const IDNUM &ID){
    if (tryAddFlagToSoul(MODEL, ID))
        tryAddComponent(ID, "model", components_model);
}
void ComponentBank::addMotion(const IDNUM &ID, const FLOAT &velX, const FLOAT &velY, const FLOAT &velZ,
                                           const FLOAT &anvX, const FLOAT &anvY, const FLOAT &anvZ){
    if (tryAddFlagToSoul(MOTION, ID))
        tryAddComponent(ID, "motion", components_motion, velX, velY, velZ, anvX, anvY, anvZ);
}
void ComponentBank::addSpatial(const IDNUM &ID, const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ,
                                            const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ){
    if (tryAddFlagToSoul(SPATIAL, ID))
        tryAddComponent(ID, "spatial", components_spatial, posX, posY, posZ, rotX, rotY, rotZ);
}
void ComponentBank::addControl(const IDNUM &ID){
    if (tryAddFlagToSoul(CONTROL, ID))
        tryAddComponent(ID, "control", components_control);
}
void ComponentBank::addPointLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue,
                                  const FLOAT &posX, const FLOAT &posY, const FLOAT &posZ){
    if (tryAddFlagToSoul(LPOINT, ID))
        tryAddComponent(ID, "point light", components_pointLight, red, green, blue, posX, posY, posZ);
}
void ComponentBank::addDirectionalLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue,
                                        const FLOAT &rotX, const FLOAT &rotY, const FLOAT &rotZ){
    if (tryAddFlagToSoul(LDIRECT, ID))
        tryAddComponent(ID, "directional light", components_directionalLight, red, green, blue, rotX, rotY, rotZ);
}
void ComponentBank::addAmbientLight(const IDNUM &ID, const COLORBYTE &red, const COLORBYTE &green, const COLORBYTE &blue){    
    if (tryAddFlagToSoul(LAMBIENT, ID))
        tryAddComponent(ID, "ambient light", components_ambientLight, red, green, blue);
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