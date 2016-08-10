/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "Bank.h"

namespace DualityEngine {

/* CONSTRUCTOR */
    Bank::Bank(BankDelegates *dlgt) {
        pWindow = NULL;
        nextID = DUA_START_ID;
        this->dlgt = dlgt;

        activeControlID = DUA_NULL_ID;
        requiredControlComponents = 0;
        currentControlType = ControlTypes::NONE;

        pSpaceControlDummy = new SpaceControl(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        pCtrlLinVelocDummy = new LinearVelocity(0, 0, 0);
        pCtrlOrientDummy = new Orientation(0, 0, 0);
        pCtrlAngVelocDummy = new AngularVelocity(0, 0, 0);
        defocusSpaceControl();

        activeFreeCameraID = DUA_NULL_ID;

        pFreeCameraDummy = new CameraFree(1, 0.5, 1000, 0, 0, 0, 0, 0, -1, 0, 1, 0);
        defocusCam();
    }

    Bank::~Bank() {
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
    void Bank::clean() {

        activeFreeCameraID = DUA_NULL_ID;
        activeControlID = DUA_NULL_ID;

        comps_soul.clear();
        comps_model.clear();
        comps_position.clear();
        comps_spatChild.clear();
        comps_spatParent.clear();
        comps_linVeloc.clear();
        comps_orientation.clear();
        comps_angVeloc.clear();
        comps_spaceControl.clear();
        comps_pntLight.clear();
        comps_dirLight.clear();
        comps_ambLight.clear();
        comps_owner.clear();
        comps_score.clear();
        comps_collision.clear();
        comps_freeCam.clear();

        nextID = DUA_START_ID;
    }

    void Bank::save(const char *saveName) {

    }

    void Bank::load(const char *saveName) {

    }


/*******************************************************************************
 * COMPONENT GETTERS SECTION
 ******************************************************************************/
    template<class componentType>
    componentType *Bank::getComponentPtr(const DUA_id &ID, const char *compName,
                                         KvMap<DUA_id, componentType> &table) {
        try {
            return &(table.at(ID));
        } catch (const std::out_of_range &oorException) {
            std::string error =
                    "No " + std::string(compName) + " component exists with ID " + idToStr(ID) + ".\n";
            dlgt->output(error.c_str());
            return NULL;
        }
    }

    Model *Bank::getModelPtr(const DUA_id ID) {
        return getComponentPtr(ID, "model", comps_model);
    }
    LinearVelocity *Bank::getLinearVelocPtr(const DUA_id ID) {
        return getComponentPtr(ID, "linear velocity", comps_linVeloc);
    }
    Position *Bank::getPositionPtr(const DUA_id ID) {
        return getComponentPtr(ID, "position", comps_position);
    }
    SpatialChild *Bank::getSpatialChildPtr(const DUA_id ID) {
        return getComponentPtr(ID, "position child", comps_spatChild);
    }
    SpatialParent *Bank::getSpatialParentPtr(const DUA_id ID) {
        return getComponentPtr(ID, "position parent", comps_spatParent);
    }
    Orientation *Bank::getOrientationPtr(const DUA_id ID) {
        return getComponentPtr(ID, "rotation", comps_orientation);
    }
    AngularVelocity *Bank::getAngularVelocPtr(const DUA_id ID) {
        return getComponentPtr(ID, "angular velocity", comps_angVeloc);
    }
    SpaceControl *Bank::getSpaceControlPtr(const DUA_id ID) {
        return getComponentPtr(ID, "control", comps_spaceControl);
    }
    PointLight *Bank::getPointLightPtr(const DUA_id ID) {
        return getComponentPtr(ID, "point light", comps_pntLight);
    }
    DirectionalLight *Bank::getDirectionalLightPtr(const DUA_id ID) {
        return getComponentPtr(ID, "directional light", comps_dirLight);
    }
    AmbientLight *Bank::getAmbientLightPtr(const DUA_id ID) {
        return getComponentPtr(ID, "ambient light", comps_ambLight);
    }
    Owner *Bank::getOwnerPtr(const DUA_id ID) {
        return getComponentPtr(ID, "owner", comps_owner);
    }
    Score *Bank::getScorePtr(const DUA_id ID) {
        return getComponentPtr(ID, "score", comps_score);
    }
    Collision *Bank::getCollisionPtr(const DUA_id ID) {
        return getComponentPtr(ID, "collision", comps_collision);
    }
    CameraFree *Bank::getCameraFreePtr(const DUA_id ID) {
        return getComponentPtr(ID, "free camera", comps_freeCam);
    }

/*******************************************************************************
 * COMPONENT CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * TRY ADD FLAG TO SOUL
 * if soul component exists at given ID, given component bit flag is OR'ed to
 * that soul's component flags.  Otherwise an error message is output.
 ******************************************************************************/
    bool Bank::tryAddFlagToSoul(const DUA_compFlag &flag, const DUA_id &ID) {
        try {
            comps_soul.at(ID).components |= flag;
        } catch (const std::out_of_range &oorException) {
            std::string error =
                    "Could not add component at ID " + idToStr(ID) + ": no soul exists at that ID.\n";
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
 * [collection to which to add the component, like 'comps_model'],
 *      and now whatever arguments the component in question takes...
 * [arg0], [arg1], ... ,[lastArg]);
 ******************************************************************************/
    template<class componentType, typename ... types>
    bool Bank::tryAddComponent(const DUA_id &ID, const char *compName, KvMap<DUA_id, componentType> &table,
                               const types &... args) {
        if (!(table.emplace(std::piecewise_construct, std::forward_as_tuple(ID), std::forward_as_tuple(args...)))) {
            std::string error = std::string(compName) + " component already exists at ID " + idToStr(ID) + "\n";
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
 * anyway - it's private and wrapped into "spawnEntity."
 ******************************************************************************/
    bool Bank::addSoul(const DUA_id &ID, const char *name) {
        return tryAddComponent(ID, "soul", comps_soul, name, DUA_DEFAULT_COMPONENTS, DUA_DEFAULT_STATE);
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
    void Bank::addModel(const DUA_id ID, const char *fileName) {
        if (tryAddFlagToSoul(MODEL, ID))
            tryAddComponent(ID, "model", comps_model, fileName);
    }

    void Bank::addLinearVeloc(const DUA_id ID, const DUA_dbl velX, const DUA_dbl velY, const DUA_dbl velZ) {
        if (tryAddFlagToSoul(LINVELOC, ID))
            tryAddComponent(ID, "linear velocity", comps_linVeloc, velX, velY, velZ);
    }

    void Bank::addPosition(const DUA_id ID, const DUA_dbl posX, const DUA_dbl posY, const DUA_dbl posZ) {
        if (tryAddFlagToSoul(POSITION, ID))
            tryAddComponent(ID, "position", comps_position, posX, posY, posZ);
    }

    void Bank::addSpatialChild(const DUA_id ID, const DUA_id refID) {
        if (tryAddFlagToSoul(SPATCHILD, ID))
            tryAddComponent(ID, "position child", comps_spatChild, refID);
    }

    void Bank::addSpatialParent(const DUA_id ID, const DUA_id refID) {
        if (tryAddFlagToSoul(SPATPARENT, ID))
            tryAddComponent(ID, "position parent", comps_spatParent, refID);
    }

    void Bank::addOrientation(const DUA_id ID, const DUA_dbl rotX, const DUA_dbl rotY, const DUA_dbl rotZ) {
        if (tryAddFlagToSoul(ORIENTATION, ID))
            tryAddComponent(ID, "rotation", comps_orientation, rotX, rotY, rotZ);
    }

    void Bank::addAngularVeloc(const DUA_id ID, const DUA_dbl angX, const DUA_dbl angY, const DUA_dbl angZ) {
        if (tryAddFlagToSoul(ANGVELOC, ID))
            tryAddComponent(ID, "angular velocity", comps_angVeloc, angX, angY, angZ);
    }

    void Bank::addSpaceControl(const DUA_id ID, const DUA_float fw, const DUA_float bk,
                               const DUA_float lf, const DUA_float rt, const DUA_float up,
                               const DUA_float dn, const DUA_float pitchp, const DUA_float pitchn,
                               const DUA_float yawp, const DUA_float yawn, const DUA_float rollp,
                               const DUA_float rolln) {
        if (tryAddFlagToSoul(CONTROLSS, ID))
            tryAddComponent(ID, "control", comps_spaceControl, fw, bk, lf, rt, up, dn, pitchp, pitchn, yawp, yawn,
                            rollp, rolln);
    }

    void Bank::addPointLight(const DUA_id ID, const DUA_colorByte red, const DUA_colorByte green,
                             const DUA_colorByte blue,
                             const DUA_dbl posX, const DUA_dbl posY, const DUA_dbl posZ) {
        if (tryAddFlagToSoul(LPOINT, ID))
            tryAddComponent(ID, "point light", comps_pntLight, red, green, blue, posX, posY, posZ);
    }

    void Bank::addDirectionalLight(const DUA_id ID, const DUA_colorByte red, const DUA_colorByte green,
                                   const DUA_colorByte blue,
                                   const DUA_dbl rotX, const DUA_dbl rotY, const DUA_dbl rotZ) {
        if (tryAddFlagToSoul(LDIRECT, ID))
            tryAddComponent(ID, "directional light", comps_dirLight, red, green, blue, rotX, rotY, rotZ);
    }

    void Bank::addAmbientLight(const DUA_id ID, const DUA_colorByte red, const DUA_colorByte green,
                               const DUA_colorByte blue) {
        if (tryAddFlagToSoul(LAMBIENT, ID))
            tryAddComponent(ID, "ambient light", comps_ambLight, red, green, blue);
    }

    void Bank::addOwner(const DUA_id ID, const DUA_id refID) {
        if (tryAddFlagToSoul(OWNER, ID))
            tryAddComponent(ID, "owner", comps_owner, refID);
    }

    void Bank::addScore(const DUA_id ID) {
        if (tryAddFlagToSoul(SCORE, ID))
            tryAddComponent(ID, "score", comps_score);
    }

    void Bank::addCollision(const DUA_id ID) {
        if (tryAddFlagToSoul(COLLISION, ID))
            tryAddComponent(ID, "collision", comps_collision);
    }

    void Bank::addCameraFree(const DUA_id ID, const DUA_float fov, const DUA_float zNear, const DUA_float zFar,
                             const DUA_dbl eyeX,
                             const DUA_dbl eyeY, const DUA_dbl eyeZ, const DUA_dbl focusX,
                             const DUA_dbl focusY,
                             const DUA_dbl focusZ,
                             const DUA_dbl upX, const DUA_dbl upY, const DUA_dbl upZ) {
        if (tryAddFlagToSoul(FREECAM, ID)) {
            if (tryAddComponent(ID, "free camera", comps_freeCam, fov, zNear, zFar, eyeX, eyeY, eyeZ, focusX,
                                focusY, focusZ, upX, upY, upZ)) {
                comps_soul.at(ID).state |= RECALCVIEWMAT | RECALCPROJMAT;
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
    void Bank::tryRemoveFlagFromSoul(const DUA_compFlag &flag, const DUA_id &ID) {
        try {
            comps_soul.at(ID).components &= ~flag;
        } catch (const std::out_of_range &oorException) {
            std::string error =
                    "Could not remove component at ID " + idToStr(ID) + ": no soul exists at that ID.\n";
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
    bool Bank::tryRemoveComponent(const DUA_id &ID, const char *compName, const DUA_compFlag &compFlag,
                                  KvMap<DUA_id, componentType> &table) {
        dlgt->systemsForceRemove(ID, compFlag);
        if (table.erase(ID) == 0) {
            std::string error =
                    "No " + std::string(compName) + " component exists to be removed at ID " + idToStr(ID) +
                    "\n";
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
    bool Bank::deleteSoul(const DUA_id &ID) {
        return tryRemoveComponent(ID, "soul", 0, comps_soul);
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
    void Bank::deleteModel(const DUA_id ID) {
        if (tryRemoveComponent(ID, "model", MODEL, comps_model))
            tryRemoveFlagFromSoul(MODEL, ID);
    }

    void Bank::deleteLinearVeloc(const DUA_id ID) {
        scrutinizeControl(ID, ControlTypes::SPACE);
        if (tryRemoveComponent(ID, "linear velocity", LINVELOC, comps_linVeloc))
            tryRemoveFlagFromSoul(LINVELOC, ID);
    }

    void Bank::deletePosition(const DUA_id ID) {
        scrutinizeCam(ID);
        if (tryRemoveComponent(ID, "position", POSITION, comps_position))
            tryRemoveFlagFromSoul(POSITION, ID);
    }

    void Bank::deleteSpatialChild(const DUA_id ID) {
        if (tryRemoveComponent(ID, "position child", SPATCHILD, comps_spatChild))
            tryRemoveFlagFromSoul(SPATCHILD, ID);
    }

    void Bank::deleteSpatialParent(const DUA_id ID) {
        if (tryRemoveComponent(ID, "position parent", SPATPARENT, comps_spatParent))
            tryRemoveFlagFromSoul(SPATPARENT, ID);
    }

    void Bank::deleteOrientation(const DUA_id ID) {
        scrutinizeCam(ID);
        scrutinizeControl(ID, ControlTypes::SPACE);
        if (tryRemoveComponent(ID, "rotation", ORIENTATION, comps_orientation))
            tryRemoveFlagFromSoul(ORIENTATION, ID);
    }

    void Bank::deleteAngularVeloc(const DUA_id ID) {
        scrutinizeControl(ID, ControlTypes::SPACE);
        if (tryRemoveComponent(ID, "angular velocity", ANGVELOC, comps_angVeloc))
            tryRemoveFlagFromSoul(ANGVELOC, ID);
    }

    void Bank::deleteSpaceControl(const DUA_id ID) {
        scrutinizeControl(ID, ControlTypes::SPACE);
        if (tryRemoveComponent(ID, "control", CONTROLSS, comps_spaceControl))
            tryRemoveFlagFromSoul(CONTROLSS, ID);
    }

    void Bank::deletePointLight(const DUA_id ID) {
        if (tryRemoveComponent(ID, "point light", LPOINT, comps_pntLight))
            tryRemoveFlagFromSoul(LPOINT, ID);
    }

    void Bank::deleteDirectionalLight(const DUA_id ID) {
        if (tryRemoveComponent(ID, "directional light", LDIRECT, comps_dirLight))
            tryRemoveFlagFromSoul(LDIRECT, ID);
    }

    void Bank::deleteAmbientLight(const DUA_id ID) {
        if (tryRemoveComponent(ID, "ambient light", LAMBIENT, comps_ambLight))
            tryRemoveFlagFromSoul(LAMBIENT, ID);
    }

    void Bank::deleteOwner(const DUA_id ID) {
        if (tryRemoveComponent(ID, "owner", OWNER, comps_owner))
            tryRemoveFlagFromSoul(OWNER, ID);
    }

    void Bank::deleteScore(const DUA_id ID) {
        if (tryRemoveComponent(ID, "score", SCORE, comps_score))
            tryRemoveFlagFromSoul(SCORE, ID);
    }

    void Bank::deleteCollision(const DUA_id ID) {
        if (tryRemoveComponent(ID, "collision", COLLISION, comps_collision))
            tryRemoveFlagFromSoul(COLLISION, ID);
    }

    void Bank::deleteCameraFree(const DUA_id ID) {
        scrutinizeCam(ID);
        if (tryRemoveComponent(ID, "free camera", FREECAM, comps_freeCam))
            tryRemoveFlagFromSoul(FREECAM, ID);
    }

/*******************************************************************************
 * ENTITY STATE GETTERS / SETTERS SECTION
 ******************************************************************************/
    DUA_compFlag Bank::getComponents(const DUA_id ID) {
        try {
            return comps_soul.at(ID).components;
        } catch (const std::out_of_range &oorException) {
            return DUA_INVALID_COMPONENTS;
        }
    }

    DUA_stateFlag Bank::getState(const DUA_id ID) {
        try {
            return comps_soul.at(ID).state;
        } catch (const std::out_of_range &oorException) {
            return DUA_INVALID_STATE;
        }
    }

    void Bank::stateOn(const DUA_id ID, const DUA_stateFlag flag) {
        try {
            comps_soul.at(ID).state |= flag;
        } catch (const std::out_of_range &oorException) {
            dlgt->output("WARNING: Bad stateOn attempt: No such ID.");
        }
    }

    void Bank::stateOff(const DUA_id ID, const DUA_stateFlag flag) {
        try {
            comps_soul.at(ID).state &= ~flag;
        } catch (const std::out_of_range &oorException) {
            dlgt->output("WARNING: Bad stateOff attempt: No such ID.");
        }
    }

/*******************************************************************************
 * ENTITY CREATION SECTION
 ******************************************************************************/
/*******************************************************************************
 * GENERATE ID
 ******************************************************************************/
    DUA_id Bank::generateID() {
        return nextID++;
    }

/*******************************************************************************
 * CREATE ENTITY
 ******************************************************************************/
    DUA_id Bank::spawnEntity(const char *name) {
        DUA_id ID = generateID();
        if (addSoul(ID, name)) {
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
    bool Bank::deleteEntity(const DUA_id ID) {
        DUA_compFlag flags;
        std::string name;
        try {
            flags = comps_soul.at(ID).components;
            name = comps_soul.at(ID).name;
        } catch (const std::out_of_range &oorException) {
            std::string error = "Could not delete entity " + idToStr(ID) + ": no soul exists at that ID.\n";
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

        } catch (...) {
            std::string error =
                    "Something went wrong upon attempting to delete entity " + idToStr(ID) + " ('" + name +
                    "')!\n";
            dlgt->output(error.c_str());
            return false;
        }

        std::string error = "Entity " + idToStr(ID) + " ('" + name + "') has been deleted.\n";
        dlgt->output(error.c_str());
        return true;
    }

/*******************************************************************************
 * PURGE ENTITY [NOT IMPLEMENTED YET]
 * this should not only delete the entity, but also delete any entities that are
 * spatial children of it.
 ******************************************************************************/
    bool Bank::purgeEntity(const DUA_id ID) {
        return deleteEntity(ID);
    }

/*******************************************************************************
 * CONVENIENCE STRING GETTERS SECTION
 ******************************************************************************/
    bool Bank::getIDs(std::string &name, std::vector<DUA_id> &IDs) {
        try {
            for (auto pair : comps_soul) {
                if (pair.second.name == name) {
                    IDs.push_back(pair.first);
                }
            }
            return true;
        } catch (const std::out_of_range &oorException) {
            return false;
        }
    }

/*******************************************************************************
 * GET NAME
 * returns a statement string containing the name of the entity (if any) at ID.
 ******************************************************************************/
    std::string Bank::getNameVerbose(const DUA_id ID) {
        std::ostringstream output;
        try {
            output << "Entity " << ID << " is named '" << comps_soul.at(ID).name << "'.";
        } catch (const std::out_of_range &oorException) {
            output << "Could not get name: No entity exists with ID " << ID;
        }
        return output.str();
    }

    std::string Bank::getName(const DUA_id ID) {
        try {
            return comps_soul.at(ID).name;
        } catch (const std::out_of_range &oorException) {
            return "Could not get name: No entity exists with ID " + idToStr(ID);
        }
    }

/*******************************************************************************
 * LIST COMPONENTS
 * returns a statement string containing a list of components currently
 * possessed by the entity at ID.
 ******************************************************************************/
    std::string Bank::listComponentsVerbose(const DUA_id ID) {
        std::ostringstream output;
        try {
            DUA_compFlag components = comps_soul.at(ID).components;

            if (components == DUA_DEFAULT_COMPONENTS) {
                output << "Entity " << getEntityInfo(ID) << " is a disembodied soul.";
            } else {

                output << "Entity " << getEntityInfo(ID) << " has: " << listComponents(components);

            }
        } catch (const std::out_of_range &oorException) {
            output << "Could not list components: No entity exists with ID " << ID;
        }
        return output.str();
    }

    std::string Bank::listComponents(const DUA_compFlag flag) {
        std::ostringstream output;
        for (auto compType : Docs::componentCollections) {
            if (flag & std::get<2>(compType)) {
                output << std::get<0>(compType) << ", ";
            }
        }
        return output.str();
    }

    std::string Bank::getEntityInfo(const DUA_id ID) {
        try {
            return (idToStr(ID) + " (" + comps_soul.at(ID).name + ")");
        } catch (const std::out_of_range &oorException) {
            return (idToStr(ID) + " (DOES NOT EXIST)");
        }
    }


/*******************************************************************************
 * CONVENIENCE CONSTRUCTS METHODS
 ******************************************************************************/


    bool Bank::updateActiveCamera(DUA_uint32 time) {
        if (activeFreeCameraID != DUA_NULL_ID) {
            if (getState(activeFreeCameraID) & RECALCVIEWMAT) {
                pFreeCameraCurrent->updateView(getRotMat(activeFreeCameraID), getPosMat(activeFreeCameraID, time));
            }
            if (getState(activeFreeCameraID) & RECALCPROJMAT) {
                pFreeCameraCurrent->updateProjection();
            }
            if (getState(activeFreeCameraID) & (RECALCVIEWMAT | RECALCPROJMAT)) {
                pFreeCameraCurrent->updateViewProjection();
                stateOff(activeFreeCameraID, RECALCVIEWMAT | RECALCPROJMAT);
            }
            return true;
        } else {
            return false;
        }
    }

    bool Bank::switchToCam(const DUA_id ID) {
        try {
            if ((comps_soul.at(ID).components) & FREECAM) {
                if (((comps_soul.at(ID).components) & (POSITION | ORIENTATION)) == (POSITION | ORIENTATION)) {
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
        } catch (const std::out_of_range &oorException) {
            dlgt->outputStr("No entity exists at ID " + idToStr(ID) + "\n");
            return false;
        }
        return true;
    }

    void Bank::scrutinizeCam(const DUA_id ID) {
        if (activeFreeCameraID == ID) {
            defocusCam();
            dlgt->outputStr("View from entity " + getEntityInfo(ID) + " has been lost.");
        }
    }

    void Bank::defocusCam() {
        activeFreeCameraID = DUA_NULL_ID;
        pFreeCameraCurrent = pFreeCameraDummy;
    }

    bool Bank::switchToControl(const DUA_id ID, ControlTypes::type controlType) {
        DUA_compFlag requiredInterfaceComponent;
        DUA_compFlag requiredOtherComponents;
        Delegate<void()> defocusControlCandidate;
        Delegate<void(const DUA_id)> focusControl;
        switch (controlType) {
            case ControlTypes::SPACE:
                requiredInterfaceComponent = CONTROLSS;
                requiredOtherComponents = ORIENTATION | LINVELOC | ANGVELOC;
                focusControl = DELEGATE(&Bank::focusSpaceControl, this);
                defocusControlCandidate = DELEGATE(&Bank::defocusSpaceControl, this);
                break;
            case ControlTypes::NONE:
                activeControlID = DUA_NULL_ID;
                defocusSpaceControl();
                //default other controls()...
                currentControlType = ControlTypes::NONE;
                dlgt->output("Control focus discarded.");
                return true;
            default:
                dlgt->outputStr("<!>    Invalid Control Type.");
                return false;
        }

        try {
            if (getComponents(ID) & requiredInterfaceComponent) {
                if ((getComponents(ID) & requiredOtherComponents) == requiredOtherComponents) {
                    activeControlID = ID;
                    currentControlType = controlType;
                    requiredControlComponents = requiredInterfaceComponent | requiredOtherComponents;
                    defocusControl = defocusControlCandidate;
                    focusControl(ID);

                } else {
                    dlgt->outputStr("entity " + getEntityInfo(ID) +
                                    " does not possess one or more of these required components: " +
                                    listComponents(requiredOtherComponents));
                    return false;
                }
            } else {
                dlgt->outputStr("entity " + getEntityInfo(ID) + " does not possess the required control interface.");
                return false;
            }
        } catch (const std::out_of_range &oorException) {
            dlgt->outputStr("No entity exists at ID " + idToStr(ID));
            return false;
        }
        return true;
    }

    void Bank::focusSpaceControl(const DUA_id ID) {
        //default other controls()...
        pSpaceControlCurrent = getSpaceControlPtr(ID);
        pCtrlLinVelocCurrent = getLinearVelocPtr(ID);
        pCtrlOrientCurrent = getOrientationPtr(ID);
    }

    void Bank::scrutinizeControl(const DUA_id ID, ControlTypes::type dependentControlType) {
        if (currentControlType == dependentControlType) {
            if (activeControlID ==
                ID) {// && getComponents(activeControlID) & requiredControlComponents != requiredControlComponents){
                dlgt->outputStr("Control of entity" + getEntityInfo(ID) + " has been lost.");
                currentControlType = ControlTypes::NONE;
                activeControlID = DUA_NULL_ID;
                defocusControl();
            }
        }
    }

    void Bank::defocusSpaceControl() {
        pSpaceControlCurrent = pSpaceControlDummy;
        pCtrlLinVelocCurrent = pCtrlLinVelocDummy;
        pCtrlOrientCurrent = pCtrlOrientDummy;
    }

    glm::mat4 Bank::getPosMat(const DUA_id ID, DUA_uint32 time) {
        try {
            if (getComponents(ID) & POSITION) {
                if (getComponents(ID) & LINVELOC) {
                    return comps_position.at(ID).getMatrix(time);
                } else {
                    return comps_position.at(ID).getMatrix();
                }
            } else {
                return Constants::duaIdentMat4;
            }
        } catch (const std::out_of_range &oorException) {
            dlgt->outputStr(
                    "ERROR: access to nonexistent position for matrix retrieval at ID " + idToStr(ID) + "\n");
        }
        return Constants::duaIdentMat4;
    }

    glm::mat4 Bank::getRotMat(const DUA_id ID) {
        try {
            if (getComponents(ID) & ORIENTATION) {
                return comps_orientation.at(ID).getMatrix();
            } else {
                return Constants::duaIdentMat4;
            }
        } catch (const std::out_of_range &oorException) {
            dlgt->outputStr(
                    "ERROR: access to nonexistent orientation for matrix retrieval at ID " + idToStr(ID) + "\n");
        }
        return Constants::duaIdentMat4;
    }

    glm::mat4 Bank::getModMat(const DUA_id ID, DUA_uint32 time) {
        return getPosMat(ID, time) * getRotMat(ID);
    }

    TimePiece Bank::timePiece;
    DUA_uint32 Bank::getTime() {
        return timePiece.getTime();
    }

    DUA_uint32 Bank::updateRenderTime() {
        currRenderTime = getTime();
        return currRenderTime;
    }

    DUA_uint32 Bank::getCurrRenderTime() {
        return currRenderTime;
    }
}
