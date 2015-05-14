/* 
 * File:   Game.h
 * Author: adayoldbagel
 *
 * Created on February 7, 2015, 1:14 PM
 */

#ifndef GAME_H
#define	GAME_H

//<editor-fold defaultstate="collapsed" desc="Includes">
#include "Render.h"
#include "PhysMove.h"
#include "PhysCollide.h"
#include "UserControl.h"
#include "Engine.h"
#include "ControlDelegates.h"
#include "Console.h"
//</editor-fold>

namespace DualityEngine {

    class Game
    {
    private:
   
        //<editor-fold defaultstate="collapsed" desc="Components that hold all game state data">

        ComponentBank bank; // holds collections of components, includes methods to manage them

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="An in-game console and output logging device">

        Console console;

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Delegates to allow for inter-system/engine communication">
        
        // A delegate for quitting the game to give to the engines in case of failures
        Delegate<void()> quitDelegate = DELEGATE(&Game::Quit, this);
        // A delegate for outputting text to the console to give to the engines
        Delegate<void(const char*)> outputDelegate = DELEGATE(&Console::output, &console);
        // control delegates of top level functions to give to the UserControl system
        ControlDelegates controlDelegates = {
            DELEGATE(&Game::Menu, this), quitDelegate,
            DELEGATE(&Console::eraseOneCharFromCommand, &console),
            DELEGATE(&Console::clearCommand, &console), outputDelegate,
            DELEGATE(&Console::addToCommand, &console),
            DELEGATE(&Console::submitCommand, &console),
            DELEGATE(&Console::getLast, &console)
        };
        
        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Systems to operate on the components, providing game mechanics">

        // A system to render all graphical components (manages openGL calls)
        System_Render renderingSystem = System_Render(&bank);
        // A system to update all spatial components according to motion components
        System_PhysMove physicsMoveSystem = System_PhysMove(&bank);
        // A system to check collisions between all spatial components according to collision components
        System_PhysCollide physicsCollisionSystem = System_PhysCollide(&bank);
        // A system to handle user input
        System_UserControl userControlSystem = System_UserControl(&bank, &controlDelegates);
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
        bool pauseSystems();
        bool resumeSystems();
        // More internal functions to come...

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="High-level 'interface' methods">

        void Menu();
        void NewGame();
        void Pause();
        void Resume();
        void Quit();
        // More top level functions to come...

        //</editor-fold>
        
    public:
        Game() {};
        ~Game() {};
        void Begin();
    };

}
#endif	/* GAME_H */

