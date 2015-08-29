/************************************************
 * File:   Game.cpp
 * Author: adayoldbagel (Galen Cochrane)
 *
 * Created on January 27, 2015, 9:14 PM
 ************************************************/

#include <fstream>
#include "Game.h"

using namespace DualityEngine;

/**************************************
 * MAIN
 * called once from where the game is
 * instantiated. Maybe will bring up a
 * main menu at some point...
 *************************************/
void Game::Main(){
    
    engageEngines();
        
    // Wait for all game threads to exit, then the game is over.
    SDL_WaitThread(physicsThread, NULL);
    SDL_WaitThread(graphicsThread, NULL);
    SDL_WaitThread(scriptingThread, NULL);   
}

/**************************************
 * DESTRUCTOR
 *************************************/
Game::~Game(){
    // Save game console log as text file
    std::ofstream logFile;
    logFile.open("log.txt", std::ios::trunc);
    logFile << console.getLog();
    logFile.close();
}


/*******************************************************************************
 *                  INTERNAL METHODS
 ******************************************************************************/

/**************************************
 * NEW GAME : Game Initializer
 * sets up game
 *************************************/
void Game::NewGame(){ 
    if (pauseBankDependentSystems()){
        cleanGameData();    
        resumeBankDependentSystems();
        outputDelegate("World cleared.\n");
    }
    
//    cleanGameData();    
//    resumeBankDependentSystems();
//    outputDelegate("World cleared.\n");
}

/**************************************
 * LOAD GAME
 *************************************/
void Game::LoadGame(const std::string& saveName){
    pauseBankDependentSystems();
    cleanGameData();
    bank.load(saveName.c_str());
    resumeBankDependentSystems();
}

/**************************************
 * SAVE GAME
 *************************************/
void Game::SaveGame(const std::string& saveName){
    pauseBankDependentSystems();
    bank.save(saveName.c_str());
    resumeBankDependentSystems();
}

/**************************************
 * PAUSE
 * only pauses the systems that make the
 * game world seem dynamic, such as the
 * physics movement systems
 *************************************/
void Game::Pause(){
    physicsMoveSystem.pause();
    physicsCollisionSystem.pause();
    spaceShipControlSystem.pause();
}

/**************************************
 * RESUME
 * resumes the systems paused by Pause().
 *************************************/
void Game::Resume(){
    physicsMoveSystem.resume();
    physicsCollisionSystem.resume();
    spaceShipControlSystem.resume();
}

/**************************************
 * QUIT
 *************************************/
void Game::Quit(){
    killSystems();
}


/*******************************************************************************
 *                  INTERNAL METHODS
 ******************************************************************************/

/**************************************
 * ENGAGE ENGINES
 * assigns the appropriate systems to
 * each engine then starts the engines.
 * Systems' tick functions will receive
 * looping calls in the order in which
 * they are added to their respective
 * engines. See below for examples.
 *************************************/
bool Game::engageEngines(){
    
    physicsEngine.addSystem(&userInputSystem);          // accept user input
    physicsEngine.addSystem(&spaceShipControlSystem);   // apply user input to any currently active space control component
    physicsEngine.addSystem(&physicsMoveSystem);        // apply velocity * time to each position
    physicsEngine.addSystem(&physicsCollisionSystem);   // check collisions between objects        
    
    scriptingEngine.addSystem(&scriptingSystem);        // check for new commands in the queue
    
    graphicsEngine.addSystem(&renderMasterSystem);      // display last frame buffer then clear the buffer
    graphicsEngine.addSystem(&renderBackgroundSystem);  // render the background (sky) first
    graphicsEngine.addSystem(&renderModelsSystem);      // render the models next
    graphicsEngine.addSystem(&renderConsoleSystem);     // render the console (GUI) last (on top).

    physicsEngine.engage();
    scriptingEngine.engage();
    graphicsEngine.engage();
    
    return true;
}

/**************************************
 * WAIT FOR SYSTEMS TO PAUSE
 * this is called by the pause engines
 * method and waits until it is certain
 * that all bank-dependent systems are
 * paused before returning.
 *************************************/
bool Game::waitForBankDependentSystemsToPause(){
    
    int startTime = SDL_GetTicks();
    bool done = false;
    while(!done){
        done = true;
        done &= renderModelsSystem.isPauseConfirmed();
        done &= physicsMoveSystem.isPauseConfirmed();
        done &= renderBackgroundSystem.isPauseConfirmed();
        done &= renderMasterSystem.isPauseConfirmed();
        done &= renderConsoleSystem.isPauseConfirmed();
        done &= physicsCollisionSystem.isPauseConfirmed();  //PROBLEMS WITH THIS BEING EXECUTED FROM CONTROL SYS... THREAD LOCKS
        done &= userInputSystem.isPauseConfirmed();         
        done &= spaceShipControlSystem.isPauseConfirmed();
        
        if (SDL_GetTicks() - startTime > Settings::Systems::systemsPauseTimeout){
            return false;
        }
    }
    return true;
}

/**************************************
 * PAUSE ENGINES
 * this is used whenever some major
 * operation needs to be done on the
 * bank (I don't really know
 * everything that might happen to the
 * running systems if I just start
 * changing things in the bank, but
 * invalidating pointers kept in systems
 * will probably happen).
 *************************************/
bool Game::pauseBankDependentSystems(){
    physicsMoveSystem.pause();
    physicsCollisionSystem.pause();
    renderModelsSystem.pause();
    userInputSystem.pause();
    renderBackgroundSystem.pause();
    renderMasterSystem.pause();
    renderConsoleSystem.pause();
    spaceShipControlSystem.pause();
    
    if (waitForBankDependentSystemsToPause()){
        return true;
    } else {
        outputDelegate("bank-dependent systems' pause timed out!\n");
        resumeBankDependentSystems();
        return false;
    }
}

/**************************************
 * RESUME ENGINES
 * resumes the engines paused by the
 * pause-bank-dependent systems method.
 *************************************/
bool Game::resumeBankDependentSystems(){
    if (!console.menuIsActive){ // to not accidentally resume game before menu is exited
        physicsMoveSystem.resume();
        physicsCollisionSystem.resume();
        spaceShipControlSystem.resume();
    }
    renderMasterSystem.resume();
    renderConsoleSystem.resume();
    renderModelsSystem.resume();
    renderBackgroundSystem.resume();
    userInputSystem.resume();    
    
    return true;
}

/**************************************
 * KILL SYSTEMS
 * Tell each system to quit, which
 * should trigger their respective
 * engines to exit their loops.
 *************************************/
bool Game::killSystems(){
    renderModelsSystem.quit();
    renderConsoleSystem.quit();
    renderMasterSystem.quit();
    renderBackgroundSystem.quit();
    physicsMoveSystem.quit();
    physicsCollisionSystem.quit();
    userInputSystem.quit();
    scriptingSystem.quit();
    spaceShipControlSystem.quit();
    return true;
}

/**************************************
 * CLEAN GAME DATA
 * Only cleans the bank and the systems
 * that keep registry(s) of IDs on hand.
 * Systems that keep no registries do not
 * need to be cleaned.
 *************************************/
bool Game::cleanGameData(){
    renderModelsSystem.clean();
    renderBackgroundSystem.clean();
    physicsMoveSystem.clean();
    physicsCollisionSystem.clean();
    userInputSystem.clean();
    
    bank.clean();
    return true;
}

/**************************************
 * SYSTEMS DISCOVER
 * Systems that keep no registries do not
 * need to discover.
 *************************************/
void Game::systems_discover(const DUA_id ID){
    renderModelsSystem.discoverID(ID);
    physicsMoveSystem.discoverID(ID);
    physicsCollisionSystem.discoverID(ID);
}

/**************************************
 * SYSTEMS SCRUTINIZE
 * Systems that keep no registries do not
 * need to scrutinize.
 *************************************/
void Game::systems_scrutinize(const DUA_id ID){
    renderModelsSystem.scrutinizeID(ID);
    physicsMoveSystem.scrutinizeID(ID);
    physicsCollisionSystem.scrutinizeID(ID);
}

/**************************************
 * SYSTEMS FORCE REMOVE
 * Systems that keep no registries do not
 * need to do this either.
 *************************************/
void Game::systems_forceRemove(const DUA_id ID, const DUA_compFlag component){
    renderModelsSystem.forceRemoveComp(ID, component);
    physicsMoveSystem.forceRemoveComp(ID, component);
    physicsCollisionSystem.forceRemoveComp(ID, component);
}