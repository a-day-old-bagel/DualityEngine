/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

/* CONSTRUCTOR */
ComponentBank::ComponentBank(){
    nextID = DU_START_ID;
}

/* CONSTRUCTOR FOR LOADING SAVED STATES */
ComponentBank::ComponentBank(const DU_ID &startingID){
    nextID = startingID;
}

ComponentBank::~ComponentBank(){
    
}

/*******************************************************************************
 * COMPONENT POINTER GETTERS SECTION - I KNOW THESE ARE A BAD IDEA.
 ******************************************************************************/
template<class componentType>
componentType* getComponentPtr(const DU_ID &ID, const char* compName, std::unordered_map<DU_ID, componentType> &table){
    try {
        return &(table.at(ID));
    }  catch(const std::out_of_range& oorException) {
        printf("No %s component exists with ID %lu.", compName, ID);
        return NULL;
    }
}
Soul* ComponentBank::getSoulPtr(const DU_ID &ID){
    return getComponentPtr(ID, "soul", components_soul);
}
Model* ComponentBank::getModelPtr(const DU_ID &ID){
    return getComponentPtr(ID, "model", components_model);
}
PositionVeloc* ComponentBank::getPositionVelocPtr(const DU_ID &ID){
    return getComponentPtr(ID, "linear velocity", components_positionVeloc);
}
Position* ComponentBank::getPositionPtr(const DU_ID &ID){
    return getComponentPtr(ID, "position", components_position);
}
PositionChild* ComponentBank::getPositionChildPtr(const DU_ID& ID){
    return getComponentPtr(ID, "position child", components_positionChild);
}
PositionParent* ComponentBank::getPositionParentPtr(const DU_ID& ID){
    return getComponentPtr(ID, "position parent", components_positionParent);
}
Rotation* ComponentBank::getRotationPtr(const DU_ID& ID){
    return getComponentPtr(ID, "rotation", components_rotation);
}
RotationVeloc* ComponentBank::getRotationVelocPtr(const DU_ID& ID){
    return getComponentPtr(ID, "angular velocity", components_rotationVeloc);
}
Control* ComponentBank::getControlPtr(const DU_ID &ID){
    return getComponentPtr(ID, "control", components_control);
}
PointLight* ComponentBank::getPointLightPtr(const DU_ID &ID){
    return getComponentPtr(ID, "point light", components_pointLight);
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const DU_ID &ID){
    return getComponentPtr(ID, "directional light", components_directionalLight);
}
AmbientLight* ComponentBank::getAmbientLightPtr(const DU_ID &ID){
    return getComponentPtr(ID, "ambient light", components_ambientLight);
}
Owner* ComponentBank::getOwnerPtr(const DU_ID& ID){
    return getComponentPtr(ID, "owner", components_owner);
}
Score* ComponentBank::getScorePtr(const DU_ID& ID){
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
bool ComponentBank::tryAddFlagToSoul(const DU_COMPFLAG &flag, const DU_ID &ID){
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
bool ComponentBank::tryAddComponent(const DU_ID &ID, const char* compName, std::unordered_map<DU_ID, componentType> &table, const types& ... args){
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
bool ComponentBank::addSoul(const DU_ID &ID, const char* name){
    return tryAddComponent(ID, "soul", components_soul, name, DU_DEFAULT_COMPONENTS, DU_DEFAULT_STATE);
}
/*******************************************************************************
 * ADD [COMPONENT] WRAPPER FUNCTIONS:
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
void ComponentBank::addModel(const DU_ID &ID){
    if (tryAddFlagToSoul(MODEL, ID))
        tryAddComponent(ID, "model", components_model);
}
void ComponentBank::addPositionVeloc(const DU_ID &ID, const DU_FLOAT &velX, const DU_FLOAT &velY, const DU_FLOAT &velZ){
    if (tryAddFlagToSoul(POSVELOC, ID))
        tryAddComponent(ID, "linear velocity", components_positionVeloc, velX, velY, velZ);
}
void ComponentBank::addPosition(const DU_ID &ID, const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ){
    if (tryAddFlagToSoul(POSITION, ID))
        tryAddComponent(ID, "position", components_position, posX, posY, posZ);
}
void ComponentBank::addPositionChild(const DU_ID& ID, const DU_ID& refID){
    if (tryAddFlagToSoul(POSCHILD, ID))
        tryAddComponent(ID, "position child", components_positionChild, refID);
}
void ComponentBank::addPositionParent(const DU_ID& ID, const DU_ID& refID){
    if (tryAddFlagToSoul(POSPARENT, ID))
        tryAddComponent(ID, "position parent", components_positionParent, refID);
}
void ComponentBank::addRotation(const DU_ID& ID, const DU_FLOAT& rotX, const DU_FLOAT& rotY, const DU_FLOAT& rotZ){
    if (tryAddFlagToSoul(ROTATION, ID))
        tryAddComponent(ID, "rotation", components_rotation, rotX, rotY, rotZ);
}
void ComponentBank::addRotationVeloc(const DU_ID& ID, const DU_FLOAT& angX, const DU_FLOAT& angY, const DU_FLOAT& angZ){
    if (tryAddFlagToSoul(ROTVELOC, ID))
        tryAddComponent(ID, "angular velocity", components_rotationVeloc, angX, angY, angZ);
}
void ComponentBank::addControl(const DU_ID &ID){
    if (tryAddFlagToSoul(CONTROL, ID))
        tryAddComponent(ID, "control", components_control);
}
void ComponentBank::addPointLight(const DU_ID &ID, const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue,
                                  const DU_FLOAT &posX, const DU_FLOAT &posY, const DU_FLOAT &posZ){
    if (tryAddFlagToSoul(LPOINT, ID))
        tryAddComponent(ID, "point light", components_pointLight, red, green, blue, posX, posY, posZ);
}
void ComponentBank::addDirectionalLight(const DU_ID &ID, const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue,
                                        const DU_FLOAT &rotX, const DU_FLOAT &rotY, const DU_FLOAT &rotZ){
    if (tryAddFlagToSoul(LDIRECT, ID))
        tryAddComponent(ID, "directional light", components_directionalLight, red, green, blue, rotX, rotY, rotZ);
}
void ComponentBank::addAmbientLight(const DU_ID &ID, const DU_COLORBYTE &red, const DU_COLORBYTE &green, const DU_COLORBYTE &blue){    
    if (tryAddFlagToSoul(LAMBIENT, ID))
        tryAddComponent(ID, "ambient light", components_ambientLight, red, green, blue);
}
void ComponentBank::addOwner(const DU_ID& ID, const DU_ID& refID){
    if (tryAddFlagToSoul(OWNER, ID))
        tryAddComponent(ID, "owner", components_owner, refID);
}
void ComponentBank::addScore(const DU_ID& ID){
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
void ComponentBank::tryRemoveFlagFromSoul(const DU_COMPFLAG &flag, const DU_ID &ID){
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
 * Arguments are pretty much the same as the TRY ADD COMPONENT function outlined
 * above, except there aren't any variadic arguments at the end.
 ******************************************************************************/
template<class componentType>
bool ComponentBank::tryRemoveComponent(const DU_ID &ID, const char* compName, std::unordered_map<DU_ID, componentType> &table){
    if (table.erase(ID) == 0){
        printf("Could not remove %s component at ID %lu: no such component exists.\n", compName, ID);
        return false;
    }
    return true;
}
/*******************************************************************************
 * DELETE SOUL
 * just like the below functions, except doesn't need to clean any flags.
 * it's private, and wrapped by "deleteEntity," so don't worry about it.
 ******************************************************************************/
bool ComponentBank::deleteSoul(const DU_ID &ID){
    return tryRemoveComponent(ID, "soul", components_soul);
}
/*******************************************************************************
 * DELETE [COMPONENT] WRAPPER FUNCTIONS:
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
void ComponentBank::deleteModel(const DU_ID &ID){
    if (tryRemoveComponent(ID, "model", components_model))
        tryRemoveFlagFromSoul(MODEL, ID);
}
void ComponentBank::deletePositionVeloc(const DU_ID &ID){
    if (tryRemoveComponent(ID, "linear velocity", components_positionVeloc))
        tryRemoveFlagFromSoul(POSVELOC, ID);
}
void ComponentBank::deletePosition(const DU_ID &ID){
    if (tryRemoveComponent(ID, "position", components_position))
        tryRemoveFlagFromSoul(POSITION, ID);
}
void ComponentBank::deletePositionChild(const DU_ID& ID){
    if (tryRemoveComponent(ID, "position child", components_positionChild))
        tryRemoveFlagFromSoul(POSCHILD, ID);
}
void ComponentBank::deletePositionParent(const DU_ID& ID){
    if (tryRemoveComponent(ID, "position parent", components_positionParent))
        tryRemoveFlagFromSoul(POSPARENT, ID);
}
void ComponentBank::deleteRotation(const DU_ID &ID){
    if (tryRemoveComponent(ID, "rotation", components_rotation))
        tryRemoveFlagFromSoul(ROTATION, ID);
}
void ComponentBank::deleteRotationVeloc(const DU_ID& ID){
    if (tryRemoveComponent(ID, "angular velocity", components_rotationVeloc))
        tryRemoveFlagFromSoul(ROTVELOC, ID);
}
void ComponentBank::deleteControl(const DU_ID &ID){
    if (tryRemoveComponent(ID, "control", components_control))
        tryRemoveFlagFromSoul(CONTROL, ID);
}
void ComponentBank::deletePointLight(const DU_ID &ID){
    if (tryRemoveComponent(ID, "point light", components_pointLight))
        tryRemoveFlagFromSoul(LPOINT, ID);
}
void ComponentBank::deleteDirectionalLight(const DU_ID &ID){
    if (tryRemoveComponent(ID, "directional light", components_directionalLight))
        tryRemoveFlagFromSoul(LDIRECT, ID);
}
void ComponentBank::deleteAmbientLight(const DU_ID &ID){
    if (tryRemoveComponent(ID, "ambient light", components_ambientLight))
        tryRemoveFlagFromSoul(LAMBIENT, ID);
}
void ComponentBank::deleteOwner(const DU_ID& ID){
    if (tryRemoveComponent(ID, "owner", components_owner))
        tryRemoveFlagFromSoul(OWNER, ID);
}
void ComponentBank::deleteScore(const DU_ID& ID){
    if (tryRemoveComponent(ID, "score", components_score))
        tryRemoveFlagFromSoul(SCORE, ID);
}

/*******************************************************************************
 * ENTITY CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * GENERATE ID
 ******************************************************************************/
DU_ID ComponentBank::generateID(){
    return nextID++;
}
/*******************************************************************************
 * CREATE ENTITY
 ******************************************************************************/
DU_ID ComponentBank::createEntity(const char* name){
    DU_ID ID = generateID();
    if (addSoul(ID, name))
        return ID;
    else {
        printf("New entity '%s' not created!\n", name);
        return DU_NULL_ID;        
    }    
}
/*******************************************************************************
 * NOTIFY SYSTEMS OF ADDITIONS
 ******************************************************************************/
bool ComponentBank::notifySystemsOfAdditions(const DU_ID &ID){
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
bool ComponentBank::deleteEntity(const DU_ID& ID){
    DU_COMPFLAG flags;
    std::string name;
    try {
        flags = components_soul.at(ID).components;
        name = components_soul.at(ID).name;
    } catch(const std::out_of_range& oorException) {
        printf("Could not delete entity: No soul component exists at ID %lu.\n", ID);
        return false;
    }
    
    try {        
        if (flags & MODEL) deleteModel(ID);
        if (flags & POSVELOC) deletePositionVeloc(ID);
        if (flags & POSITION) deletePosition(ID);
        if (flags & POSCHILD) deletePositionChild(ID);
        if (flags & POSPARENT) deletePositionParent(ID);
        if (flags & CONTROL) deleteControl(ID);
        if (flags & LPOINT) deletePointLight(ID);
        if (flags & LDIRECT) deleteDirectionalLight(ID);
        if (flags & LAMBIENT) deleteAmbientLight(ID);
        if (flags & OWNER) deleteOwner(ID);
        if (flags & SCORE) deleteScore(ID);

        deleteSoul(ID);
    
    } catch(...) {
        printf("Something went wrong upon attempting to delete entity %lu ('%s')!\n", ID, name.c_str());
        return false;
    }
    
    printf("Entity %lu ('%s') has been deleted.", ID, name.c_str());
    return true;
}
/*******************************************************************************
 * PURGE ENTITY
 * this should not only delete the entity, but also delete any entities that are
 * spatial children of it.
 ******************************************************************************/
bool ComponentBank::purgeEntity(const DU_ID& ID){
    deleteEntity(ID);
}
/*******************************************************************************
 * NOTIFY SYSTEMS OF DELETIONS
 ******************************************************************************/
bool ComponentBank::notifySystemsOfDeletions(const DU_ID& ID){
    return true;
}
/*******************************************************************************
 * CONVENIENCE STRING GETTERS SECTION
 ******************************************************************************/
/*******************************************************************************
 * GET NAME
 * returns a statement string containing the name of the entity (if any) at ID.
 ******************************************************************************/
std::string ComponentBank::getName(DU_ID &ID){
    std::ostringstream output;
    try {
        output << "Entity " << ID << " is named '" << components_soul.at(ID).name << "'.";
    } catch(const std::out_of_range& oorException) {
        output << "Could not get name: No entity exists with ID " << ID;
    }
    return output.str();
}
/*******************************************************************************
 * LIST COMPONENTS
 * returns a statement string containing a list of components currently
 * possessed by the entity at ID.
 ******************************************************************************/
std::string ComponentBank::listComponents(DU_ID &ID){
    std::ostringstream output;    
    try {        
        DU_COMPFLAG components = components_soul.at(ID).components;
        
        if (components == DU_DEFAULT_COMPONENTS){
            output << "Entity " << ID << " is a disembodied soul.";      
        } else {
            
            output << "Entity " << ID << " has: ";
            if (components & MODEL)
                output << "MODEL ";
            if (components & POSITION)
                output << "SPATIAL ";
            if (components & POSPARENT)
                output << "SPATPARENT ";
            if (components & POSCHILD)
                output << "SPATCHILD ";
            if (components & POSVELOC)
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
            if (components & OWNER)
                output << "OWNER ";
            if (components & SCORE)
                output << "SCORE ";
            
        }
    } catch(const std::out_of_range& oorException) {
        output << "Could not list components: No entity exists with ID " << ID;
    }    
    return output.str();
}