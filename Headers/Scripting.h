/*******************************************************************************
 * File:   Scripting.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:38 AM
 * 
 * 
 ******************************************************************************/

#ifndef SCRIPTING_H
#define	SCRIPTING_H

#include "System.h"
#include "ScriptingDelegates.h"
#include <queue>

namespace DualityEngine {

    class System_Scripting : public System
    {
    private:
        ScriptingDelegates* dlgt;
        std::queue<std::string> commandQueue;
        std::unordered_map<std::string, DUA_id> entityVariables = {};
        std::unordered_map<std::string, const std::string> commandUsages = {
            {"newent" , "newent [name]    (no spaces in names)"},
            {"delent" , "delent [entity ID]"},
            {"add" , "add [component type] [entity ID] [component arguments...]"},
            {"remove" , "remove [component type] [entity ID]"},
            {"id" , "id [name]    (no spaces in names)"},
            {"name" , "name [entity ID]"},
            {"help" , "help  OR  help [command]  OR  help [component]  OR  help components"},
            {"comps" , "comps [entity ID]"},
            {"cam" , "cam [entity ID]"},
            {"control" , "control [entity ID]"},
            {"exit" , "exit"},
            {"new" , "new OR new [script]"},
            {"save" , "save [save name]    (no spaces in names)"},
            {"load" , "load [save name]    (no spaces in names)"},
            {"run" , "run [script]"}
        };
        std::unordered_map<std::string, const std::string> commandExamples = {
            {"newent" , "\"newent testEntity\" creates a new entity (new soul) named \"testEntity\""},
            {"delent" , "\"delent A6\" deletes the entity with ID A6"},
            {"add" , "\"add position 12 10 0 10\" gives entity 12 a spatial position of (10, 0, 10)"},
            {"remove" , "\"remove model 6\" removes the graphical model possessed by entity 6"},
            {"id" , "\"id fooBox\" displays the ID of the entity named \"fooBox,\" if there is one (CTRL-C to copy ID afterwards)."},
            {"name" , "\"name 42\" displays the name of entity 42."},
            {"help" , "\"help add\" helps the user to know how to use the \"add\" command."},
            {"comps" , "\"comps 343\" lists all components of entity 343. If 343 has a model, \"MODEL\" will be displayed."},
            {"cam" , "\"cam 33\" switches the view to that of the camera of entity 33, if it has one."},
            {"control" , "\"control 4045\" switches the users control to entity 4045."},
            {"exit" , "\"exit\" exits the game, no prompts offered."},
            {"new" , "\"new\" creates an empty world (game state). \"new testScene\" creates an empty game state, then runs the script \"testScene\"."},
            {"save" , "\"save myGame\" creates a new saved state called \"myGame\" that you can load later."},
            {"load" , "\"load myGame\" discards the current game state and instead loads the saved state named \"myGame\"."},
            {"run" , "\"run myScript\" executes the duality script named \"myScript\"."}
        };
        std::unordered_map<std::string, const std::string> commandHelps = {
            {"newent" , "creates a new entity, displaying its assigned ID (CTRL-C to copy the ID afterwards)."},
            {"delent" , "deletes an entity by removing all components belonging to a given ID, including the soul."},
            {"add" , "creates a new component (or functionality) for a given entity."},
            {"remove" , "removes a component (or functionality) from a given entity."},
            {"id" , "tells the user the ID number of any and all entities with a given name."},
            {"name" , "displays the name given to the entity assigned a given ID."},
            {"help" , "helps those who help themselves."},
            {"comps" , "displays a list of all components associated with a given ID, in Duality enumerator format."},
            {"cam" , "switches the users view to the perspective of the entity at a given ID."},
            {"control" , "binds the user's controls to the control component of a given entity."},
            {"exit" , "exits the game."},
            {"new" , "discards any currently running game and presents an empty world."},
            {"save" , "stores the current game state to the hard drive under a given name to be loaded later. Overwrites saves with the same name."},
            {"load" , "discards any current game state and presents the game state stored under a given name."},
            {"run" , "executes a script, regardless of current game state (scripted variable names may be overwritten, saves may be corrupted, etc). Use at your own peril."}
        };
        std::unordered_map<std::string, const std::string> componentHelps = {
            {DUA_COMPCOLL(0, 1) , "is something that every entity starts out with. Souls cannot be created or destroyed directly."},
            {DUA_COMPCOLL(2, 1) , "gives an entity a location in 3D space."},
            {DUA_COMPCOLL(7, 1) , "gives an entity an orientation in 3D space."},
            {DUA_COMPCOLL(5, 1) , "gives an entity a linear velocity in 3D space (translation)."},
            {DUA_COMPCOLL(8, 1) , "gives an entity an angular velocity (spin)."},
            {DUA_COMPCOLL(6, 1) , "allows an entity to collide with other entities if it has a position and geometry."},
            {DUA_COMPCOLL(1, 1) , "makes an entity visible by giving it a 3D model representation."},
            {DUA_COMPCOLL(9, 1) , "allows an entity to be controlled by the user."},
            {DUA_COMPCOLL(12, 1), "attaches an ambient light to an entity."},
            {DUA_COMPCOLL(11, 1), "attaches a directional light to an entity."},
            {DUA_COMPCOLL(10, 1), "attaches a point light to an entity."},
            {DUA_COMPCOLL(3, 1) , "means another entity is a positional child of this entity."},
            {DUA_COMPCOLL(4, 1) , "means another entity is a positional parent of this entity."},
            {DUA_COMPCOLL(13, 1), "means another entity is the owner of this entity."},
            {DUA_COMPCOLL(14, 1), "gives an entity a scorekeeping device."},
            {DUA_COMPCOLL(15, 1), "allows a user to see from the viewpoint of an entity."}
        };
        std::unordered_map<std::string, std::pair<const std::string, const int>> componentArgs = {
            {DUA_COMPCOLL(0, 1) , {"N/A - a soul cannot be created with the \"add\" command. \"newent\" must be used" , 0}},
            {DUA_COMPCOLL(2, 1) , {"[X-coordinate] [Y-coordinate] [Z-coordinate]" , 3}},
            {DUA_COMPCOLL(7, 1) , {"[X-angle (pitch)] [Y-angle (yaw)] [Z-angle (roll)]" , 3}},
            {DUA_COMPCOLL(5, 1) , {"[X velocity] [Y velocity] [Z velocity]" , 3}},
            {DUA_COMPCOLL(8, 1) , {"[X angular velocity] [Y angular velocity] [Z angular velocity]" , 3}},
            {DUA_COMPCOLL(6, 1) , {"none" , 0}},
            {DUA_COMPCOLL(1, 1) , {"[filepath to model]" , 1}},
            {DUA_COMPCOLL(9, 1) , {"none" , 0}},
            {DUA_COMPCOLL(12, 1), {"[R] [G] [B]" , 3}},
            {DUA_COMPCOLL(11, 1), {"[R] [G] [B] [X vector component] [Y vector component] [Z vector component]" , 6}},
            {DUA_COMPCOLL(10, 1), {"[R] [G] [B] [X-coordinate] [Y-coordinate] [Z-coordinate]" , 6}},
            {DUA_COMPCOLL(3, 1) , {"[ID number of positional child]" , 1}},
            {DUA_COMPCOLL(4, 1) , {"[ID number of positional parent]" , 1}},
            {DUA_COMPCOLL(13, 1), {"[ID number of owner]", 1}},
            {DUA_COMPCOLL(14, 1), {"none" , 0}},
            {DUA_COMPCOLL(15, 1), {"[field of view in radians] [near plane] [far plane] [eye X] [eye Y] [eye Z] [focus X] [focus Y] [focus Z] [up X] [up Y] [up Z]", 12}}
        };
        
        DUA_id prsID(const std::string& IDstring);
        DUA_dbl prsDbl(const std::string& dblString);
        DUA_float prsFlt(const std::string& floatString);
        DUA_colorByte prsClr(const std::string& colorValue);
        void parseCommand(const std::string& command);
        void parseAddCommand(const std::vector<std::string>& args);
        void parseAssignment(const std::vector<std::string>& args);
        void parseRemoveCommand(const std::vector<std::string>& args);
        void handleBadUsage(const std::string& command);
    public:
        System_Scripting(ComponentBank* bank, ScriptingDelegates* delegates);
        ~System_Scripting();
        void tick() override;
        bool init(std::stringstream& output) override;
        void submitCommand(const std::string&);
    };

}

#endif	/* SCRIPTING_H */

