/* 
 * File:   Engine.h
 * Author: adayoldbagel
 *
 * Created on February 7, 2015, 1:13 PM
 *
 * The SystemEngine (or Engine) is an object that manages operations done on a single thread.
 * It has a pointer to a thread and a collection (tuple) of systems to be run on that thread.
 * It takes care of looping the calls to each of its systems' tick() functions until told to pause or quit.
 * So it's basically just a big wrapper around a thread.
 *
 * The thread itself is instantiated elsewhere and passed in as a pointer.
 *
 * Due to the use of templated functions, code that would normally be split into .h and .cpp files all appears here.
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

    //  ***********************  HEADERS  ***********************

    //region ThreadData Struct
    /*
     * A ThreadData object contains all the information that will be needed for the thread to run, like a pointer to
     * the collection of system pointers, a pointer to the name of the thread, a pointer to the logging output stream
     * delegate, and a pointer to the quit delegate.
     * A structure like this was necessary because all that can be passed to the thread when it initialized is a
     * void pointer, so the address of this struct will be cast as a void pointer and then cast back inside the thread.
     */
    template <typename... System_Types>
    struct ThreadData {
        std::tuple<System_Types...>* systems;    // Collection of systems to manage
        std::string* threadName;                 // Name of the thread given to OS
        Delegate<void(const char*)>* output;    // Output stream
        Delegate<void()>* quit;                 // Used in case of failure of system(s))
    };
    //endregion

    //region Engine Thread Function Prototype
    /*
     * This function stub is for the function that will be given to the thread when it starts.  As you can see, it takes
     * only a void pointer.
     */
    template <typename... System_Types>
    int EngineThreadFunction(void* data);
    //endregion

    //region SystemEngine Class
    /*
     * Here is the actual SystemEngine class declaration.
     */
    template <typename... System_Types>
    class SystemEngine {
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
    //endregion

    //  ***********************  IMPLEMENTATION  ***********************

    //region Constructor
    /*
     * Constructor takes:
     * a pointer to the thread itself, a string for the thread name, a pointer to the output log delegate, a pointer
     * to the quit delegate, and the variadic list of system pointers (make sure you pass pointers)
     */
    template <typename... System_Types>
    SystemEngine<System_Types...>::SystemEngine(SDL_Thread** thread,
                                                const char* name,
                                                Delegate<void(const char*)>* output,
                                                Delegate<void()>* quit,
                                                System_Types... systems) {
        workThread = thread;
        this->systems = std::make_tuple(systems...);
        threadName = name;
        threadData.threadName = &threadName;
        threadData.output = output;
        threadData.quit = quit;
        threadData.systems = &(this->systems);
    }
    //endregion

    //region Destructor
    /*
     * Destructor
     */
    template <typename... System_Types>
    SystemEngine<System_Types...>::~SystemEngine()
    {

    }
    //endregion

    //region Engage
    /*
     * Starts the engine running (begins the thread)
     * As you can see, threadData is cast to a void pointer to fulfill the parameter type of SDL_CreateThread
     */
    template <typename... System_Types>
    void SystemEngine<System_Types...>::engage() {
        *workThread = SDL_CreateThread (EngineThreadFunction<System_Types...>, threadName.c_str(), (void*) &threadData);
    }
    //endregion

    //region System Init Template
    /*
     * These next two confusing templated functions are for calling [system].init() for each system in
     * a tuple of systems of varying types.  OOP inheritance and virtual functions have been avoided.
     * There are two because they unroll recursively into code that calls init() on each system. The first function
     * is only used to terminate the recursion.
     * Just trust me. I couldn't find any way to make this pretty.
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
            if (!(std::get<I>(systems)->init(tempOut))) {   // Here is the actual init() call
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
    //endregion

    //region System Tick Template
    /*
     * These next two functions are just like the last two, except they're for calling tick() on each system instead
     * of init().
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
    //endregion

    //region Engine Thread Function
    /*
     * And here, finally, is the function that runs in the thread once it's initialized. It casts the void pointer data
     * back into the correct types, calls init() once on each system in the collection, the begins looping calls to
     * tick() until it is told to pause or quit.
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

        // Call init() on each system managed by this engine (this template unrolls into a code block for each system)
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
                // Call tick() on each system managed by this engine
                // (this line unrolls into a code block for each system)
                do_ticks(tempOut, quitGame, escape, systems);
            }
            tempOut << "Terminating " << threadName << "\n\n";
            // Output the thread's reports to Game console
            (*output)(tempOut.str().c_str());

        } else { // Systems' initialization has failed.
            tempOut << initBlockText_End << "\n<!>    " << threadName
            << ": Execution cannot continue after failed initialization of one or more systems!\n";
            // output log to Game console
            (*output)(tempOut.str().c_str());
            (*quitGame)();
        }
        return 0;
    }
    //endregion
}

#endif	/* SYSTEMENGINE_H */
