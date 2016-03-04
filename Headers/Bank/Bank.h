/* 
 * File:   ComponentBank.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 5:19 PM
 */

#ifndef COMPONENTBANK_H
#define	COMPONENTBANK_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include <cstdint>
#include <sstream>
#include <iostream>
#include <vector>
#include <SDL.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "KvMap.h"
#include "ToString.h"
#include "BankDelegates.h"
#include "../Utilities/Settings.h"
#include "TimePiece.h"

#include "../Components/Soul.h"
#include "../Components/Model.h"
#include "../Components/SpaceControl.h"
#include "../Components/Position.h"
#include "../Components/PositionChild.h"
#include "../Components/PositionParent.h"
#include "../Components/LinearVelocity.h"
#include "../Components/Orientation.h"
#include "../Components/AngularVelocity.h"
#include "../Components/PointLight.h"
#include "../Components/DirectionalLight.h"
#include "../Components/AmbientLight.h"
#include "../Components/Owner.h"
#include "../Components/Score.h"
#include "../Components/Collision.h"
#include "../Components/CameraFree.h"

#include "ControlTypes.h"

#include "HelpDocs.h"

#ifdef _WIN32
#include <array>
#endif

namespace DualityEngine {

    class Bank {

        DUA_id nextID;
        
        KvMap<DUA_id, Soul>             comps_soul;
        KvMap<DUA_id, Model>            comps_model;
        KvMap<DUA_id, Position>         comps_position;
        KvMap<DUA_id, SpatialChild>     comps_spatChild;
        KvMap<DUA_id, SpatialParent>    comps_spatParent;
        KvMap<DUA_id, LinearVelocity>   comps_linVeloc;
        KvMap<DUA_id, Collision>        comps_collision;
        KvMap<DUA_id, Orientation>      comps_orientation;
        KvMap<DUA_id, AngularVelocity>  comps_angVeloc;
        KvMap<DUA_id, SpaceControl>     comps_spaceControl;
        KvMap<DUA_id, PointLight>       comps_pntLight;
        KvMap<DUA_id, DirectionalLight> comps_dirLight;
        KvMap<DUA_id, AmbientLight>     comps_ambLight;
        KvMap<DUA_id, Owner>            comps_owner;
        KvMap<DUA_id, Score>            comps_score;
        KvMap<DUA_id, CameraFree>       comps_freeCam;
        
        /* COMPONENT POINTER GETTER */
        template<class componentType>
        componentType* getComponentPtr(const DUA_id&, const char*, KvMap<DUA_id, componentType>&);

        /* COMPONENT CREATION */
        bool tryAddFlagToSoul(const DUA_compFlag &flag, const DUA_id &ID);

        template<class componentType, typename ... types>
        bool tryAddComponent(const DUA_id &ID, const char* compName, KvMap<DUA_id, componentType> &, const types& ... args);

        bool addSoul(const DUA_id &ID, const char* name);
        
        /* COMPONENT DELETION */
        void tryRemoveFlagFromSoul(const DUA_compFlag &flag, const DUA_id &ID);

        template<class componentType>
        bool tryRemoveComponent(const DUA_id &ID, const char* compName, const DUA_compFlag& compFlag, KvMap<DUA_id, componentType> &table);

        bool deleteSoul(const DUA_id &ID);
        
        /* ENTITY CREATION */
        DUA_id generateID();

    public:        

        Bank(BankDelegates* dlgt);
        ~Bank();
        
        /* BANK MANAGEMENT */
        void clean();
        void save(const char* saveName);
        void load(const char* saveName);
        
        /* COMPONENT POINTER GETTERS */
        Model* getModelPtr(const DUA_id ID);
        LinearVelocity* getLinearVelocPtr(const DUA_id ID);
        Position* getPositionPtr(const DUA_id ID);
        Orientation* getOrientationPtr(const DUA_id ID);
        AngularVelocity* getAngularVelocPtr(const DUA_id ID);
        SpatialChild* getSpatialChildPtr(const DUA_id ID);
        SpatialParent* getSpatialParentPtr(const DUA_id ID);
        SpaceControl* getSpaceControlPtr(const DUA_id ID);
        PointLight* getPointLightPtr(const DUA_id ID);
        DirectionalLight* getDirectionalLightPtr(const DUA_id ID);
        AmbientLight* getAmbientLightPtr(const DUA_id ID);
        Owner* getOwnerPtr(const DUA_id ID);
        Score* getScorePtr(const DUA_id ID);
        Collision* getCollisionPtr(const DUA_id ID);
        CameraFree* getCameraFreePtr(const DUA_id ID);
        
        /* COMPONENT CREATION */
//        template<typename ... types>
//        bool addComponent(const DUA_id &ID, const char* compType, const types& ... args);
        
        void addModel(const DUA_id ID, const char *fileName);
        void addLinearVeloc(const DUA_id ID, const DUA_dbl velX, const DUA_dbl velY, const DUA_dbl velZ);
        void addPosition(const DUA_id ID, const DUA_dbl posX, const DUA_dbl posY, const DUA_dbl posZ);
        void addOrientation(const DUA_id ID, const DUA_dbl rotX, const DUA_dbl rotY, const DUA_dbl rotZ);
        void addAngularVeloc(const DUA_id ID, const DUA_dbl angX, const DUA_dbl angY, const DUA_dbl angZ);
        void addSpatialChild(const DUA_id ID, const DUA_id refID);
        void addSpatialParent(const DUA_id ID, const DUA_id refID);
        void addSpaceControl(const DUA_id ID, const DUA_float fw, const DUA_float bk,
                             const DUA_float lf, const DUA_float rt, const DUA_float up,
                             const DUA_float dn, const DUA_float pitchp, const DUA_float pitchn,
                             const DUA_float yawp, const DUA_float yawn, const DUA_float rollp, const DUA_float rolln);
        void addPointLight(const DUA_id ID, const DUA_colorByte red, const DUA_colorByte green,const DUA_colorByte blue,
                           const DUA_dbl posX, const DUA_dbl posY, const DUA_dbl posZ);
        void addDirectionalLight(const DUA_id ID, const DUA_colorByte red, const DUA_colorByte green,
                                 const DUA_colorByte blue, const DUA_dbl rotX, const DUA_dbl rotY, const DUA_dbl rotZ);
        void addAmbientLight(const DUA_id ID, const DUA_colorByte red, const DUA_colorByte green,
                             const DUA_colorByte blue);
        void addOwner(const DUA_id ID, const DUA_id refID);
        void addScore(const DUA_id ID);
        void addCollision(const DUA_id ID);
        void addCameraFree(const DUA_id ID, const DUA_float fov, const DUA_float zNear, const DUA_float zFar,
                           const DUA_dbl eyeX, const DUA_dbl eyeY, const DUA_dbl eyeZ,
                           const DUA_dbl focusX, const DUA_dbl focusY, const DUA_dbl focusZ,
                           const DUA_dbl upX, const DUA_dbl upY, const DUA_dbl upZ);

        /* COMPONENT DELETION */
        void deleteModel(const DUA_id ID);
        void deleteLinearVeloc(const DUA_id ID);
        void deletePosition(const DUA_id ID);
        void deleteOrientation(const DUA_id ID);
        void deleteAngularVeloc(const DUA_id ID);
        void deleteSpatialChild(const DUA_id ID);
        void deleteSpatialParent(const DUA_id ID);
        void deleteSpaceControl(const DUA_id ID);
        void deletePointLight(const DUA_id ID);
        void deleteDirectionalLight(const DUA_id ID);
        void deleteAmbientLight(const DUA_id ID);
        void deleteOwner(const DUA_id ID);
        void deleteScore(const DUA_id ID);
        void deleteCollision(const DUA_id ID);
        void deleteCameraFree(const DUA_id ID);
        
        /* ENTITY STATE GETTERS / SETTERS */
        DUA_compFlag getComponents(const DUA_id ID);
        DUA_stateFlag getState(const DUA_id ID);
        void stateOn(const DUA_id ID, const DUA_stateFlag flag);
        void stateOff(const DUA_id ID, const DUA_stateFlag flag);

        /* ENTITY CREATION */
        DUA_id spawnEntity(const char *name);
        DUA_id createBox(const char *name,
                         const DUA_dbl posX, const DUA_dbl posY, const DUA_dbl posZ,
                         const DUA_dbl rotX, const DUA_dbl rotY, const DUA_dbl rotZ,
                         const DUA_dbl velX, const DUA_dbl velY, const DUA_dbl velZ,
                         const DUA_dbl angX, const DUA_dbl angY, const DUA_dbl angZ);
        
        /* ENTITY DELETION */
        bool deleteEntity(const DUA_id ID);  // orphans children
        bool purgeEntity(const DUA_id ID);   // deletes children too.

        /* CONVENIENCE GETTERS */
        bool getIDs(std::string& name, std::vector<DUA_id>& IDs);
        std::string getNameVerbose(const DUA_id ID);
        std::string getName(const DUA_id ID);
        std::string listComponentsVerbose(const DUA_id ID);
        std::string listComponents(const DUA_compFlag flag);
        std::string getEntityInfo(const DUA_id ID);
        
        /*****************************
         * HEREAFTER FOLLOW VARIOUS HELPFUL CONSTRUCTS NOT FUNDAMENTAL TO COMPONENTS,
         * BUT SHARED BY ALL SYSTEMS AS PART OF GAME STATE OR GAME FUNCTIONALITY
         *****************************/

        // The dang SDL window that doesn't want to go anywhere else.
        SDL_Window* pWindow;
        
        /* PUBLIC DELEGATES */
        BankDelegates* dlgt;

        /* TIME PIECE */
        static TimePiece timePiece;
        DUA_uint32 getTime();
        
        bool switchToControl(const DUA_id id, ControlTypes::type controlType);
        void scrutinizeControl(const DUA_id id, ControlTypes::type controlType);
        DUA_id activeControlID;
        LinearVelocity* pCtrlLinVelocCurrent;
        LinearVelocity* pCtrlLinVelocDummy;
        Orientation* pCtrlOrientCurrent;       
        Orientation* pCtrlOrientDummy; 
        AngularVelocity* pCtrlAngVelocCurrent;
        AngularVelocity* pCtrlAngVelocDummy;
        Delegate<void()> defocusControl;
        DUA_compFlag requiredControlComponents;
        ControlTypes::type currentControlType;
        
        void focusSpaceControl(const DUA_id ID);
        void defocusSpaceControl();        
        SpaceControl* pSpaceControlCurrent;   
        SpaceControl* pSpaceControlDummy;
        
        
        /* CAM STUFF */
        
        DUA_id activeFreeCameraID;
        CameraFree* pFreeCameraCurrent;
        CameraFree* pFreeCameraDummy;
        bool updateActiveCamera(DUA_uint32 time);
        bool switchToCam(const DUA_id id);
        void scrutinizeCam(const DUA_id id);
        void defocusCam();
        
        
        /* TRANSFORM MATRIX GETTERS */
        
        glm::mat4 getPosMat(const DUA_id ID, DUA_uint32 time);     // translation matrix
        glm::mat4 getRotMat(const DUA_id ID);     // rotation matrix
        glm::mat4 getModMat(const DUA_id ID, DUA_uint32 time);     // full model matrix (combines above two)
    };

}

#endif	/* COMPONENTSTORE_H */

