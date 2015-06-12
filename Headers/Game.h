/* 
 * File:   Game.h
 * Author: adayoldbagel
 *
 * Created on February 7, 2015, 1:14 PM
 */

#ifndef GAME_H
#define	GAME_H

//<editor-fold defaultstate="collapsed" desc="Includes">
#include <fstream>
#include <vector>

#include "Render_Master.h"
#include "Render_Console.h"
#include "Render_Models.h"
#include "PhysMove.h"
#include "PhysCollide.h"
#include "UserControl.h"
#include "Scripting.h"
#include "Engine.h"
#include "ControlDelegates.h"
#include "ScriptingDelegates.h"
#include "BankDelegates.h"
#include "Console.h"
//</editor-fold>

namespace DualityEngine {

    class Game
    {
    private:
   
        //<editor-fold defaultstate="collapsed" desc="An in-game console and output logging device">

        Console console;

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Delegates to allow for inter-system/engine communication">
        
        // A delegate for quitting the game to give to the engines in case of failures
        Delegate<void()> quitDelegate = DELEGATE(&Game::Quit, this);
        // A delegate for outputting text to the console to give to the engines
        Delegate<void(const char*)> outputDelegate = DELEGATE(&Console::output, &console);
        // Same thing, but for std::string
        Delegate<void(const std::string&)> outputStrDelegate = DELEGATE(&Console::outputStr, &console);
        // Delegate to submit a command to the scripting system
        Delegate<void(const std::string&)> submitCommand = DELEGATE(&Game::submitScriptCommand, this);
        // control delegates of top level functions to give to the UserControl system
        ControlDelegates controlDelegates = {
            quitDelegate, DELEGATE(&Game::NewGame, this),
            DELEGATE(&Game::Pause, this),
            DELEGATE(&Game::Resume, this),
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
            DELEGATE(&Console::setState, &console),
            DELEGATE(&Console::traverseLog, &console),
            DELEGATE(&Game::RunScript, this)
        };
        ScriptingDelegates scriptingDelegates = {
            outputDelegate, outputStrDelegate
        };
        // Some more delegates for the bank
        BankDelegates bankDelegates = {
            DELEGATE(&Game::systems_discover, this),
            DELEGATE(&Game::systems_scrutinize, this),
            DELEGATE(&Game::systems_forceRemove, this), outputDelegate, outputStrDelegate
        };
        
        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Components that hold all game state data">

        ComponentBank bank = ComponentBank(&bankDelegates); // holds collections of components, includes methods to manage them

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Systems to operate on the components, providing game mechanics">

        // A system to render all graphical components (manages openGL calls)
        System_Render_Master renderMasterSystem = System_Render_Master(&bank);
        
        System_Render_Console renderConsoleSystem = System_Render_Console(&bank, &console);
        
        System_Render_Models renderModelsSystem = System_Render_Models(&bank);
        // A system to update all spatial components according to motion components
        System_PhysMove physicsMoveSystem = System_PhysMove(&bank);
        // A system to check collisions between all spatial components according to collision components
        System_PhysCollide physicsCollisionSystem = System_PhysCollide(&bank);
        // A system to handle user input
        System_UserControl userControlSystem = System_UserControl(&bank, &controlDelegates);
        // A system to handle scripting input from the console or from a file.
        System_Scripting scriptingSystem = System_Scripting(&bank, &scriptingDelegates);
        // More systems to come...

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Engines on which to run the systems">

        // An engine (with accompanying thread) to run all graphics Systems
        SDL_Thread* graphicsThread = NULL;
        SystemEngine graphicsEngine = SystemEngine(&graphicsThread, "Duality Graphics Engine",
                                                   &outputDelegate, &quitDelegate);        
        // An engine (with accompanying thread) to run all physics Systems (and control system)
        SDL_Thread* physicsThread = NULL;
        SystemEngine physicsEngine = SystemEngine(&physicsThread, "Duality Physics Engine",
                                                  &outputDelegate, &quitDelegate);        
        // More engines to come...

        //</editor-fold>        

        //<editor-fold defaultstate="collapsed" desc="Low-level 'internal' methods">

        bool engageEngines();
        bool killSystems();
        bool cleanGameData();
        bool pauseBankDependentSystems();
        bool waitForBankDependentSystemsToPause();
        bool resumeSystems();
        void systems_discover(const DUA_id &ID);
        void systems_scrutinize(const DUA_id &ID);
        void systems_forceRemove(const DUA_id &ID, const DUA_compFlag& component);
        void submitScriptCommand(const std::string& command);
        // More internal functions to come...

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="High-level 'interface' methods">

        void NewGame();
        void RunScript(const std::string&);
        void LoadGame(const std::string&);
        void SaveGame(const std::string&);
        void Pause();
        void Resume();
        void Quit();
        // More top level functions to come...

        //</editor-fold>
        
    public:
        Game() {};
        ~Game() {};
        void Main();
    };

}
#endif	/* GAME_H */

