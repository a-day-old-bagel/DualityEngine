/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/System.h"

using namespace DualityEngine;

System::System(ComponentBank* bank, std::string name, int numRegisters){
    this->bank = bank;
    this->name = name;
    for (int i = 0; i < numRegisters; i++){
        registeredIDs.push_back(std::vector<DUA_id>());
        requiredComponents.push_back(DUA_DEFAULT_COMPONENTS);
    }
}

System::~System(){
    bank = NULL;
}

void System::clean(){
    int numRegisters = registeredIDs.size();
    registeredIDs = std::vector<std::vector<DUA_id>>();
    for (int i = 0; i < numRegisters; i++){
        registeredIDs.push_back(std::vector<DUA_id>());
    }
}

std::string System::getName(){
    return name;
}

void System::discoverID(const DUA_id& ID){
    // for each instance of registeredIDs and corresponding instance of requiredComponents...
    for (uint i = 0; i < registeredIDs.size(); ++i){
        // If the entity (the ID) isn't already in the list of IDs
        if (std::find(registeredIDs[i].begin(), registeredIDs[i].end(), ID) == registeredIDs[i].end()){
            // If the entity (the ID) has all the required components
            if ((bank->getComponents(ID) & requiredComponents[i]) == requiredComponents[i]){
                registeredIDs[i].push_back(ID);    // add the entity (ID) to the list
            }
        }
    }
}

void System::scrutinizeID(const DUA_id& ID){
    for (uint i = 0; i < registeredIDs.size(); ++i){
        // get location (if any) of ID in registeredIDs
        std::vector<DUA_id>::iterator it =
            std::find(registeredIDs[i].begin(), registeredIDs[i].end(), ID);
        // if the ID is in registeredIDs...
        if (it != registeredIDs[i].end()){
            // if the entity no longer has the required components
            if ((bank->getComponents(ID) & requiredComponents[i]) != requiredComponents[i])
                registeredIDs[i].erase(it);    // remove that entity (ID) from the list
        }
    }
}

void System::forceRemoveComp(const DUA_id& ID, const DUA_compFlag& component){
    for (uint i = 0; i < registeredIDs.size(); ++i){
        if (requiredComponents[i] & component){
            std::vector<DUA_id>::iterator it =
            std::find(registeredIDs[i].begin(), registeredIDs[i].end(), ID);
            if (it != registeredIDs[i].end()){
                registeredIDs[i].erase(it);    // remove that entity (ID) from the list
            }
        }
    }
}

void System::pause(){
    if (!paused){
        paused = true;
    }
}

void System::confirmPaused(){
    pauseConfirmed = true;
}

void System::resume(){
    if (paused){
        paused = false;
        pauseConfirmed = false;
    }
}

void System::quit(){
    if (!escape){
        escape = true;
    }
}

bool System::isPaused(){
    return paused;
}

bool System::isPauseConfirmed(){
    return pauseConfirmed;
}

bool System::isQuit(){
    return escape;
}
