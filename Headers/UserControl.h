/*******************************************************************************
 * File:   UserControl.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:38 AM
 * 
 * 
 ******************************************************************************/

#ifndef USERCONTROL_H
#define	USERCONTROL_H

#include <SDL.h>
#include "System.h"
#include <glm/glm.hpp>
#include "ControlDelegates.h"

namespace DualityEngine {

    class System_UserControl : public System
    {
    private:
        ControlDelegates* dlgt;
        SDL_Event sdlEvent;
        bool consoleIsActive = false;
        bool MenuIsActive = false;
        const std::string menuText = "****************************************\n*  MENU - ENTER ONE OF THESE OPTIONS:  *\n*        new, load, save, exit         *\n****************************************\n";
        std::unordered_map<std::string, const std::string> commandUsages = {
            {"newent" , "\"newent [name (no spaces)]\""},
            {"add" , "\"add [component type] [entity ID] [component arguments...]\""},
            {"delete" , "\"delete [component type] [entity ID]\""},
            {"help" , "\"help\" OR \"help [command]\" OR \"help [component]\""}
        };
        std::unordered_map<std::string, const std::string> commandExamples = {
            {"newent" , "\"newent testEntity\""},
            {"add" , "\"add position 12 10 0 10\" gives entity 12 a spatial position of (10, 0, 10)"},
            {"delete" , "\"delete model 6\" removes the graphical model possessed by entity 6"},
            {"help" , "\"help add\" helps the user to know how to use the \"add\" command."}
        };
        std::unordered_map<std::string, const std::string> commandHelps = {
            {"newent" , "creates a new entity, displaying its ID (CTRL-C to copy the ID afterwards)"},
            {"add" , "creates a new component (or functionality) for a given entity."},
            {"delete" , "removes a component (or functionality) from a given entity."},
            {"help" , "helps those who help themselves."}
        };
        std::unordered_map<std::string, const std::string> componentHelps = {
            {"soul" , "is something that every entity starts out with. they cannot be created directly."},
            {"position" , "gives an entity a location in 3D space."},
            {"rotation" , "gives an entity an orientation in 3D space."},
            {"linveloc" , "gives an entity a linear velocity in 3D space (translation)."},
            {"angveloc" , "gives an entity an angular velocity (spin)."},
            {"collision" , "allows an entity to collide with other entities if it has a position and geometry."},
            {"model" , "makes an entity visible by giving it a 3D model representation."},
            {"control" , "allows an entity to be controlled by the user."},
            {"lambient" , "attaches an ambient light to an entity."},
            {"ldirect" , "attaches a directional light to an entity."},
            {"lpoint" , "attaches a point light to an entity."},
            {"poschild" , "means another entity is a positional child of this entity."},
            {"posparent" , "means another entity is a positional parent of this entity."},
            {"owner" , "means another entity is the owner of this entity."},
            {"score" , "gives an entity a scorekeeping device."}
        };
        std::unordered_map<std::string, const std::string> componentArgs = {
            {"soul" , "cannot be created with the \"add\" command. \"newent\" must be used"},
            {"position" , "[X-coordinate] [Y-coordinate] [Z-coordinate]"},
            {"rotation" , "[X-angle (pitch)] [Y-angle (yaw)] [Z-angle (roll)]"},
            {"linveloc" , "[X velocity] [Y velocity] [Z velocity]"},
            {"angveloc" , "[X angular velocity] [Y angular velocity] [Z angular velocity]"},
            {"collision" , ""},
            {"model" , "[filepath to model]"},
            {"control" , ""},
            {"lambient" , "[R] [G] [B]"},
            {"ldirect" , "[R] [G] [B] [X vector component] [Y vector component] [Z vector component]"},
            {"lpoint" , "[R] [G] [B] [X-coordinate] [Y-coordinate] [Z-coordinate]"},
            {"poschild" , "[ID number of positional child]"},
            {"posparent" , "[ID number of positional parent]"},
            {"owner" , "[ID number of owner]"},
            {"score" , ""}
        };
        
        void parseCommand(std::string command);
        void handleBadUsage(std::string command);
        void handleControlKeys(const Uint8* keyStates);
        void presentTextMenu();
    public:
        System_UserControl(ComponentBank* bank, ControlDelegates* delegates);
        ~System_UserControl();
        void tick() override;
        bool init(std::stringstream& output) override;
    };

}

#endif	/* USERCONTROL_H */

