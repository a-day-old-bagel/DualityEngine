/************************************************
 * File:   Game.cpp
 * Author: adayoldbagel (Galen Cochrane)
 *
 * Created on January 27, 2015, 9:14 PM
 ************************************************/

#include "../Headers/Game.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Begin">
/**************************************
 * BEGIN
 * called once from where the game is
 * instantiated. Maybe will bring up a
 * main menu at some point...
 *************************************/
void Game::Begin(){
    NewGame();  
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Menu">
/**************************************
 * MENU
 *************************************/
void Game::Menu(){
    pauseSystems();
    char input;
    bool menuDone = false;
    std::cout << "****************************************\n"
              << "* MENU:\n"
              << "*    Q: Quit\n"
              << "*    N: New Game\n"
              << "*    R: Resume\n"
              << "****************************************\n"
              << ">: ";
    std::cin >> input;
    
    while (!menuDone){
        switch (input){
            case 'Q':
            case 'q':
                Quit();
                menuDone = true;
                break;
            case 'N':
            case 'n':
                NewGame();
                menuDone = true;
                break;
            case 'R':
            case 'r':
                resumeSystems();
                menuDone = true;
                break;
            default:
                std::cout << "bad input.\n>: ";
                std::cin >> input;
                break;
        }
    }
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="New Game">
/**************************************
 * NEW GAME : Game Initializer
 * sets up game
 *************************************/
void Game::NewGame(){ 
    engageEngines();
        
    // Wait for all game threads to exit, then the game is over.
    SDL_WaitThread(physicsThread, NULL);
    SDL_WaitThread(graphicsThread, NULL);
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Pause">
/**************************************
 * PAUSE
 *************************************/
void Game::Pause(){
    pauseSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume">
/**************************************
 * RESUME
 *************************************/
void Game::Resume(){
    resumeSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Quit">
/**************************************
 * QUIT
 *************************************/
void Game::Quit(){
    killSystems();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Engage Engines">
/**************************************
 * ENGAGE ENGINES
 * starts each engine, creating all
 * the threads in the process.
 *************************************/
bool Game::engageEngines(){
    graphicsEngine.addSystem(&renderingSystem);
    graphicsEngine.engage();
    
    physicsEngine.addSystem(&physicsMoveSystem);
    physicsEngine.addSystem(&physicsCollisionSystem);
    physicsEngine.addSystem(&userControlSystem);
    physicsEngine.engage();
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Pause Engines">
/**************************************
 * PAUSE ENGINES
 *************************************/
bool Game::pauseSystems(){
    physicsMoveSystem.pause();
    physicsCollisionSystem.pause();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume Engines">
/**************************************
 * RESUME ENGINES
 *************************************/
bool Game::resumeSystems(){
    physicsMoveSystem.resume();
    physicsCollisionSystem.resume();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Kill Engines">
/**************************************
 * QUIT ENGINES
 * Tell each system to quit, which
 * should trigger their respective
 * engines to exit their loops.
 * Then wait for threads to exit.
 *************************************/
bool Game::killSystems(){
    renderingSystem.quit();
    physicsMoveSystem.quit();
    physicsCollisionSystem.quit();
    userControlSystem.quit();    
    return true;
}
//</editor-fold>