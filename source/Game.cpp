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
 * CONSTRUCTOR
 * Before having to accommodate
 * Window's ridiculous lack of support
 * for some c++11 features, this
 * constructor was actually pretty.
 *************************************/
Game::Game() :  bank(&bankDelegates),
                token(ceq.requestProducerToken()),
                renderMasterSystem(&bank),
                renderConsoleSystem(&bank, &console),
                renderModelsSystem(&bank),
                renderBackgroundSystem(&bank),
                physicsMoveSystem(&bank),
                physicsCollisionSystem(&bank),
                spaceShipControlSystem(&bank),
                userInputSystem(&bank),
                scriptingSystem(&bank),
				graphicsEngine(&graphicsThread, "Duality Graphics Engine",
								&bankDelegates.output, &bankDelegates.quit, &renderMasterSystem, &renderBackgroundSystem,
								&renderModelsSystem, &renderConsoleSystem),
                physicsEngine(&physicsThread, "Duality Physics Engine", &bankDelegates.output, &bankDelegates.quit,
                              &userInputSystem, &spaceShipControlSystem, &physicsMoveSystem, &physicsCollisionSystem),
                scriptingEngine(&scriptingThread, "Duality Scripting Engine",
                                &bankDelegates.output, &bankDelegates.quit, &scriptingSystem)
	{
    graphicsThread = NULL;
    physicsThread = NULL;
    scriptingThread = NULL;
    isAlive = true;

    bankDelegates = {
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
	};
}

/**************************************
 * DESTRUCTOR
 *************************************/
Game::~Game() {
    // Save game console log as text file
    std::ofstream logFile;
    logFile.open("log.txt", std::ios::trunc);
    logFile << console.getLog();
    logFile.close();
}

void Game::sayHi() {
    bankDelegates.output("HI!\n");
}

/**************************************
 * MAIN
 * called once from where the game is
 * instantiated. Maybe will bring up a
 * main menu at some point...
 *************************************/
void Game::Main(){

    engageEngines();

    Delegate<void(void)> sayHiDelegate = DELEGATE(&Game::sayHi, this);
    Event event;
    event.type = Event::VOIDVOID;
    event.voidVoid = sayHiDelegate;
    ceq.newEvent(token, event);

    // Start the event handling loop, to exit only when game exits
    while (isAlive) {
        handleEvents();
        SDL_Delay(1);
    }

    // Wait for all game threads to exit, then the game is over.
	SDL_WaitThread(graphicsThread, NULL);
    SDL_WaitThread(physicsThread, NULL);    
    SDL_WaitThread(scriptingThread, NULL);

    // Handle any last events that are still laying around
    handleEvents();
}

/****** INTERFACE METHODS ******/

/**************************************
 * NEW GAME : Game Initializer
 * sets up game
 *************************************/
void Game::NewGame(){ 
    if (pauseBankDependentSystems()){
        cleanGameData();    
        resumeBankDependentSystems();
        bankDelegates.output("World cleared.\n");
    }
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
    bank.timePiece.pause();
}

/**************************************
 * RESUME
 * resumes the systems paused by Pause().
 *************************************/
void Game::Resume(){
    physicsMoveSystem.resume();
    physicsCollisionSystem.resume();
    spaceShipControlSystem.resume();
    bank.timePiece.resume();
}

/**************************************
 * QUIT
 *************************************/
void Game::Quit(){
    killSystems();
    isAlive = false;
}

/****** INTERNAL METHODS ******/

/**************************************
 * ENGAGE ENGINES
 *************************************/
bool Game::engageEngines(){
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
        bankDelegates.output("bank-dependent systems' pause timed out!\n");
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
 * systems that keep no registries do not
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
 * systems that keep no registries do not
 * need to discover.
 *************************************/
void Game::systems_discover(const DUA_id ID){
    renderModelsSystem.discoverID(ID);
    physicsMoveSystem.discoverID(ID);
    physicsCollisionSystem.discoverID(ID);
}

/**************************************
 * SYSTEMS SCRUTINIZE
 * systems that keep no registries do not
 * need to scrutinize.
 *************************************/
void Game::systems_scrutinize(const DUA_id ID){
    renderModelsSystem.scrutinizeID(ID);
    physicsMoveSystem.scrutinizeID(ID);
    physicsCollisionSystem.scrutinizeID(ID);
}

/**************************************
 * SYSTEMS FORCE REMOVE
 * systems that keep no registries do not
 * need to do this either.
 *************************************/
void Game::systems_forceRemove(const DUA_id ID, const DUA_compFlag component){
    renderModelsSystem.forceRemoveComp(ID, component);
    physicsMoveSystem.forceRemoveComp(ID, component);
    physicsCollisionSystem.forceRemoveComp(ID, component);
}
void Game::handleEvents() {
    Event currentEvent;
    bool thereAreMoreEvents = ceq.getNext(currentEvent);
    while (thereAreMoreEvents) {
        switch(currentEvent.type) {
            case Event::VOIDVOID:
                currentEvent.voidVoid();
                break;
            case Event::VOIDCSTRING:
                currentEvent.voidCstring("C-String Event");
                break;
            case Event::VOIDSTRING:
                currentEvent.voidString("String Event");
                break;

            default:
                break;
        }
        thereAreMoreEvents = ceq.getNext(currentEvent);
    }
}
