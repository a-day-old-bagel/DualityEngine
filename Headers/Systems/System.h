/* 
 * File:   System.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 9:43 AM
 *
 * This is the base class for all systems.  It uses the curiously recurring template pattern and avoids any
 * virtual functions.  The only reason to use inheritance and have a base class at all is just to allow for less
 * typing, as nearly all systems should need the following members and methods.
 *
 * Due to the use of templated functions, code that would normally be split into .h and .cpp files all appears here.
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

#include "ComponentBank.h"

namespace DualityEngine {

    //  ***************************  HEADERS  ***************************

    template<typename Derived_System>
    class System
    {
    private:
        std::string name;
        bool escape;
        bool paused;
        bool pauseConfirmed;
        Derived_System& sys();
        
    protected:
        ComponentBank* bank;
        std::vector<std::vector<DUA_id>> registeredIDs;   // These are collections so that a system can operate
        std::vector<DUA_compFlag> requiredComponents;     // on more than one kind of entity if it needs to.
        uint tockFreq; // every 'tockFreq' times 'tock' is called, it's code will execute. (periodic loop not linked to 'tick')
		uint counter;
        
    public:        
        System(ComponentBank* bank, std::string name, int numRegisters);
        ~System();
        void clean();
        std::string getName();
        void discoverID (const DUA_id& ID);
        void scrutinizeID (const DUA_id& ID);
        void forceRemoveComp(const DUA_id& ID, const DUA_compFlag&);
        bool init(std::stringstream& output){ return false; }
        void tick();
        void tock();
        void pause();
        void resume();
        void confirmPaused();
        void quit();

        bool isPaused();
        bool isPauseConfirmed();
        bool isQuit();
    };

    //  ***************************  IMPLEMENTATION  ***************************

    template<typename Derived_System>
    System<Derived_System>::System(ComponentBank* bank, std::string name, int numRegisters){
        escape = false;
        paused = false;
        pauseConfirmed = false;
        this->bank = bank;
        this->name = name;
        for (int i = 0; i < numRegisters; i++){
            registeredIDs.push_back(std::vector<DUA_id>());
            requiredComponents.push_back(DUA_DEFAULT_COMPONENTS);
        }
        tockFreq = 1;
		counter = 0;
    }

    template<typename Derived_System>
    System<Derived_System>::~System(){
        bank = NULL;
    }

    template<typename Derived_System>
    Derived_System& System<Derived_System>::sys() {
        return *static_cast<Derived_System*>(this);
    }

    template<typename Derived_System>
    void System<Derived_System>::tick() {
        sys().tickImpl();
		++counter %= tockFreq;
		if (!counter) {
			tock();
		}
    }

    template<typename Derived_System>
    void System<Derived_System>::tock() {
        sys().tockImpl();
    }

    template<typename Derived_System>
    void System<Derived_System>::clean(){
        int numRegisters = registeredIDs.size();
        registeredIDs = std::vector<std::vector<DUA_id>>();
        for (int i = 0; i < numRegisters; i++){
            registeredIDs.push_back(std::vector<DUA_id>());
        }
    }

    template<typename Derived_System>
    std::string System<Derived_System>::getName(){
        return name;
    }

    template<typename Derived_System>
    void System<Derived_System>::discoverID(const DUA_id& ID){
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

    template<typename Derived_System>
    void System<Derived_System>::scrutinizeID(const DUA_id& ID){
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

    template<typename Derived_System>
    void System<Derived_System>::forceRemoveComp(const DUA_id& ID, const DUA_compFlag& component){
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

    template<typename Derived_System>
    void System<Derived_System>::pause(){
        if (!paused){
            paused = true;
        }
    }

    template<typename Derived_System>
    void System<Derived_System>::confirmPaused(){
        pauseConfirmed = true;
    }

    template<typename Derived_System>
    void System<Derived_System>::resume(){
        if (paused){
            paused = false;
            pauseConfirmed = false;
        }
    }

    template<typename Derived_System>
    void System<Derived_System>::quit(){
        if (!escape){
            escape = true;
        }
    }

    template<typename Derived_System>
    bool System<Derived_System>::isPaused(){
        return paused;
    }

    template<typename Derived_System>
    bool System<Derived_System>::isPauseConfirmed(){
        return pauseConfirmed;
    }

    template<typename Derived_System>
    bool System<Derived_System>::isQuit(){
        return escape;
    }
}

#endif	/* SYSTEM_H */

