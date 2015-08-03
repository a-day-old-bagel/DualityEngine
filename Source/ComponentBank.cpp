/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/ComponentBank.h"
#include "../Headers/HashMap.h"
#include "ControlBase.h"

using namespace DualityEngine;

/* CONSTRUCTOR */
ComponentBank::ComponentBank(BankDelegates* dlgt){
    nextID = DUA_START_ID;
    this->dlgt = dlgt;
    
    pSpaceControlDummy = new SpaceControl(0, 0, 0, 0, 0, 0, 0, 0, 0);
    pCtrlLinVelocDummy = new LinearVelocity(0, 0, 0);
    pCtrlOrientDummy = new Orientation(0, 0, 0);
    pCtrlAngVelocDummy = new AngularVelocity(0, 0, 0);
    defocusSpaceControl();
    
    pFreeCameraDummy = new CameraFree(1, 0.5, 1000, 0, 0, 0, 0, 0, -1, 0, 1, 0);
    defocusCam();
}

ComponentBank::~ComponentBank(){
    dlgt = NULL;
    
    delete pSpaceControlDummy;
    delete pCtrlLinVelocDummy;
    delete pCtrlOrientDummy;
    delete pCtrlAngVelocDummy;
    pSpaceControlDummy = NULL;
    pCtrlLinVelocDummy = NULL;
    pCtrlOrientDummy = NULL;
    pCtrlAngVelocDummy = NULL;
    
    delete pFreeCameraDummy;
    pFreeCameraDummy = NULL;
}

/*******************************************************************************
 * BANK MANAGEMENT SECTION
 ******************************************************************************/
void ComponentBank::clean(){
    
    activeFreeCameraID = DUA_NULL_ID;
    activeControlID = DUA_NULL_ID;
    
    components_soul.clear();
    components_model.clear();
    components_position.clear();
    components_spatialChild.clear();
    components_spatialParent.clear();
    components_linearVeloc.clear();
    components_orientation.clear();
    components_angularVeloc.clear();
    components_spacecontrol.clear();
    components_pointLight.clear();
    components_directionalLight.clear();
    components_ambientLight.clear();
    components_owner.clear();
    components_score.clear();
    components_collision.clear();
    components_freeCam.clear();
    
    nextID = DUA_START_ID;
}
void ComponentBank::save(const char* saveName){
    
}
void ComponentBank::load(const char* saveName){
    
}


/*******************************************************************************
 * COMPONENT POINTER GETTERS SECTION - I KNOW THESE ARE A BAD IDEA.
 ******************************************************************************/
template<class componentType>
componentType* ComponentBank::getComponentPtr(const DUA_id &ID, const char* compName, HashMap<DUA_id, componentType> &table){
    try {
        return &(table.at(ID));
    }  catch(const std::out_of_range& oorException) {
        std::string error = "No " + std::string(compName) + " component exists with ID " + std::to_string(ID) + ".\n";
        dlgt->output(error.c_str());
        return NULL;
    }
}

Model* ComponentBank::getModelPtr(const DUA_id &ID){
    return getComponentPtr(ID, "model", components_model);
}
LinearVelocity* ComponentBank::getLinearVelocPtr(const DUA_id &ID){
    return getComponentPtr(ID, "linear velocity", components_linearVeloc);
}
Position* ComponentBank::getPositionPtr(const DUA_id &ID){
    return getComponentPtr(ID, "position", components_position);
}
SpatialChild* ComponentBank::getSpatialChildPtr(const DUA_id& ID){
    return getComponentPtr(ID, "position child", components_spatialChild);
}
SpatialParent* ComponentBank::getSpatialParentPtr(const DUA_id& ID){
    return getComponentPtr(ID, "position parent", components_spatialParent);
}
Orientation* ComponentBank::getOrientationPtr(const DUA_id& ID){
    return getComponentPtr(ID, "rotation", components_orientation);
}
AngularVelocity* ComponentBank::getAngularVelocPtr(const DUA_id& ID){
    return getComponentPtr(ID, "angular velocity", components_angularVeloc);
}
SpaceControl* ComponentBank::getSpaceControlPtr(const DUA_id &ID){
    return getComponentPtr(ID, "control", components_spacecontrol);
}
PointLight* ComponentBank::getPointLightPtr(const DUA_id &ID){
    return getComponentPtr(ID, "point light", components_pointLight);
}
DirectionalLight* ComponentBank::getDirectionalLightPtr(const DUA_id &ID){
    return getComponentPtr(ID, "directional light", components_directionalLight);
}
AmbientLight* ComponentBank::getAmbientLightPtr(const DUA_id &ID){
    return getComponentPtr(ID, "ambient light", components_ambientLight);
}
Owner* ComponentBank::getOwnerPtr(const DUA_id& ID){
    return getComponentPtr(ID, "owner", components_owner);
}
Score* ComponentBank::getScorePtr(const DUA_id& ID){
    return getComponentPtr(ID, "score", components_score);
}
Collision* ComponentBank::getCollisionPtr(const DUA_id& ID){
    return getComponentPtr(ID, "collision", components_collision);
}
CameraFree* ComponentBank::getCameraFreePtr(const DUA_id& ID){
    return getComponentPtr(ID, "free camera", components_freeCam);
}
    
/*******************************************************************************
 * COMPONENT CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * TRY ADD FLAG TO SOUL
 * if soul component exists at given ID, given component bit flag is OR'ed to
 * that soul's component flags.  Otherwise an error message is output.
 ******************************************************************************/
bool ComponentBank::tryAddFlagToSoul(const DUA_compFlag &flag, const DUA_id &ID){
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
bool ComponentBank::tryAddComponent(const DUA_id &ID, const char* compName, HashMap<DUA_id, componentType> &table, const types& ... args){
    if ((table.emplace(std::piecewise_construct, std::forward_as_tuple(ID), std::forward_as_tuple(args...))) == false){
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
bool ComponentBank::addSoul(const DUA_id &ID, const char* name){
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
void ComponentBank::addModel(const DUA_id &ID, const char* fileName){
    if (tryAddFlagToSoul(MODEL, ID))
        tryAddComponent(ID, "model", components_model, fileName);
}
void ComponentBank::addLinearVeloc(const DUA_id &ID, const DUA_dbl &velX, const DUA_dbl &velY, const DUA_dbl &velZ){
    if (tryAddFlagToSoul(LINVELOC, ID))
        tryAddComponent(ID, "linear velocity", components_linearVeloc, velX, velY, velZ);
}
void ComponentBank::addPosition(const DUA_id &ID, const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ){
    if (tryAddFlagToSoul(POSITION, ID))
        tryAddComponent(ID, "position", components_position, posX, posY, posZ);
}
void ComponentBank::addSpatialChild(const DUA_id& ID, const DUA_id& refID){
    if (tryAddFlagToSoul(SPATCHILD, ID))
        tryAddComponent(ID, "position child", components_spatialChild, refID);
}
void ComponentBank::addSpatialParent(const DUA_id& ID, const DUA_id& refID){
    if (tryAddFlagToSoul(SPATPARENT, ID))
        tryAddComponent(ID, "position parent", components_spatialParent, refID);
}
void ComponentBank::addOrientation(const DUA_id& ID, const DUA_dbl& rotX, const DUA_dbl& rotY, const DUA_dbl& rotZ){
    if (tryAddFlagToSoul(ORIENTATION, ID))
        tryAddComponent(ID, "rotation", components_orientation, rotX, rotY, rotZ);
}
void ComponentBank::addAngularVeloc(const DUA_id& ID, const DUA_dbl& angX, const DUA_dbl& angY, const DUA_dbl& angZ){
    if (tryAddFlagToSoul(ANGVELOC, ID))
        tryAddComponent(ID, "angular velocity", components_angularVeloc, angX, angY, angZ);
}
void ComponentBank::addSpaceControl(const DUA_id &ID, const DUA_dbl& fw, const DUA_dbl& bk, const DUA_dbl& lf, const DUA_dbl& rt, const DUA_dbl& up, const DUA_dbl& dn, const DUA_dbl& roll, const DUA_dbl& pitch, const DUA_dbl& yaw){
    if (tryAddFlagToSoul(CONTROLSS, ID))
        tryAddComponent(ID, "control", components_spacecontrol, fw, bk, lf, rt, up, dn, roll, pitch, yaw);
}
void ComponentBank::addPointLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue,
                                  const DUA_dbl &posX, const DUA_dbl &posY, const DUA_dbl &posZ){
    if (tryAddFlagToSoul(LPOINT, ID))
        tryAddComponent(ID, "point light", components_pointLight, red, green, blue, posX, posY, posZ);
}
void ComponentBank::addDirectionalLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue,
                                        const DUA_dbl &rotX, const DUA_dbl &rotY, const DUA_dbl &rotZ){
    if (tryAddFlagToSoul(LDIRECT, ID))
        tryAddComponent(ID, "directional light", components_directionalLight, red, green, blue, rotX, rotY, rotZ);
}
void ComponentBank::addAmbientLight(const DUA_id &ID, const DUA_colorByte &red, const DUA_colorByte &green, const DUA_colorByte &blue){    
    if (tryAddFlagToSoul(LAMBIENT, ID))
        tryAddComponent(ID, "ambient light", components_ambientLight, red, green, blue);
}
void ComponentBank::addOwner(const DUA_id& ID, const DUA_id& refID){
    if (tryAddFlagToSoul(OWNER, ID))
        tryAddComponent(ID, "owner", components_owner, refID);
}
void ComponentBank::addScore(const DUA_id& ID){
    if (tryAddFlagToSoul(SCORE, ID))
        tryAddComponent(ID, "score", components_score);
}
void ComponentBank::addCollision(const DUA_id& ID){
    if (tryAddFlagToSoul(COLLISION, ID))
        tryAddComponent(ID, "collision", components_collision);
}
void ComponentBank::addCameraFree(const DUA_id& ID, DUA_float fov, DUA_float zNear, DUA_float zFar, DUA_dbl eyeX, DUA_dbl eyeY, DUA_dbl eyeZ, DUA_dbl focusX, DUA_dbl focusY, DUA_dbl focusZ, DUA_dbl upX, DUA_dbl upY, DUA_dbl upZ){
    if (tryAddFlagToSoul(FREECAM, ID)){
        if (tryAddComponent(ID, "free camera", components_freeCam, fov, zNear, zFar, eyeX, eyeY, eyeZ, focusX, focusY, focusZ, upX, upY, upZ)){
            components_soul.at(ID).state |= RECALCVIEWMAT | RECALCPROJMAT;
        }
    }        
}

/*******************************************************************************
 * COMPONENT DELETION SECTION
 ******************************************************************************/
/*******************************************************************************
 * TRY REMOVE FLAG FROM SOUL
 * tries to AND a NOT-flag onto the component flags of the soul at ID.
 * if there isn't a soul there, it outputs an error message.
 ******************************************************************************/
void ComponentBank::tryRemoveFlagFromSoul(const DUA_compFlag &flag, const DUA_id &ID){
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
bool ComponentBank::tryRemoveComponent(const DUA_id &ID, const char* compName, const DUA_compFlag& compFlag, HashMap<DUA_id, componentType> &table){
    dlgt->systemsForceRemove(ID, compFlag);
    if (table.erase(ID) == 0){
        std::string error = "No " + std::string(compName) + " component exists to be removed at ID " + std::to_string(ID) + "\n";
        dlgt->output(error.c_str());
        return false;
    }
    return true;
}
/*******************************************************************************
 * DELETE SOUL
 * just like the below functions, except doesn't need to clean any flags.
 * it's private, and wrapped by "deleteEntity," so don't worry about it.
 ******************************************************************************/
bool ComponentBank::deleteSoul(const DUA_id &ID){
    return tryRemoveComponent(ID, "soul", 0, components_soul);
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
void ComponentBank::deleteModel(const DUA_id &ID){
    if (tryRemoveComponent(ID, "model", MODEL, components_model))
        tryRemoveFlagFromSoul(MODEL, ID);
}
void ComponentBank::deleteLinearVeloc(const DUA_id &ID){
    scrutinizeControl(ID, ControlTypes::SPACE);
    if (tryRemoveComponent(ID, "linear velocity", LINVELOC, components_linearVeloc))
        tryRemoveFlagFromSoul(LINVELOC, ID);
}
void ComponentBank::deletePosition(const DUA_id &ID){
    scrutinizeCam(ID);
    if (tryRemoveComponent(ID, "position", POSITION, components_position))
        tryRemoveFlagFromSoul(POSITION, ID);
}
void ComponentBank::deleteSpatialChild(const DUA_id& ID){
    if (tryRemoveComponent(ID, "position child", SPATCHILD, components_spatialChild))
        tryRemoveFlagFromSoul(SPATCHILD, ID);
}
void ComponentBank::deleteSpatialParent(const DUA_id& ID){
    if (tryRemoveComponent(ID, "position parent", SPATPARENT, components_spatialParent))
        tryRemoveFlagFromSoul(SPATPARENT, ID);
}
void ComponentBank::deleteOrientation(const DUA_id &ID){
    scrutinizeCam(ID);
    scrutinizeControl(ID, ControlTypes::SPACE);
    if (tryRemoveComponent(ID, "rotation", ORIENTATION, components_orientation))
        tryRemoveFlagFromSoul(ORIENTATION, ID);
}
void ComponentBank::deleteAngularVeloc(const DUA_id& ID){
    scrutinizeControl(ID, ControlTypes::SPACE);
    if (tryRemoveComponent(ID, "angular velocity", ANGVELOC, components_angularVeloc))
        tryRemoveFlagFromSoul(ANGVELOC, ID);
}
void ComponentBank::deleteSpaceControl(const DUA_id &ID){
    scrutinizeControl(ID, ControlTypes::SPACE);
    if (tryRemoveComponent(ID, "control", CONTROLSS, components_spacecontrol))
        tryRemoveFlagFromSoul(CONTROLSS, ID);
}
void ComponentBank::deletePointLight(const DUA_id &ID){
    if (tryRemoveComponent(ID, "point light", LPOINT, components_pointLight))
        tryRemoveFlagFromSoul(LPOINT, ID);
}
void ComponentBank::deleteDirectionalLight(const DUA_id &ID){
    if (tryRemoveComponent(ID, "directional light", LDIRECT, components_directionalLight))
        tryRemoveFlagFromSoul(LDIRECT, ID);
}
void ComponentBank::deleteAmbientLight(const DUA_id &ID){
    if (tryRemoveComponent(ID, "ambient light", LAMBIENT, components_ambientLight))
        tryRemoveFlagFromSoul(LAMBIENT, ID);
}
void ComponentBank::deleteOwner(const DUA_id& ID){
    if (tryRemoveComponent(ID, "owner", OWNER, components_owner))
        tryRemoveFlagFromSoul(OWNER, ID);
}
void ComponentBank::deleteScore(const DUA_id& ID){
    if (tryRemoveComponent(ID, "score", SCORE, components_score))
        tryRemoveFlagFromSoul(SCORE, ID);
}
void ComponentBank::deleteCollision(const DUA_id& ID){
    if (tryRemoveComponent(ID, "collision", COLLISION, components_collision))
        tryRemoveFlagFromSoul(COLLISION, ID);
}
void ComponentBank::deleteCameraFree(const DUA_id& ID){
    scrutinizeCam(ID);
    if (tryRemoveComponent(ID, "free camera", FREECAM, components_freeCam))
        tryRemoveFlagFromSoul(FREECAM, ID);
}

/*******************************************************************************
 * ENTITY STATE GETTERS / SETTERS SECTION
 ******************************************************************************/
DUA_compFlag ComponentBank::getComponents(const DUA_id& ID){
    DUA_compFlag flags;
    try {
        return components_soul.at(ID).components;
    } catch(const std::out_of_range& oorException) {
        return DUA_INVALID_COMPONENTS;
    }
}
DUA_stateFlag ComponentBank::getState(const DUA_id& ID){
    DUA_stateFlag flags;
    try {
        return components_soul.at(ID).state;
    } catch(const std::out_of_range& oorException) {
        return DUA_INVALID_STATE;
    }
}
void ComponentBank::stateOn(const DUA_id& ID, const DUA_stateFlag& flag){
    try{
        components_soul.at(ID).state |= flag;
    }catch (const std::out_of_range& oorException) {
        dlgt->output("WARNING: Bad stateOn attempt: No such ID.");
    }
}
void ComponentBank::stateOff(const DUA_id& ID, const DUA_stateFlag& flag){
    try{
        components_soul.at(ID).state &= ~flag;
    }catch (const std::out_of_range& oorException) {
        dlgt->output("WARNING: Bad stateOff attempt: No such ID.");
    }
}

/*******************************************************************************
 * ENTITY CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * GENERATE ID
 ******************************************************************************/
DUA_id ComponentBank::generateID(){
    return nextID++;
}
/*******************************************************************************
 * CREATE ENTITY
 ******************************************************************************/
DUA_id ComponentBank::createEntity(const char* name){
    DUA_id ID = generateID();
    if (addSoul(ID, name)){
        return ID;
    } else {
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
bool ComponentBank::deleteEntity(const DUA_id& ID){
    DUA_compFlag flags;
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
        if (flags & LINVELOC) deleteLinearVeloc(ID);
        if (flags & POSITION) deletePosition(ID);
        if (flags & SPATCHILD) deleteSpatialChild(ID);
        if (flags & SPATPARENT) deleteSpatialParent(ID);
        if (flags & CONTROLSS) deleteSpaceControl(ID);
        if (flags & LPOINT) deletePointLight(ID);
        if (flags & LDIRECT) deleteDirectionalLight(ID);
        if (flags & LAMBIENT) deleteAmbientLight(ID);
        if (flags & OWNER) deleteOwner(ID);
        if (flags & SCORE) deleteScore(ID);
        if (flags & ORIENTATION) deleteOrientation(ID);
        if (flags & ANGVELOC) deleteAngularVeloc(ID);
        if (flags & COLLISION) deleteCollision(ID);
        if (flags & FREECAM) deleteCameraFree(ID);

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
 * PURGE ENTITY [NOT IMPLEMENTED YET]
 * this should not only delete the entity, but also delete any entities that are
 * spatial children of it.
 ******************************************************************************/
bool ComponentBank::purgeEntity(const DUA_id& ID){
    deleteEntity(ID);
}

/*******************************************************************************
 * CONVENIENCE STRING GETTERS SECTION
 ******************************************************************************/
bool ComponentBank::getIDs(std::string& name, std::vector<DUA_id>& IDs){
    try {
        for (auto pair : components_soul){
            if (pair.second.name == name){
                IDs.push_back(pair.first);
            }
        }
        return true;
    } catch(const std::out_of_range& oorException) {
        return false;
    }
}
/*******************************************************************************
 * GET NAME
 * returns a statement string containing the name of the entity (if any) at ID.
 ******************************************************************************/
std::string ComponentBank::getNameVerbose(const DUA_id &ID){
    std::ostringstream output;
    try {
        output << "Entity " << ID << " is named '" << components_soul.at(ID).name << "'.";
    } catch(const std::out_of_range& oorException) {
        output << "Could not get name: No entity exists with ID " << ID;
    }
    return output.str();
}
std::string ComponentBank::getName(const DUA_id& ID){
    try{
        return components_soul.at(ID).name;
    }catch(const std::out_of_range& oorException) {
        return "Could not get name: No entity exists with ID " + std::to_string(ID);
    }
}
/*******************************************************************************
 * LIST COMPONENTS
 * returns a statement string containing a list of components currently
 * possessed by the entity at ID.
 ******************************************************************************/
std::string ComponentBank::listComponentsVerbose(const DUA_id &ID){
    std::ostringstream output;    
    try {        
        DUA_compFlag components = components_soul.at(ID).components;
        
        if (components == DUA_DEFAULT_COMPONENTS){
            output << "Entity " << getEntityInfo(ID) << " is a disembodied soul.";      
        } else {
                        
            output << "Entity " << getEntityInfo(ID) << " has: " << listComponents(components);
            
        }
    } catch(const std::out_of_range& oorException) {
        output << "Could not list components: No entity exists with ID " << ID;
    }    
    return output.str();
}
std::string ComponentBank::listComponents(const DUA_compFlag& flag){
    std::ostringstream output;
    for (auto compType : componentCollections){
        if (flag & std::get<2>(compType)){
            output << std::get<0>(compType) << ", ";
        }
    }
    return output.str();
}

std::string ComponentBank::getEntityInfo(const DUA_id& ID){
    try{
        return (std::to_string(ID) + " (" + components_soul.at(ID).name + ")");
    }catch(const std::out_of_range& oorException){
        return (std::to_string(ID) + " (DOES NOT EXIST)");
    }
}




/*******************************************************************************
 * CONVENIENCE CONSTRUCTS METHODS
 ******************************************************************************/


bool ComponentBank::updateActiveCamera(){
    if (activeFreeCameraID != DUA_NULL_ID){
        if (getState(activeFreeCameraID) & RECALCVIEWMAT){            
            pFreeCameraCurrent->updateView(getRotMat(activeFreeCameraID), getPosMat(activeFreeCameraID));            
        }
        if (getState(activeFreeCameraID) & RECALCPROJMAT){
            pFreeCameraCurrent->updateProjection();
        }
        if (getState(activeFreeCameraID) & (RECALCVIEWMAT | RECALCPROJMAT)){
            pFreeCameraCurrent->updateViewProjection();
            stateOff(activeFreeCameraID, RECALCVIEWMAT | RECALCPROJMAT);
        }
        return true;
    } else {
        return false;
    }
}

bool ComponentBank::switchToCam(const DUA_id& ID){
    try{
        if ((components_soul.at(ID).components) & FREECAM){
            if((components_soul.at(ID).components) & (POSITION | ORIENTATION) == (POSITION | ORIENTATION)){
                activeFreeCameraID = ID;
                pFreeCameraCurrent = getCameraFreePtr(ID);
            } else {
                dlgt->outputStr("entity " + getEntityInfo(ID) + " requires position and orientation.");
                return false;
            }
        } else {
            dlgt->outputStr("entity " + getEntityInfo(ID) + " doesn't have a camera.");
            return false;
        }
    }catch(const std::out_of_range& oorException){
        dlgt->outputStr("No entity exists at ID " + std::to_string(ID) + "\n");
        return false;
    }
    return true;
}
void ComponentBank::scrutinizeCam(const DUA_id& ID){
    if (activeFreeCameraID == ID){
        defocusCam();
        dlgt->outputStr("View from entity " + getEntityInfo(ID) + " has been lost.");
    }
}
void ComponentBank::defocusCam(){
    activeFreeCameraID = DUA_NULL_ID;
    pFreeCameraCurrent = pFreeCameraDummy;
}

bool ComponentBank::switchToControl(const DUA_id& ID, ControlTypes::type controlType){
    DUA_compFlag requiredControlComponent;
    DUA_compFlag requiredOtherComponents;
    Delegate<void()> defocusControlCandidate;
    Delegate<void(const DUA_id&)> focusControl;
    switch(controlType){
        case ControlTypes::SPACE:
            requiredControlComponent = CONTROLSS;
            requiredOtherComponents = ORIENTATION | LINVELOC | ANGVELOC;
            focusControl = DELEGATE(&ComponentBank::focusSpaceControl, this);
            defocusControlCandidate = DELEGATE(&ComponentBank::defocusSpaceControl, this);
            break;
        case ControlTypes::NONE:
            activeControlID = DUA_NULL_ID;
            defocusSpaceControl();
            //defualt other controls()...
            currentControlType = ControlTypes::NONE;
            dlgt->output("Control focus discarded.");
            return true;
        default:
            dlgt->outputStr("Not a control type: " + std::to_string(controlType));
            return false;
    }
    
    try{
        if (getComponents(ID) & requiredControlComponent){
            if((getComponents(ID) & requiredOtherComponents) == requiredOtherComponents){
                activeControlID = ID;
                currentControlType = controlType;
                requiredControlComponents = requiredControlComponent | requiredOtherComponents;
                defocusControl = defocusControlCandidate;                
                focusControl(ID);
                
            } else {
                dlgt->outputStr("entity " + getEntityInfo(ID) + " does not possess one or more of these required components: " + listComponents(requiredOtherComponents));
                return false;
            }
        } else {
            dlgt->outputStr("entity " + getEntityInfo(ID) + " does not possess the required control interface.");
            return false;
        }
    }catch(const std::out_of_range& oorException){
        dlgt->outputStr("No entity exists at ID " + std::to_string(ID));
        return false;
    }
    return true;
}
void ComponentBank::focusSpaceControl(const DUA_id& ID){
    //default other controls()...
    pSpaceControlCurrent = getSpaceControlPtr(ID);
    pCtrlLinVelocCurrent = getLinearVelocPtr(ID);
    pCtrlOrientCurrent = getOrientationPtr(ID);    
}
void ComponentBank::scrutinizeControl(const DUA_id& ID, ControlTypes::type dependentControlType){
    if (currentControlType == dependentControlType){
        if (activeControlID == ID){// && getComponents(activeControlID) & requiredControlComponents != requiredControlComponents){
            dlgt->outputStr("Control of entity" + getEntityInfo(ID) + " has been lost.");
            currentControlType = ControlTypes::NONE;
            activeControlID = DUA_NULL_ID;            
            defocusControl(); 
        }
    }
}
void ComponentBank::defocusSpaceControl(){
    pSpaceControlCurrent = pSpaceControlDummy;
    pCtrlLinVelocCurrent = pCtrlLinVelocDummy;
    pCtrlOrientCurrent = pCtrlOrientDummy;
}

glm::mat4 ComponentBank::getPosMat(const DUA_id& ID){
    try {
        if (getComponents(ID) & POSITION){
            return components_position.at(ID).getMatrix();
        } else {
            return Constants::duaIdentMat4;
        }
    }catch(const std::out_of_range& oorException){
        dlgt->outputStr("ERROR: access to nonexistent position for matrix retrieval at ID " + std::to_string(ID) + "\n");
    }
}
glm::mat4 ComponentBank::getRotMat(const DUA_id& ID){
    try {
        if (getComponents(ID) & ORIENTATION){
            return components_orientation.at(ID).getMatrix();
        } else {
            return Constants::duaIdentMat4;
        }
    }catch(const std::out_of_range& oorException){
        dlgt->outputStr("ERROR: access to nonexistent orientation for matrix retrieval at ID " + std::to_string(ID) + "\n");
    }
}
glm::mat4 ComponentBank::getModMat(const DUA_id& ID){
    return getPosMat(ID) * getRotMat(ID);
}