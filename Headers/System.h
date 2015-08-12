/* 
 * File:   System.h
 * Author: adayoldbagel
 *
 * Created on February 10, 2015, 9:43 AM
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#include <algorithm>
#include <vector>
#include <string>
#include <iostream>

#include "ComponentBank.h"

namespace DualityEngine {

    class System
    {
    private:
        std::string name;
        bool escape = false;
        bool paused = false;
        bool pauseConfirmed = false;
        
    protected:
        ComponentBank* bank;
        std::vector<std::vector<DUA_id>> registeredIDs;   // These are collections so that a system can operate
        std::vector<DUA_compFlag> requiredComponents;     // on more than one kind of entity if it needs to.
        uint tockFreq = 1;  // every 'tockFreq' times 'tock' is called, it's code will execute. (periodic loop not linked to 'tick')
        
    public:        
        System(ComponentBank* bank, std::string name, int numRegisters);
        virtual ~System();
        virtual void clean();
        std::string getName();
        void discoverID (const DUA_id& ID);
        void scrutinizeID (const DUA_id& ID);
        void forceRemoveComp(const DUA_id& ID, const DUA_compFlag&);
        virtual bool init(std::stringstream& output){ return false; }
        virtual void tick(){}
        virtual void tock(){}
        void pause();
        void resume();
        void confirmPaused();
        void quit();

        bool isPaused();
        bool isPauseConfirmed();
        bool isQuit();
    };

}

#endif	/* SYSTEM_H */

