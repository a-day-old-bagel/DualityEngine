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
#include "Render_UI.h"
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
        Bank bank = Bank(&bankDelegates);

        /*************
         * The Central Event Queue (CEQ) to provide a universal event queue to all systems
         *************/
        EventQueue ceq;
        Token token = Token(ceq.requestProducerToken());
        bool isAlive = true;

        /*************
         * SYSTEMS to operate on the components, providing game mechanics
         *************/
        System_Render_Master renderMasterSystem         = System_Render_Master(&bank);
        System_Render_UI renderConsoleSystem       = System_Render_UI(&bank, &console);
        System_Render_Models renderModelsSystem         = System_Render_Models(&bank);
        System_Render_Background renderBackgroundSystem = System_Render_Background(&bank);
        System_PhysMove physicsMoveSystem               = System_PhysMove(&bank);
        System_PhysCollide physicsCollisionSystem       = System_PhysCollide(&bank);
        System_Control_SS spaceShipControlSystem        = System_Control_SS(&bank);
        System_UserInput userInputSystem                = System_UserInput(&bank);
        System_Scripting scriptingSystem                = System_Scripting(&bank);

        /*************
         * ENGINES (thread with accompanying SystemEngine) on which to run the systems
         *************/
		typedef SystemEngine <
                System_Render_Master*,
                System_Render_Background*,
                System_Render_Models*,
                System_Render_UI*
        > DUA_graphicsEngine;

        SDL_Thread* graphicsThread = NULL;
		DUA_graphicsEngine graphicsEngine = DUA_graphicsEngine (
                &graphicsThread, "Duality Graphics Engine", &bankDelegates.output, &bankDelegates.quit,
                &renderMasterSystem,
                &renderBackgroundSystem,
                &renderModelsSystem,
                &renderConsoleSystem
        );

        typedef SystemEngine <
                System_UserInput*,
                System_Control_SS*,
                System_PhysMove*,
                System_PhysCollide*
        > DUA_physicsEngine;

        SDL_Thread* physicsThread = NULL;
        DUA_physicsEngine physicsEngine = DUA_physicsEngine (
                &physicsThread, "Duality Physics Engine", &bankDelegates.output, &bankDelegates.quit,
                &userInputSystem,
                &spaceShipControlSystem,
                &physicsMoveSystem,
                &physicsCollisionSystem);

        typedef SystemEngine <
                System_Scripting*
        > DUA_scriptingEngine;

        SDL_Thread* scriptingThread = NULL;
        DUA_scriptingEngine scriptingEngine = DUA_scriptingEngine (
                &scriptingThread, "Duality Scripting Engine", &bankDelegates.output, &bankDelegates.quit,
                &scriptingSystem);

        /*************
         * DELEGATES to allow for inter-system/engine communication (they allow pretty much any function to be called
         * from anywhere, as long as you have a pointer to one of these delegates. So just watch who you give them to.)
         * All delegates are contained in the BankDelegates struct, to which bank has a public pointer.
         * This is so that anything with access to the bank has access to the delegates.
         *************/
        BankDelegates bankDelegates = {
                DELEGATE(&Game::systems_discover, this),
                DELEGATE(&Game::systems_scrutinize, this),
                DELEGATE(&Game::systems_forceRemove, this),
                DELEGATE(&Game::Quit, this),
                DELEGATE(&Game::NewGame, this),
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
                DELEGATE(&Console::output, &console),
                DELEGATE(&Console::outputStr, &console),
                DELEGATE(&Console::addToCommand, &console),
                DELEGATE(&Console::submitCommand, &console),
                DELEGATE(&System_Scripting::submitCommand, &scriptingSystem),
                DELEGATE(&Console::getLogLineFromBack, &console),
                DELEGATE(&Console::getCurrentLogLine, &console),
                DELEGATE(&Console::setState, &console),
                DELEGATE(&Console::traverseLog, &console),
                DELEGATE(&System_Render_Background::queueSkyChange, &renderBackgroundSystem)
        };;

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

