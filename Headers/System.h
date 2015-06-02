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
//      std::string messageQueue;   // might want in future for message output from systems...
        
    public:        
        System(ComponentBank* bank, std::string name, int numRegisters);
        ~System();
        void clean();
        std::string getName();
        virtual void discoverID (const DUA_id& ID);
        virtual void scrutinizeID (const DUA_id& ID);
        virtual bool init(std::stringstream& output){}
        virtual void tick(){}
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

