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
        std::vector<IDnumber> registeredIDs;
        componentFlag requiredComponents;
        ComponentBank* bank;
        bool escape = false;
        bool paused = false;

    public:        
        System(ComponentBank* bank, std::string name);
        ~System();
        std::string getName();
        void discoverID (IDnumber ID);
        void scrutinizeID (IDnumber ID);
        virtual bool init(){}
        virtual void tick(){}
        void pause();
        void resume();
        void quit();

        bool isPaused();
        bool isQuit();
    };

}

#endif	/* SYSTEM_H */

