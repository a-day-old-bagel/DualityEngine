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
#include "ControlDelegateBag.h"
//</editor-fold>

namespace DualityEngine {

    class Game
    {
    private:
        //<editor-fold defaultstate="collapsed" desc="Members">

        //<editor-fold defaultstate="collapsed" desc="A window in which to play">

        SDL_Window* window;

        //</editor-fold>    
        //<editor-fold defaultstate="collapsed" desc="Assets - meshes, textures, sounds, etc.">

        // Nothing yet

        //</editor-fold>    
        //<editor-fold defaultstate="collapsed" desc="Components that reference the assets">

        ComponentBank bank; // holds all the game data, stored as collections of components

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Systems to operate on the components and draw from the assets">

        // A system to render all graphical components (manages openGL calls)
        System_Render* renderingSystem;
        // A system to update all spatial components according to motion components
        System_PhysMove* physicsMoveSystem;
        // A system to check collisions between all spatial components according to collision components
        System_PhysCollide* physicsCollisionSystem;
        // A system to handle user input, along with delegates of top level functions
        System_UserControl* userControlSystem;
        DelegateBag* controlDelegates;
        // More systems to come...

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="Engines on which to run the systems">

        // A delegate for quitting the game to give to the engines in case of failures
        VoidDelegate* quitDelegate;
        // An engine (with accompanying thread) to run all graphics Systems
        SystemEngine* graphicsEngine;
        SDL_Thread* graphicsThread;
        // An engine (with accompanying thread) to run all physics Systems
        SystemEngine* physicsEngine;
        SDL_Thread* physicsThread;
        // More engines to come...

        //</editor-fold>

        //</editor-fold>    
        //<editor-fold defaultstate="collapsed" desc="Methods">

        //<editor-fold defaultstate="collapsed" desc="Low-level 'internal' methods">

        // Set up the SDL window
        bool initializeSDLwindow();
        // Set up the components and systems (entities are implied)
        bool initializeECS();
        // Set up the engines
        bool initializeEngines();
        // Start the engines (create the threads)
        bool startEngines();
        // Quit the the systems, wait for the threads to exit.
        bool killEngines();
        // Pause game systems momentarily
        bool pauseEngines();
        // Resume game systems from a paused state
        bool resumeEngines();
        // Set all fields to NULL
        void nullifyPointers();
        // De-allocate memory
        void freeMemory();
        // More internal functions to come...

        //</editor-fold>
        //<editor-fold defaultstate="collapsed" desc="High-level 'interface' methods">

        // pull up the in-game menu
        void Menu();
        // start a new game
        bool NewGame();
        // pause the game
        void Pause();
        // resume the game
        void Resume();
        // quit the game
        void Quit();
        // More top level functions to come...

        //</editor-fold>

        //</editor-fold>

    public:
        //<editor-fold defaultstate="collapsed" desc="Methods">
        Game();
        ~Game();
        bool Begin();
        //</editor-fold>
    };

}
#endif	/* GAME_H */

