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
#include "Delegate.h"

namespace DualityEngine {


    // HEADERS



    // One of these is passed to the engines thread when it executes.
    template <typename... System_Types>
    struct ThreadData {
        std::tuple<System_Types...>* systems;    // Collection of systems to manage
        std::string* threadName;                 // Name of the thread given to OS
        Delegate<void(const char*)>* output;    // Output stream
        Delegate<void()>* quit;                 // Used in case of failure of system(s))
    };

    // stub for the function that is given to the engine's thread
    template <typename... System_Types>
    int EngineThreadFunction(void* data);

    template <typename... System_Types>
    class SystemEngine
    {
    private:
        std::tuple<System_Types...> systems;    // Collection of systems to manage
        std::string threadName;
        ThreadData<System_Types...> threadData;
        SDL_Thread** workThread;

    public:
        // Constructor takes a thread on which to run the engine
        SystemEngine(SDL_Thread** thread, const char* name, Delegate<void(const char*)>* output,
                     Delegate<void()>* quit, System_Types... systems);
        // Destructor
        ~SystemEngine();
        // Use this to start the engine running.
        void engage();
    };




    // IMPLEMENTATION



    template <typename... System_Types>
    SystemEngine<System_Types...>::SystemEngine(SDL_Thread** thread, const char* name, Delegate<void(const char*)>* output,
                                                Delegate<void()>* quit, System_Types... systems)
    {
        workThread = thread;
        this->systems = std::make_tuple(systems...);
        threadName = name;
        threadData.threadName = &threadName;
        threadData.output = output;
        threadData.quit = quit;
        threadData.systems = &(this->systems);
    }

    template <typename... System_Types>
    SystemEngine<System_Types...>::~SystemEngine()
    {

    }

    template <typename... System_Types>
    void SystemEngine<System_Types...>::engage() {
        *workThread = SDL_CreateThread (EngineThreadFunction<System_Types...>, threadName.c_str(), (void*) &threadData);
    }

    /*
     * UNFOLDING VARIADIC TEMPLATES FOR HANDLING TUPLES OF SYSTEMS (NEXT FOUR FUNCTIONS)...
     */

    /*
     * Templates for calling [system].init() on a tuple of systems of varying types
     */
    template<std::size_t I = 0, typename... Sys_Types>
    inline typename std::enable_if<I == sizeof...(Sys_Types), void>::type
    do_inits(std::stringstream& tempOut, bool& systemsInitializedCorrectly, std::tuple<Sys_Types...>& systems)
    {  } // Once this function is called the templates are finished unrolling

    template<std::size_t I = 0, typename... Sys_Types>
    inline typename std::enable_if<I < sizeof...(Sys_Types), void>::type
    do_inits(std::stringstream& tempOut, bool& systemsInitializedCorrectly, std::tuple<Sys_Types...>& systems) {

        tempOut << "Beginning initialization of " << std::get<I>(systems)->getName() << "...\n";
        try {
            if (!(std::get<I>(systems)->init(tempOut))) {
                systemsInitializedCorrectly = false;
                return;
            }
            else
                tempOut << std::get<I>(systems)->getName() << " has initialized.\n\n";
        } catch (...) {
            tempOut << "<!>    EXCEPTION thrown: ";
            std::exception_ptr eptr = std::current_exception();
            try {
                std::rethrow_exception(eptr);
            } catch (const std::exception &e) {
                tempOut << "\"" << e.what() << "\". ";
            } catch (...) {
                tempOut << "Unknown Exception. ";
            }

            systemsInitializedCorrectly = false;
            return;
        }

        do_inits<I + 1, Sys_Types...>(tempOut, systemsInitializedCorrectly, systems);
    }

    /*
     * Templates for calling [system].tick() on a tuple of systems of varying types
     */
    template<std::size_t I = 0, typename... Sys_Types>
    inline typename std::enable_if<I == sizeof...(Sys_Types), void>::type
    do_ticks(std::stringstream& tempOut, Delegate<void()>* quitGame, bool& escape, std::tuple<Sys_Types...>& systems)
    {  } // Once this function is called the templates are finished unrolling

    template<std::size_t I = 0, typename... Sys_Types>
    inline typename std::enable_if<I < sizeof...(Sys_Types), void>::type
    do_ticks(std::stringstream& tempOut, Delegate<void()>* quitGame, bool& escape, std::tuple<Sys_Types...>& systems) {

        if (std::get<I>(systems)->isQuit()){
            tempOut << std::get<I>(systems)->getName() << " aknowledges the call to exit.\n";
        } else if (std::get<I>(systems)->isPaused()){
            escape = false;
            if (!std::get<I>(systems)->isPauseConfirmed()){
                std::get<I>(systems)->confirmPaused();
            }
        } else {
            escape = false;
            try{
                std::get<I>(systems)->tick();             // THIS IS THE IMPORTANT LINE
            } catch (const char* err) {
                tempOut << "<!>    ERROR in " << std::get<I>(systems)->getName() << ".tick : " << err << std::endl;
                (*quitGame)();
            } catch (...) {
                tempOut << "<!>    EXCEPTION thrown in " << std::get<I>(systems)->getName() << ".tick : ";
                std::exception_ptr eptr = std::current_exception();
                try {
                    std::rethrow_exception(eptr);
                } catch (const std::exception& e) {
                    tempOut << "\"" << e.what() << "\"\n";
                } catch (...) {
                    tempOut << "Unknown exception.\n";
                }
                (*quitGame)();
            }
        }

        do_ticks<I + 1, Sys_Types...>(tempOut, quitGame, escape, systems);
    }

    /*
     * Function that runs in an engine's thread
     */
    template <typename... System_Types>
    int EngineThreadFunction(void* data)
    {
        ThreadData<System_Types...>* threadData = (ThreadData<System_Types...>*)data;
        std::tuple<System_Types...> systems = *(threadData->systems);
        Delegate<void(const char*)>* output = threadData->output;
        Delegate<void()>* quitGame = threadData->quit;
        std::stringstream tempOut;  // This serves as a log for stuff happening in this thread
        std::string threadName = *(threadData->threadName);

        std::string initBlockText_Begin = "@============@  " + threadName + " initialization log BEGINS  @============@\n\n";
        std::string initBlockText_End   = "\n@============@  " + threadName + " initialization log ENDS  @============@\n\n";

        // Log the beginning of systems' initialization steps
        tempOut << initBlockText_Begin;

        // Call init() on each system managed by this engine (this template unrolls into an init block for each system)
        bool systemsInitializedCorrectly = true;
        do_inits(tempOut, systemsInitializedCorrectly, systems);

        if (systemsInitializedCorrectly){

            // Add initialization success message to end of log
            tempOut << threadName << " is entering main loop...\n" << initBlockText_End;
            // Output the thread's log to Game console.
            (*output)(tempOut.str().c_str());
            // Clear the thread's log.
            tempOut.str(std::string());
            tempOut.clear();

            // Main engine loop
            bool escape = false;
            while (!escape) {
                escape = true;
                // Call tick() on each system managed by this engine (this template unrolls into an init block for each system)
                do_ticks(tempOut, quitGame, escape, systems);
            }
            tempOut << "Terminating " << threadName << "\n\n";

            // Output the thread's reports to Game console
            (*output)(tempOut.str().c_str());

        } else {

            // If initialization failed in the systems add fail message to end of log
            tempOut << initBlockText_End << "\n<!>    " << threadName
            << ": Execution cannot continue after failed initialization of one or more systems!\n";
            // output log to Game console
            (*output)(tempOut.str().c_str());
            (*quitGame)();

        }

        return 0;
    }
}

#endif	/* SYSTEMENGINE_H */
