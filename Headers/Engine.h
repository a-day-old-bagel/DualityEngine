/* 
 * File:   Engine.h
 * Author: adayoldbagel
 *
 * Created on February 7, 2015, 1:13 PM
 */

#ifndef SYSTEMENGINE_H
#define	SYSTEMENGINE_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <SDL.h>
#include <SDL_thread.h>
#include "System.h"

namespace DualityEngine {
    
    //<editor-fold defaultstate="collapsed" desc="Thread Data Struct & Function Stub">

    // One of these is passed to the engines thread when it executes.
    struct ThreadData
    {
        std::vector<System*> systemsToExecute;  // The systems to manage
        std::string threadName;                 // Name of the thread given to OS
        std::stringstream output;               // Output stream
    };

    // stub for the function that is given to the engine's thread
    int EngineThreadFunction(void* data);

    //</editor-fold>
    //<editor-fold defaultstate="collapsed" desc="Engine Class Definition">

    class SystemEngine
    {
    private:
        //<editor-fold defaultstate="collapsed" desc="Members">
        ThreadData threadData;
        // Thread on which to run the attached Systems
        SDL_Thread** workThread;
        //</editor-fold>

    public:
        //<editor-fold defaultstate="collapsed" desc="Methods">
        // Constructor takes a thread on which to run the engine
        SystemEngine(SDL_Thread** thread, std::string name);
        // Destructor
        ~SystemEngine();
        // Use this to start the engine running.
        void engage();
        // Use this to add another system to this engine.
        void addSystem(System* system);
        //</editor-fold>
    };

    //</editor-fold>

}

#endif	/* SYSTEMENGINE_H */
