/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"

using namespace DualityEngine;

/* CONSTRUCTOR */
ComponentBank::ComponentBank(BankDelegates* dlgt){
    nextID = DUA_START_ID;
    this->dlgt = dlgt;
}

ComponentBank::~ComponentBank(){
    dlgt = NULL;
}

/*******************************************************************************
 * BANK MANAGEMENT SECTION
 ******************************************************************************/
void ComponentBank::clean(){
    components_soul.clear();
    components_model.clear();
    components_position.clear();
    components_positionChild.clear();
    components_positionParent.clear();
    components_positionVeloc.clear();
    components_rotation.clear();
    components_rotationVeloc.clear();
    components_control.clear();
    components_pointLight.clear();
    components_directionalLight.clear();
    components_ambientLight.clear();
    components_owner.clear();
    components_score.clear();
}
void ComponentBank::save(const char* saveName){
    
}
void ComponentBank::load(const char* saveName){
    
}


/*******************************************************************************
 * COMPONENT POINTER GETTERS SECTION - I KNOW THESE ARE A BAD IDEA.
 ******************************************************************************/
template<class componentType>
componentType* ComponentBank::getComponentPtr(const DUA_ID &ID, const char* compName, std::unordered_map<DUA_ID, componentType> &table){
    try {
        return &(table.at(ID));
    }  catch(const std::out_of_range& oorException) {
        std::string error = "No " + std::string(compName) + " component exists with ID " + std::to_string(ID) + ".\n";
        dlgt->output(error.c_str());
        return NULL;
    }
}
Soul* ComponentBank::getSoulPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "soul", components_soul);
}
Model* ComponentBank::getModelPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "model", components_model);
}
PositionVeloc* ComponentBank::getPositionVelocPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "linear velocity", components_positionVeloc);
}
Position* ComponentBank::getPositionPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "position", components_position);
}
PositionChild* ComponentBank::getPositionChildPtr(const DUA_ID& ID){
    return getComponentPtr(ID, "position child", components_positionChild);
}
PositionParent* ComponentBank::getPositionParentPtr(const DUA_ID& ID){
    return getComponentPtr(ID, "position parent", components_positionParent);
}
Rotation* ComponentBank::getRotationPtr(const DUA_ID& ID){
    return getComponentPtr(ID, "rotation", components_rotation);
}
RotationVeloc* ComponentBank::getRotationVelocPtr(const DUA_ID& ID){
    return getComponentPtr(ID, "angular velocity", components_rotationVeloc);
}
Control* ComponentBank::getControlPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "control", components_control);
}
PointLight* ComponentBank::getPointLightPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "point light", components_pointLight);
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "directional light", components_directionalLight);
}
AmbientLight* ComponentBank::getAmbientLightPtr(const DUA_ID &ID){
    return getComponentPtr(ID, "ambient light", components_ambientLight);
}
Owner* ComponentBank::getOwnerPtr(const DUA_ID& ID){
    return getComponentPtr(ID, "owner", components_owner);
}
Score* ComponentBank::getScorePtr(const DUA_ID& ID){
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
bool ComponentBank::tryAddFlagToSoul(const DUA_COMPFLAG &flag, const DUA_ID &ID){
    try{
        components_soul.at(ID).components |= flag;
    } catch(const std::out_of_range& oorException) {
        std::string error = "Could not add component at ID " + std::to_string(ID) + ": no soul exists at that ID.\n";
        dlgt->output(error.c_str());
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
bool ComponentBank::tryAddComponent(const DUA_ID &ID, const char* compName, std::unordered_map<DUA_ID, componentType> &table, const types& ... args){
    if ((table.emplace(std::piecewise_construct, std::forward_as_tuple(ID), std::forward_as_tuple(args...))).second == false){
        std::string error = std::string(compName) + " component already exists at ID " + std::to_string(ID) + "\n";
        dlgt->output(error.c_str());
        return false;
    }
    dlgt->systemsDiscover(ID);
    return true;
}
/*******************************************************************************
 * ADD SOUL
 * is a little different than the rest below because it doesn't need to add any
 * flags to a soul component (for obvious reasons). You won't need to call it
 * anyway - it's private and wrapped into "createEntity."
 ******************************************************************************/
bool ComponentBank::addSoul(const DUA_ID &ID, const char* name){
    return tryAddComponent(ID, "soul", components_soul, name, DUA_DEFAULT_COMPONENTS, DUA_DEFAULT_STATE);
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
void ComponentBank::addModel(const DUA_ID &ID){
    if (tryAddFlagToSoul(MODEL, ID))
        tryAddComponent(ID, "model", components_model);
}
void ComponentBank::addPositionVeloc(const DUA_ID &ID, const DUA_DBL &velX, const DUA_DBL &velY, const DUA_DBL &velZ){
    if (tryAddFlagToSoul(LINVELOC, ID))
        tryAddComponent(ID, "linear velocity", components_positionVeloc, velX, velY, velZ);
}
void ComponentBank::addPosition(const DUA_ID &ID, const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ){
    if (tryAddFlagToSoul(POSITION, ID))
        tryAddComponent(ID, "position", components_position, posX, posY, posZ);
}
void ComponentBank::addPositionChild(const DUA_ID& ID, const DUA_ID& refID){
    if (tryAddFlagToSoul(POSCHILD, ID))
        tryAddComponent(ID, "position child", components_positionChild, refID);
}
void ComponentBank::addPositionParent(const DUA_ID& ID, const DUA_ID& refID){
    if (tryAddFlagToSoul(POSPARENT, ID))
        tryAddComponent(ID, "position parent", components_positionParent, refID);
}
void ComponentBank::addRotation(const DUA_ID& ID, const DUA_DBL& rotX, const DUA_DBL& rotY, const DUA_DBL& rotZ){
    if (tryAddFlagToSoul(ROTATION, ID))
        tryAddComponent(ID, "rotation", components_rotation, rotX, rotY, rotZ);
}
void ComponentBank::addRotationVeloc(const DUA_ID& ID, const DUA_DBL& angX, const DUA_DBL& angY, const DUA_DBL& angZ){
    if (tryAddFlagToSoul(ANGVELOC, ID))
        tryAddComponent(ID, "angular velocity", components_rotationVeloc, angX, angY, angZ);
}
void ComponentBank::addControl(const DUA_ID &ID){
    if (tryAddFlagToSoul(CONTROL, ID))
        tryAddComponent(ID, "control", components_control);
}
void ComponentBank::addPointLight(const DUA_ID &ID, const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue,
                                  const DUA_DBL &posX, const DUA_DBL &posY, const DUA_DBL &posZ){
    if (tryAddFlagToSoul(LPOINT, ID))
        tryAddComponent(ID, "point light", components_pointLight, red, green, blue, posX, posY, posZ);
}
void ComponentBank::addDirectionalLight(const DUA_ID &ID, const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue,
                                        const DUA_DBL &rotX, const DUA_DBL &rotY, const DUA_DBL &rotZ){
    if (tryAddFlagToSoul(LDIRECT, ID))
        tryAddComponent(ID, "directional light", components_directionalLight, red, green, blue, rotX, rotY, rotZ);
}
void ComponentBank::addAmbientLight(const DUA_ID &ID, const DUA_COLORBYTE &red, const DUA_COLORBYTE &green, const DUA_COLORBYTE &blue){    
    if (tryAddFlagToSoul(LAMBIENT, ID))
        tryAddComponent(ID, "ambient light", components_ambientLight, red, green, blue);
}
void ComponentBank::addOwner(const DUA_ID& ID, const DUA_ID& refID){
    if (tryAddFlagToSoul(OWNER, ID))
        tryAddComponent(ID, "owner", components_owner, refID);
}
void ComponentBank::addScore(const DUA_ID& ID){
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
void ComponentBank::tryRemoveFlagFromSoul(const DUA_COMPFLAG &flag, const DUA_ID &ID){
    try{
        components_soul.at(ID).components &= ~flag;
    } catch(const std::out_of_range& oorException) {
        std::string error = "Could not remove component at ID " + std::to_string(ID) + ": no soul exists at that ID.\n";
        dlgt->output(error.c_str());
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
bool ComponentBank::tryRemoveComponent(const DUA_ID &ID, const char* compName, std::unordered_map<DUA_ID, componentType> &table){
    if (table.erase(ID) == 0){
        std::string error = "No " + std::string(compName) + " component exists to be removed at ID " + std::to_string(ID) + "\n";
        dlgt->output(error.c_str());
        return false;
    }
    dlgt->systemsScrutinize(ID);
    return true;
}
/*******************************************************************************
 * DELETE SOUL
 * just like the below functions, except doesn't need to clean any flags.
 * it's private, and wrapped by "deleteEntity," so don't worry about it.
 ******************************************************************************/
bool ComponentBank::deleteSoul(const DUA_ID &ID){
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
void ComponentBank::deleteModel(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "model", components_model))
        tryRemoveFlagFromSoul(MODEL, ID);
}
void ComponentBank::deletePositionVeloc(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "linear velocity", components_positionVeloc))
        tryRemoveFlagFromSoul(LINVELOC, ID);
}
void ComponentBank::deletePosition(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "position", components_position))
        tryRemoveFlagFromSoul(POSITION, ID);
}
void ComponentBank::deletePositionChild(const DUA_ID& ID){
    if (tryRemoveComponent(ID, "position child", components_positionChild))
        tryRemoveFlagFromSoul(POSCHILD, ID);
}
void ComponentBank::deletePositionParent(const DUA_ID& ID){
    if (tryRemoveComponent(ID, "position parent", components_positionParent))
        tryRemoveFlagFromSoul(POSPARENT, ID);
}
void ComponentBank::deleteRotation(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "rotation", components_rotation))
        tryRemoveFlagFromSoul(ROTATION, ID);
}
void ComponentBank::deleteRotationVeloc(const DUA_ID& ID){
    if (tryRemoveComponent(ID, "angular velocity", components_rotationVeloc))
        tryRemoveFlagFromSoul(ANGVELOC, ID);
}
void ComponentBank::deleteControl(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "control", components_control))
        tryRemoveFlagFromSoul(CONTROL, ID);
}
void ComponentBank::deletePointLight(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "point light", components_pointLight))
        tryRemoveFlagFromSoul(LPOINT, ID);
}
void ComponentBank::deleteDirectionalLight(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "directional light", components_directionalLight))
        tryRemoveFlagFromSoul(LDIRECT, ID);
}
void ComponentBank::deleteAmbientLight(const DUA_ID &ID){
    if (tryRemoveComponent(ID, "ambient light", components_ambientLight))
        tryRemoveFlagFromSoul(LAMBIENT, ID);
}
void ComponentBank::deleteOwner(const DUA_ID& ID){
    if (tryRemoveComponent(ID, "owner", components_owner))
        tryRemoveFlagFromSoul(OWNER, ID);
}
void ComponentBank::deleteScore(const DUA_ID& ID){
    if (tryRemoveComponent(ID, "score", components_score))
        tryRemoveFlagFromSoul(SCORE, ID);
}

/*******************************************************************************
 * ENTITY CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * GENERATE ID
 ******************************************************************************/
DUA_ID ComponentBank::generateID(){
    return nextID++;
}
/*******************************************************************************
 * CREATE ENTITY
 ******************************************************************************/
DUA_ID ComponentBank::createEntity(const char* name){
    DUA_ID ID = generateID();
    if (addSoul(ID, name))
        return ID;
    else {
        std::string error = "New entity " + std::string(name) + " not created!\n";
        dlgt->output(error.c_str());
        return DUA_NULL_ID;        
    }    
}

/*******************************************************************************
 * ENTITY DELETION SECTION
 ******************************************************************************/
/*******************************************************************************
 * DELETE ENTITY
 * first attempts to remove all non-soul components with ID 'ID', then
 * REMOVES THE ENTITY'S SOUL!!!! <- this is the main reason I named it "soul."
 ******************************************************************************/
bool ComponentBank::deleteEntity(const DUA_ID& ID){
    DUA_COMPFLAG flags;
    std::string name;
    try {
        flags = components_soul.at(ID).components;
        name = components_soul.at(ID).name;
    } catch(const std::out_of_range& oorException) {
        std::string error = "Could not delete entity " + std::to_string(ID) + ": no soul exists at that ID.\n";
        dlgt->output(error.c_str());
        return false;
    }
    
    try {        
        if (flags & MODEL) deleteModel(ID);
        if (flags & LINVELOC) deletePositionVeloc(ID);
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
        std::string error = "Something went wrong upon attempting to delete entity " + std::to_string(ID) + " ('" + name + "')!\n";
        dlgt->output(error.c_str());
        return false;
    }
    
    std::string error = "Entity " + std::to_string(ID) + " ('" + name + "') has been deleted.\n";
    dlgt->output(error.c_str());
    return true;
}
/*******************************************************************************
 * PURGE ENTITY
 * this should not only delete the entity, but also delete any entities that are
 * spatial children of it.
 ******************************************************************************/
bool ComponentBank::purgeEntity(const DUA_ID& ID){
    deleteEntity(ID);
}

/*******************************************************************************
 * CONVENIENCE STRING GETTERS SECTION
 ******************************************************************************/
/*******************************************************************************
 * GET NAME
 * returns a statement string containing the name of the entity (if any) at ID.
 ******************************************************************************/
std::string ComponentBank::getName(DUA_ID &ID){
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
std::string ComponentBank::listComponents(DUA_ID &ID){
    std::ostringstream output;    
    try {        
        DUA_COMPFLAG components = components_soul.at(ID).components;
        
        if (components == DUA_DEFAULT_COMPONENTS){
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
            if (components & LINVELOC)
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