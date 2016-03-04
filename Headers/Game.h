/* 
 * File:   Game.h
 * Author: adayoldbagel
 *
 * Created on February 7, 2015, 1:14 PM
 *
 */

#ifndef GAME_H
#define	GAME_H

#include <fstream>
#include <vector>

#include "Render_Master.h"
#include "Render_Console.h"
#include "Render_Models.h"
#include "Render_Background.h"
#include "PhysMove.h"
#include "PhysCollide.h"
#include "UserInput.h"
#include "Control_SS.h"
#include "Scripting.h"
#include "Engine.h"
#include "BankDelegates.h"
#include "Console.h"
#include "Event.h"

namespace DualityEngine {

    class Game {
    private:
        /*************
         * bank of COMPONENTS and other constructs to hold all game state data
         *************/
        Bank bank;

        /*************
         * The Central Event Queue (CEQ) to provide a universal event queue to all systems
         *************/
        EventQueue ceq;
        Token token;        // this is used for the concurrent queue
        bool isAlive;       // this is used to maintain or exit the main event loop

        /*************
         * SYSTEMS to operate on the components, providing game mechanics
         *************/
        System_Render_Master renderMasterSystem; // render all graphical components (manages openGL calls)
        System_Render_Console renderConsoleSystem; // render the in-game console
        System_Render_Models renderModelsSystem; // render any 3d models present
        System_Render_Background renderBackgroundSystem; // render the background (usually skybox)
        System_PhysMove physicsMoveSystem; // update all spatial components according to motion components
        System_PhysCollide physicsCollisionSystem; // check collisions between entities via collision components
        System_Control_SS spaceShipControlSystem; // apply user's key presses to current spaceShipControl component
        System_UserInput userInputSystem; // handle user input
        System_Scripting scriptingSystem; // handle scripting commands

        /*************
         * ENGINES (thread with accompanying SystemEngine) on which to run the systems
         *************/
		SDL_Thread* graphicsThread; // run all graphics Systems
		typedef SystemEngine<System_Render_Master*, System_Render_Background*, System_Render_Models*,
			System_Render_Console*> DUA_graphicsEngine;
		DUA_graphicsEngine graphicsEngine;

        SDL_Thread* physicsThread; // run all physics Systems (and physical control system)
        typedef SystemEngine<System_UserInput*, System_Control_SS*, System_PhysMove*, System_PhysCollide*> DUA_physicsEngine;
        DUA_physicsEngine physicsEngine;

        SDL_Thread* scriptingThread; // run the scripting systems
        typedef SystemEngine<System_Scripting*> DUA_scriptingEngine;
        DUA_scriptingEngine scriptingEngine;

        /*************
         * DELEGATES to allow for inter-system/engine communication (they allow pretty much any function to be called
         * from anywhere, as long as you have a pointer to one of these delegates. So just watch who you give them to.)
         * All delegates are contained in the bankDelegates struct, to which bank has a public pointer.
         * This is so that anything with access to the bank has access to the delegates.
         *************/
        BankDelegates bankDelegates;
        
        /*************
         * An in-game CONSOLE and output logging device
         *************/
        Console console;

        /*************
         * Methods
         *************/
        void NewGame();
        void LoadGame(const std::string&);
        void SaveGame(const std::string&);
        void Pause();
        void Resume();
        void Quit();

        bool engageEngines();
        bool killSystems();
        bool cleanGameData();
        bool pauseBankDependentSystems();
        bool waitForBankDependentSystemsToPause();
        bool resumeBankDependentSystems();
        void systems_discover(const DUA_id ID);
        void systems_scrutinize(const DUA_id ID);
        void systems_forceRemove(const DUA_id ID, const DUA_compFlag component);
        void handleEvents();
        DUA_uint32 getTime();
        void sayHi();
        
    public:
        Game();
        ~Game();
        void Main();
    };

}
#endif	/* GAME_H */

