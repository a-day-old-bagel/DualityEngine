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
    engageEngines();
        
    // Wait for all game threads to exit, then the game is over.
    SDL_WaitThread(physicsThread, NULL);
    SDL_WaitThread(graphicsThread, NULL);  
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="New Game">
/**************************************
 * NEW GAME : Game Initializer
 * sets up game
 *************************************/
void Game::NewGame(){ 
    pauseSystems();
    renderingSystem.clean();
    physicsMoveSystem.clean();
    physicsCollisionSystem.clean();
    userControlSystem.clean();
    bank.clean();
    resumeSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Load Game">
/**************************************
 * LOAD GAME
 *************************************/
void Game::LoadGame(){
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Save Game">
/**************************************
 * SAVE GAME
 *************************************/
void Game::SaveGame(){
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Pause">
/**************************************
 * PAUSE
 *************************************/
void Game::Pause(){
    physicsMoveSystem.pause();
    physicsCollisionSystem.pause();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume">
/**************************************
 * RESUME
 *************************************/
void Game::Resume(){
    physicsMoveSystem.resume();
    physicsCollisionSystem.resume();
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
//<editor-fold defaultstate="collapsed" desc="Pause Systems">
/**************************************
 * PAUSE ENGINES
 *************************************/
bool Game::pauseSystems(){
    physicsMoveSystem.pause();
    physicsCollisionSystem.pause();
    renderingSystem.pause();
    userControlSystem.pause();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume Systems">
/**************************************
 * RESUME ENGINES
 *************************************/
bool Game::resumeSystems(){
    physicsMoveSystem.resume();
    physicsCollisionSystem.resume();
    renderingSystem.resume();
    userControlSystem.resume();
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
//<editor-fold defaultstate="collapsed" desc="Systems Discover">
/**************************************
 * SYSTEMS DISCOVER
 *************************************/
void Game::systems_discover(const DUA_ID &ID){
    renderingSystem.discoverID(ID);
    physicsMoveSystem.discoverID(ID);
    physicsCollisionSystem.discoverID(ID);
    userControlSystem.discoverID(ID);
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Systems Scrutinize">
/**************************************
 * SYSTEMS SCRUTINIZE
 *************************************/
void Game::systems_scrutinize(const DUA_ID &ID){
    renderingSystem.scrutinizeID(ID);
    physicsMoveSystem.scrutinizeID(ID);
    physicsCollisionSystem.scrutinizeID(ID);
    userControlSystem.scrutinizeID(ID);
}
//</editor-fold>