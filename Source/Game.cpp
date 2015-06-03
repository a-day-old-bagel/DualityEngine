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
void Game::Main(){
    
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
    cleanGameData();    
    resumeSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Load Game">
/**************************************
 * LOAD GAME
 *************************************/
void Game::LoadGame(const std::string& saveName){
    pauseSystems();
    cleanGameData();
    bank.load(saveName.c_str());
    resumeSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Save Game">
/**************************************
 * SAVE GAME
 *************************************/
void Game::SaveGame(const std::string& saveName){
    pauseSystems();
    bank.save(saveName.c_str());
    resumeSystems();
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
    graphicsEngine.addSystem(&renderModelsSystem);
    graphicsEngine.addSystem(&renderConsoleSystem);
    graphicsEngine.engage();
    
    physicsEngine.addSystem(&physicsMoveSystem);
    physicsEngine.addSystem(&physicsCollisionSystem);
    physicsEngine.addSystem(&userControlSystem);
    physicsEngine.addSystem(&scriptingSystem);
    physicsEngine.engage();
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Wait For Systems To Pause">
/**************************************
 * WAIT FOR SYSTEMS TO PAUSE
 *************************************/
bool Game::waitForSystemsToPause(){
    
    int startTime = SDL_GetTicks();
    bool done = false;
    while(!done){
        done = true;
        done &= renderingSystem.isPauseConfirmed();
        done &= renderConsoleSystem.isPauseConfirmed();
        done &= renderModelsSystem.isPauseConfirmed();
        done &= physicsMoveSystem.isPauseConfirmed();
        done &= physicsCollisionSystem.isPauseConfirmed();
        done &= userControlSystem.isPauseConfirmed();
        done &= scriptingSystem.isPauseConfirmed();
        
        if (SDL_GetTicks() - startTime > Settings::systemsPauseTimeout){
            return false;
        }
    }
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
    renderConsoleSystem.pause();
    renderModelsSystem.pause();
    userControlSystem.pause();
    scriptingSystem.pause();
    
    if (waitForSystemsToPause()){
        outputDelegate("All systems paused.\n");
        return true;
    } else {
        outputDelegate("Systems pause timed out!\n");
        resumeSystems();
        return false;
    }
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
    renderConsoleSystem.resume();
    renderModelsSystem.resume();
    userControlSystem.resume();
    scriptingSystem.resume();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Kill Systems">
/**************************************
 * KILL SYSTEMS
 * Tell each system to quit, which
 * should trigger their respective
 * engines to exit their loops.
 *************************************/
bool Game::killSystems(){
    renderModelsSystem.quit();
    renderConsoleSystem.quit();
    renderingSystem.quit();
    physicsMoveSystem.quit();
    physicsCollisionSystem.quit();
    userControlSystem.quit();
    scriptingSystem.quit();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Clean Game Data">
/**************************************
 * CLEAN GAME DATA
 * Only cleans the bank and the systems
 * that keep registry(s) of IDs on hand.
 * Systems that keep no registries do not
 * need to be cleaned.
 *************************************/
bool Game::cleanGameData(){
    renderModelsSystem.clean();
    physicsMoveSystem.clean();
    physicsCollisionSystem.clean();
    bank.clean();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Systems Discover">
/**************************************
 * SYSTEMS DISCOVER
 * Systems that keep no registries do not
 * need to discover.
 *************************************/
void Game::systems_discover(const DUA_id &ID){
    renderModelsSystem.discoverID(ID);
    physicsMoveSystem.discoverID(ID);
    physicsCollisionSystem.discoverID(ID);
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Systems Scrutinize">
/**************************************
 * SYSTEMS SCRUTINIZE
 * Systems that keep no registries do not
 * need to scrutinize.
 *************************************/
void Game::systems_scrutinize(const DUA_id &ID){
    renderModelsSystem.scrutinizeID(ID);
    physicsMoveSystem.scrutinizeID(ID);
    physicsCollisionSystem.scrutinizeID(ID);
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Submit Script Command">
/**************************************
 * SUBMIT SCRIPT COMMAND
 * sends a single line (command with args)
 * to the scripting system for parsing
 * and execution (or error handling)
 *************************************/
void Game::submitScriptCommand(const std::string& command){
    scriptingSystem.submitCommand(command);
}
//</editor-fold>