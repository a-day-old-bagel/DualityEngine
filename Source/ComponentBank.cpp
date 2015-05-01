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

/*******************************************************************************
 * COMPONENT POINTER GETTERS SECTION - I KNOW THESE ARE A BAD IDEA.
 ******************************************************************************/
template<class componentType>
componentType* getComponentPtr(const IDNUM &ID, const char* compName, std::unordered_map<IDNUM, componentType> &table){
    try {
        return &(table.at(ID));
    }  catch(const std::out_of_range& oorException) {
        printf("No %s component exists with ID %lu.", compName, ID);
        return NULL;
    }
}
Soul* ComponentBank::getSoulPtr(const IDNUM &ID){
    return getComponentPtr(ID, "soul", components_soul);
}
Model* ComponentBank::getModelPtr(const IDNUM &ID){
    return getComponentPtr(ID, "model", components_model);
}
Motion* ComponentBank::getMotionPtr(const IDNUM &ID){
    return getComponentPtr(ID, "motion", components_motion);
}
Spatial* ComponentBank::getSpatialPtr(const IDNUM &ID){
    return getComponentPtr(ID, "spatial", components_spatial);
}
SpatialChild* ComponentBank::getSpatialChildPtr(const uint_fast32_t& ID){
    return getComponentPtr(ID, "spatial child", components_spatialChild);
}
SpatialParent* ComponentBank::getSpatialParentPtr(const uint_fast32_t& ID){
    return getComponentPtr(ID, "spatial parent", components_spatialParent);
}
Control* ComponentBank::getControlPtr(const IDNUM &ID){
    return getComponentPtr(ID, "control", components_control);
}
PointLight* ComponentBank::getPointLightPtr(const IDNUM &ID){
    return getComponentPtr(ID, "point light", components_pointLight);
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const IDNUM &ID){
    return getComponentPtr(ID, "directional light", components_directionalLight);
}
AmbientLight* ComponentBank::getAmbientLightPtr(const IDNUM &ID){
    return getComponentPtr(ID, "ambient light", components_ambientLight);
}
Owner* ComponentBank::getOwnerPtr(const uint_fast32_t& ID){
    return getComponentPtr(ID, "owner", components_owner);
}
Score* ComponentBank::getScorePtr(const uint_fast32_t& ID){
    return getComponentPtr(ID, "score", components_score);
}
    
/*******************************************************************************
 * COMPONENT CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * TRY ADD FLAG TO SOUL
 * if soul component exists at given ID, given component bit flag is OR'ed to
 * that soul's component flags.  Otherwise an error message is output.
 ******************************************************************************/
bool ComponentBank::tryAddFlagToSoul(const COMPFLAG &flag, const IDNUM &ID){
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
void ComponentBank::addSpatialChild(const uint_fast32_t& ID, const uint_fast32_t& refID){
    if (tryAddFlagToSoul(SPATCHILD, ID))
        tryAddComponent(ID, "spatial child", components_spatialChild, refID);
}
void ComponentBank::addSpatialParent(const uint_fast32_t& ID, const uint_fast32_t& refID){
    if (tryAddFlagToSoul(SPATPARENT, ID))
        tryAddComponent(ID, "spatial parent", components_spatialParent, refID);
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
void ComponentBank::addOwner(const uint_fast32_t& ID, const uint_fast32_t& refID){
    if (tryAddFlagToSoul(OWNER, ID))
        tryAddComponent(ID, "owner", components_owner, refID);
}
void ComponentBank::addScore(const uint_fast32_t& ID){
    if (tryAddFlagToSoul(SCORE, ID))
        tryAddComponent(ID, "score", components_score);
}

/*******************************************************************************
 * COMPONENT DELETION SECTION
 ******************************************************************************/
/*******************************************************************************
 * TRY REMOVE FLAG FROM SOUL
 * tries to AND a NOT-flag onto the component flags of the soul at ID.
 * if there isn't a soul there, it outputs an error message.
 ******************************************************************************/
void ComponentBank::tryRemoveFlagFromSoul(const COMPFLAG &flag, const IDNUM &ID){
    try{
        components_soul.at(ID).components &= ~flag;
    } catch(const std::out_of_range& oorException) {
        printf("Cannot remove component flag at ID %lu: no soul exists at that ID\n", ID);
    }
}
/*******************************************************************************
 * TRY REMOVE COMPONENT [TEMPLATED]
 * tries to call erase on the collection 'table' of a component of type
 * 'componentType' at ID 'ID'. Outputs a message if there's no component there.
 ******************************************************************************/
template<class componentType>
bool ComponentBank::tryRemoveComponent(const IDNUM &ID, const char* compName, std::unordered_map<IDNUM, componentType> &table){
    if (table.erase(ID) == 0){
        printf("Could not remove %s component at ID %lu: no such component exists.\n", compName, ID);
        return false;
    }
    return true;
}
/*******************************************************************************
 * DELETE SOUL
 * just like the below functions, except doesn't need to clean any flags.
 * it's private, and wrapped by "deleteEntity."
 ******************************************************************************/
bool ComponentBank::deleteSoul(const IDNUM &ID){
    return tryRemoveComponent(ID, "soul", components_soul);
}
/*******************************************************************************
 * DELETE [COMPONENT] FUNCTIONS:
 * OK, pretty straightforward like the create component functions above.  First
 * try to remove the actual component from it's collection, then if that worked,
 * try to erase the flag from that entity's soul.
 * 
 * This will work in the case
 * that a component has somehow been orphaned (the corresponding soul has been
 * deleted).  It will not work in the case that there's no actual component, but
 * the soul still has a flag for some reason. To make this work in both cases,
 * just call both methods without any of the "if" nonsense.  However, this will
 * cause double error messages to be output in most of the failure cases.
 ******************************************************************************/
void ComponentBank::deleteModel(const IDNUM &ID){
    if (tryRemoveComponent(ID, "model", components_model))
        tryRemoveFlagFromSoul(MODEL, ID);
}
void ComponentBank::deleteMotion(const IDNUM &ID){
    if (tryRemoveComponent(ID, "motion", components_motion))
        tryRemoveFlagFromSoul(MOTION, ID);
}
void ComponentBank::deleteSpatial(const IDNUM &ID){
    if (tryRemoveComponent(ID, "spatial", components_spatial))
        tryRemoveFlagFromSoul(SPATIAL, ID);
}
void ComponentBank::deleteSpatialChild(const uint_fast32_t& ID){
    if (tryRemoveComponent(ID, "spatial child", components_spatialChild))
        tryRemoveFlagFromSoul(SPATCHILD, ID);
}
void ComponentBank::deleteSpatialParent(const uint_fast32_t& ID){
    if (tryRemoveComponent(ID, "spatial parent", components_spatialParent))
        tryRemoveFlagFromSoul(SPATPARENT, ID);
}
void ComponentBank::deleteControl(const IDNUM &ID){
    if (tryRemoveComponent(ID, "control", components_control))
        tryRemoveFlagFromSoul(CONTROL, ID);
}
void ComponentBank::deletePointLight(const IDNUM &ID){
    if (tryRemoveComponent(ID, "point light", components_pointLight))
        tryRemoveFlagFromSoul(LPOINT, ID);
}
void ComponentBank::deleteDirectionalLight(const IDNUM &ID){
    if (tryRemoveComponent(ID, "directional light", components_directionalLight))
        tryRemoveFlagFromSoul(LDIRECT, ID);
}
void ComponentBank::deleteAmbientLight(const IDNUM &ID){
    if (tryRemoveComponent(ID, "ambient light", components_ambientLight))
        tryRemoveFlagFromSoul(LAMBIENT, ID);
}
void ComponentBank::deleteOwner(const uint_fast32_t& ID){
    if (tryRemoveComponent(ID, "owner", components_owner))
        tryRemoveFlagFromSoul(OWNER, ID);
}
void ComponentBank::deleteScore(const uint_fast32_t& ID){
    if (tryRemoveComponent(ID, "score", components_score))
        tryRemoveFlagFromSoul(SCORE, ID);
}

/*******************************************************************************
 * ENTITY CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * GENERATE ID
 ******************************************************************************/
IDNUM ComponentBank::generateID(){
    return nextID++;
}
/*******************************************************************************
 * CREATE ENTITY
 ******************************************************************************/
IDNUM ComponentBank::createEntity(const char* name){
    IDNUM ID = generateID();
    if (addSoul(ID, name))
        return ID;
    else {
        printf("New entity '%s' not created!\n", name);
        return NULL_ID;        
    }    
}
/*******************************************************************************
 * NOTIFY SYSTEMS OF ADDITIONS
 ******************************************************************************/
bool ComponentBank::notifySystemsOfAdditions(const IDNUM &ID){
    return true;
}

/*******************************************************************************
 * ENTITY DELETION SECTION
 ******************************************************************************/
/*******************************************************************************
 * DELETE ENTITY
 * first attempts to remove all non-soul components with ID 'ID', then
 * REMOVES THE ENTITY'S SOUL!!!! <- this is the main reason I named it "soul."
 ******************************************************************************/
bool ComponentBank::deleteEntity(const uint_fast32_t& ID){
    COMPFLAG flags;
    std::string name;
    try {
        flags = components_soul.at(ID).components;
        name = components_soul.at(ID).name;
    } catch(const std::out_of_range& oorException) {
        printf("Could not delete entity: No soul component exists at ID %lu.", ID);
        return false;
    }
    
    try {
        
    if (flags & MODEL) deleteModel(ID);
    if (flags & MOTION) deleteMotion(ID);
    if (flags & SPATIAL) deleteSpatial(ID);
    if (flags & SPATCHILD) deleteSpatialChild(ID);
    if (flags & SPATPARENT) deleteSpatialParent(ID);
    if (flags & CONTROL) deleteControl(ID);
    if (flags & LPOINT) deletePointLight(ID);
    if (flags & LDIRECT) deleteDirectionalLight(ID);
    if (flags & LAMBIENT) deleteAmbientLight(ID);
    if (flags & OWNER) deleteOwner(ID);
    if (flags & SCORE) deleteScore(ID);
    
    deleteSoul(ID);
    
    } catch(...) {
        printf("Something went wrong upon attempting to delete entity %s ( ID %lu )!", name.c_str(), ID);
    }
    
    printf("Entity '%s' ( ID %lu ) has been destroyed.", name.c_str(), ID);
}
/*******************************************************************************
 * PURGE ENTITY
 * this should not only delete the entity, but also delete any entities that are
 * spatial children of it.
 ******************************************************************************/
bool ComponentBank::purgeEntity(const uint_fast32_t& ID){
    deleteEntity(ID);
}
/*******************************************************************************
 * NOTIFY SYSTEMS OF DELETIONS
 ******************************************************************************/
bool ComponentBank::notifySystemsOfDeletions(const uint_fast32_t& ID){
    return true;
}
/*******************************************************************************
 * CONVENIENCE GETTERS SECTION
 ******************************************************************************/
std::string ComponentBank::getName(IDNUM &ID){
    try {
        return components_soul.at(ID).name;
    } catch(const std::out_of_range& oorException) {
        printf("No entity exists with ID %lu.", ID);
        return "";
    }
}
std::string ComponentBank::listComponents(IDNUM &ID){
    COMPFLAG components;
    try {
        components = components_soul.at(ID).components;
    } catch(const std::out_of_range& oorException) {
        printf("No entity exists with ID %lu.", ID);
        return "";
    }
    std::ostringstream output;
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