/* 
 * File:   Game.h
 * Author: adayoldbagel
 *
 * Created on February 7, 2015, 1:14 PM
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
#include "UserControl.h"
#include "Scripting.h"
#include "Engine.h"
#include "BankDelegates.h"
#include "Console.h"

namespace DualityEngine {

    class Game
    {
    private:
   
        /*************
         * bank of COMPONENTS and other constructs to hold all game state data
         *************/

        ComponentBank bank = ComponentBank(&bankDelegates);

        /*************
         * SYSTEMS to operate on the components, providing game mechanics
         *************/

        // render all graphical components (manages openGL calls)
        System_Render_Master renderMasterSystem = System_Render_Master(&bank);
        // render the in-game console
        System_Render_Console renderConsoleSystem = System_Render_Console(&bank, &console);
        // render any 3d models present
        System_Render_Models renderModelsSystem = System_Render_Models(&bank);
        // render the background (usually skybox)
        System_Render_Background renderBackgroundSystem = System_Render_Background(&bank);
        
        // update all spatial components according to motion components
        System_PhysMove physicsMoveSystem = System_PhysMove(&bank);
        // check collisions between all spatial components according to collision components
        System_PhysCollide physicsCollisionSystem = System_PhysCollide(&bank);
        
        // handle user input
        System_UserControl userControlSystem = System_UserControl(&bank);
        // handle scripting commands
        System_Scripting scriptingSystem = System_Scripting(&bank);
        // More systems to come...

        /*************
         * ENGINES on which to run the systems
         *************/

        // An engine (with accompanying thread) to run all graphics Systems
        SDL_Thread* graphicsThread = NULL;
        SystemEngine graphicsEngine = SystemEngine(&graphicsThread, "Duality Graphics Engine",
                                                   &outputDelegate, &quitDelegate);    
        
        // An engine (with accompanying thread) to run all physics Systems (and physical control system)
        SDL_Thread* physicsThread = NULL;
        SystemEngine physicsEngine = SystemEngine(&physicsThread, "Duality Physics Engine",
                                                  &outputDelegate, &quitDelegate);
        
        // And one to run the scripting systems
        SDL_Thread* scriptingThread = NULL;
        SystemEngine scriptingEngine = SystemEngine(&scriptingThread, "Duality Scripting Engine",
                                                    &outputDelegate, &quitDelegate);
        // More engines to come...
        
        /*************
         * An in-game CONSOLE and output logging device
         *************/

        Console console;

        /*************
         * DELEGATES to allow for inter-system/engine communication
         *************/
        
        // These four delegates are assigned outside of bankDelegates for convenience of use inside game.cpp
        
        // A delegate for quitting the game
        Delegate<void()> quitDelegate = DELEGATE(&Game::Quit, this);
        // A delegate for outputting text to the console
        Delegate<void(const char*)> outputDelegate = DELEGATE(&Console::output, &console);
        // Same thing, but for std::string
        Delegate<void(const std::string&)> outputStrDelegate = DELEGATE(&Console::outputStr, &console);
        // Delegate to submit a command to the scripting system
        Delegate<void(const std::string&)> submitCommand = DELEGATE(&System_Scripting::submitCommand, &scriptingSystem);
        
        // Delegates are now consolidated into a single large structure held by the bank itself, instead of many small
        // system-specific collections. These delegates will be available to all systems via bank->dlgt.        
        BankDelegates bankDelegates = {
            DELEGATE(&Game::systems_discover, this),
            DELEGATE(&Game::systems_scrutinize, this),
            DELEGATE(&Game::systems_forceRemove, this),
            quitDelegate, DELEGATE(&Game::NewGame, this),
            DELEGATE(&Game::Pause, this),
            DELEGATE(&Game::Resume, this),
            DELEGATE(&System_Scripting::submitScript, &scriptingSystem),
            DELEGATE(&Game::pauseBankDependentSystems, this),
            DELEGATE(&Game::waitForBankDependentSystemsToPause, this),
            DELEGATE(&Game::resumeBankDependentSystems, this),
            DELEGATE(&Console::applyBackspace, &console),
            DELEGATE(&Console::applyDelete, &console),
            DELEGATE(&Console::clearCommand, &console),
            DELEGATE(&Console::upOneCommand, &console),
            DELEGATE(&Console::downOneCommand, &console),
            DELEGATE(&Console::leftCursor, &console),
            DELEGATE(&Console::rightCursor, &console),
            outputDelegate, outputStrDelegate,
            DELEGATE(&Console::addToCommand, &console),
            DELEGATE(&Console::submitCommand, &console),
            submitCommand,
            DELEGATE(&Console::getLogLineFromBack, &console),
            DELEGATE(&Console::getCurrentLogLine, &console),
            DELEGATE(&Console::setState, &console),
            DELEGATE(&Console::traverseLog, &console),
            DELEGATE(&System_Render_Background::queueSkyChange, &renderBackgroundSystem)
            
        };
        
        

        /*************
         * Low-level 'internal' METHODS
         *************/

        bool engageEngines();
        bool killSystems();
        bool cleanGameData();
        bool pauseBankDependentSystems();
        bool waitForBankDependentSystemsToPause();
        bool resumeBankDependentSystems();
        void systems_discover(const DUA_id &ID);
        void systems_scrutinize(const DUA_id &ID);
        void systems_forceRemove(const DUA_id &ID, const DUA_compFlag& component);
        // More internal functions to come...

        /*************
         * High-level 'interface' METHODS
         *************/

        void NewGame();
        void LoadGame(const std::string&);
        void SaveGame(const std::string&);
        void Pause();
        void Resume();
        void Quit();
        // More interface functions to come...
        
    public:
        Game() {};
        ~Game();
        // Called from wherever Game is instantiated
        void Main();
    };

}
#endif	/* GAME_H */

