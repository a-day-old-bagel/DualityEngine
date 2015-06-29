/************************************************
 * File:   Game.cpp
 * Author: adayoldbagel (Galen Cochrane)
 *
 * Created on January 27, 2015, 9:14 PM
 ************************************************/

#include <fstream>
#include "../Headers/Game.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="MAIN">
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
    
    // Save game console log as text file
    std::ofstream logFile;
    logFile.open("log.txt", std::ios::trunc);
    logFile << console.getLog();
    logFile.close();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="New Game">
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
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Run Script">
/**************************************
 * RUN SCRIPT
 *************************************/
void Game::RunScript(const std::string& fileName){
    std::string filePath = "Assets/Scripts/" + fileName + ".dua";
    std::vector<std::string> lines;
    std::string lineReader;
    std::ifstream infile (filePath, std::ios_base::in);
    while (getline(infile, lineReader, '\n')){
      lines.push_back (lineReader);
    }
    
    if (lines.empty()){
        outputStrDelegate(filePath + ": file either not found, unreadable, or empty.\n");
    } else {
        for (int i = 0; i < lines.size();){
            if (lines.at(i).empty() || lines.at(i).at(0) == '#'){
                lines.erase(lines.begin() + i);
            } else {
                ++i;
            }
        }
        
        if (lines.size() < 2){
            outputStrDelegate(filePath + ": not a valid Duality script.\n");
        } else {
            std::stringstream firstTwoLines(lines.at(0) + " " + lines.at(1));
            std::string headerTypeDeclaration, headerVersion, headerNumEntities;
            firstTwoLines >> headerTypeDeclaration;
            firstTwoLines >> headerVersion;
            firstTwoLines >> headerNumEntities;
            headerNumEntities.clear();
            firstTwoLines >> headerNumEntities;
            if (headerTypeDeclaration != "DualityEngineScript" || headerVersion != DUA_VERSION){
                outputStrDelegate(filePath + ": invalid Duality script or wrong version.");
            } else {

                // DO SOMETHING WITH NUMENTITIES - RESERVE SPACE IN HASH TABLES, ETC.

                lines.erase(lines.begin(), lines.begin() + 1);

                if (pauseBankDependentSystems()){
                    SDL_Delay(1);
                    for (auto line : lines){
                        scriptingSystem.submitCommand(line);
                        //SDL_Delay(1);       // MAKES MORE STABLE FOR SOME REASON - FIX THIS.
                    }
                    resumeBankDependentSystems();
                }

                //outputStrDelegate(fileName + " script has completed.\n");
            }
        }
    }
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Load Game">
/**************************************
 * LOAD GAME
 *************************************/
void Game::LoadGame(const std::string& saveName){
    pauseBankDependentSystems();
    cleanGameData();
    bank.load(saveName.c_str());
    resumeBankDependentSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Save Game">
/**************************************
 * SAVE GAME
 *************************************/
void Game::SaveGame(const std::string& saveName){
    pauseBankDependentSystems();
    bank.save(saveName.c_str());
    resumeBankDependentSystems();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Pause">
/**************************************
 * PAUSE
 * only pauses the systems that make the
 * game world seem dynamic, such as the
 * movement systems and the collision system.
 * Pausing rendering systems would
 * look crappy and pausing the control systems
 * would just be stupid.
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
    graphicsEngine.addSystem(&renderMasterSystem);
    graphicsEngine.addSystem(&renderBackgroundSystem);
    graphicsEngine.addSystem(&renderModelsSystem);
    graphicsEngine.addSystem(&renderConsoleSystem);
    graphicsEngine.engage();
    
    physicsEngine.addSystem(&physicsMoveSystem);
    physicsEngine.addSystem(&physicsCollisionSystem);
    physicsEngine.addSystem(&userControlSystem);
    physicsEngine.engage();
    
    scriptingEngine.addSystem(&scriptingSystem);
    scriptingEngine.engage();
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Wait For Systems To Pause">
/**************************************
 * WAIT FOR SYSTEMS TO PAUSE
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
        done &= userControlSystem.isPauseConfirmed();
        
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
bool Game::pauseBankDependentSystems(){
    physicsMoveSystem.pause();
    physicsCollisionSystem.pause();     // ADD CONTROL SYSTEM
    renderModelsSystem.pause();
    userControlSystem.pause();
    renderBackgroundSystem.pause();
    renderMasterSystem.pause();
    renderConsoleSystem.pause();
    
    if (waitForBankDependentSystemsToPause()){
//        outputDelegate("All bank-dependent systems paused.\n");
        return true;
    } else {
        outputDelegate("bank-dependent systems' pause timed out!\n");
        resumeBankDependentSystems();
        return false;
    }
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume Systems">
/**************************************
 * RESUME ENGINES
 *************************************/
bool Game::resumeBankDependentSystems(){
    if (!console.menuIsActive){ // to not accidentally resume game before menu is exited
        physicsMoveSystem.resume();
        physicsCollisionSystem.resume();
    }
    renderMasterSystem.resume();
    renderConsoleSystem.resume();
    renderModelsSystem.resume();
    renderBackgroundSystem.resume();
    userControlSystem.resume();
    
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
    renderMasterSystem.quit();
    renderBackgroundSystem.quit();
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
    renderBackgroundSystem.clean();
    physicsMoveSystem.clean();
    physicsCollisionSystem.clean();
    userControlSystem.clean();
    
//    renderMasterSystem = System_Render_Master(&bank);
//    renderConsoleSystem = System_Render_Console(&bank, &console);
//    renderModelsSystem = System_Render_Models(&bank);
//    physicsMoveSystem = System_PhysMove(&bank);
//    physicsCollisionSystem = System_PhysCollide(&bank);
//    userControlSystem = System_UserControl(&bank, &controlDelegates);
//    scriptingSystem = System_Scripting(&bank, &scriptingDelegates);
    
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
//<editor-fold defaultstate="collapsed" desc="Systems Force Remove">
/**************************************
 * SYSTEMS FORCE REMOVE
 * Systems that keep no registries do not
 * need to do this either.
 *************************************/
void Game::systems_forceRemove(const DUA_id& ID, const DUA_compFlag& component){
    renderModelsSystem.forceRemoveComp(ID, component);
    physicsMoveSystem.forceRemoveComp(ID, component);
    physicsCollisionSystem.forceRemoveComp(ID, component);
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