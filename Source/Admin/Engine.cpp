/* 
 * File:   Engine.cpp
 * Author: adayoldbagel
 *
 * Created on January 27, 2015, 4:07 PM
 */

#include "Engine.h"

using namespace DualityEngine;


SystemEngine::SystemEngine(SDL_Thread** thread, const char* name, Delegate<void(const char*)>* output, Delegate<void()>* quit)
{
    workThread = thread;
    threadData.threadName = name;
    threadData.output = output;
    threadData.quit = quit;
}


SystemEngine::~SystemEngine()
{

}


void SystemEngine::addSystem(System* system)
{
    threadData.systemsToExecute.push_back(system);
}


void SystemEngine::engage()
{
    *workThread = SDL_CreateThread (EngineThreadFunction, threadData.threadName.c_str(), (void*) &threadData);
}


int DualityEngine::EngineThreadFunction(void* data)
{
    System* system;
    ThreadData* threadData = (ThreadData*)data;
    std::vector<System*>* systems = &(threadData->systemsToExecute);
    Delegate<void(const char*)>* output = threadData->output;
    std::stringstream tempOut;
    std::string threadName = threadData->threadName;
    Delegate<void()>* quitGame = threadData->quit;
    
    std::string initBlockText_Begin = "@============@  " + threadName + " initialization log BEGINS  @============@\n\n";
    std::string initBlockText_End   = "\n@============@  " + threadName + " initialization log ENDS  @============@\n\n";
    
    bool systemsInitializedCorrectly = true;
    bool escape = false; 
    
    // Initialization of each system running on this thread
    tempOut << initBlockText_Begin;
    for (uint i = 0; i < systems->size(); i++)
    {
        system = systems->at(i);
        tempOut << "Beginning initialization of " << system->getName() << "...\n";
        try {
            if (!(system->init(tempOut)))
            {
                tempOut << "<!>    " << system->getName() << " failed to initialize! Terminating "
                        << threadName << "!\n" << initBlockText_End;

                systemsInitializedCorrectly = false;
                break;
            }
            else
                tempOut << system->getName() << " has initialized.\n\n";
        } catch (...) {
            tempOut << "<!>    EXCEPTION thrown: ";
            std::exception_ptr eptr = std::current_exception();
            try {
                std::rethrow_exception(eptr);
            } catch (const std::exception& e) {
                tempOut << "\"" << e.what() << "\". ";
            } catch (...) {
                tempOut << "Unknown Exception. ";
            }
            tempOut << "Terminating " << threadName << "!\n" << initBlockText_End;

            systemsInitializedCorrectly = false;
            break;
        }
    }
    
    if (systemsInitializedCorrectly){
        // Printout of what's running on this thread
        tempOut << threadName << " is entering main loop running the following systems:\n";
        for (uint i = 0; i < systems->size(); i++)
            tempOut << "    " << i + 1 << ". " << systems->at(i)->getName() << std::endl;
        tempOut << initBlockText_End;

        // Output the thread's reports to console.
        (*output)(tempOut.str().c_str());

        // Clear the thread's output.
        tempOut.str(std::string());
        tempOut.clear();

        // Main engine loop
        while (!escape)
        {
            escape = true;
            for (uint i = 0; i < systems->size(); i++)
            {
                system = systems->at(i);
                if (system->isQuit()){
                    tempOut << system->getName() << " aknowledges the call to exit.\n";
                } else if (system->isPaused()){
                    escape = false;
                    if (!system->isPauseConfirmed()){
                        system->confirmPaused();                 
                    }
                } else {
                    escape = false;
                    try{
                        system->tick();             // THIS IS THE IMPORTANT LINE
                    } catch (const char* err) {
                        tempOut << "<!>    ERROR in " << system->getName() << ".tick : " << err << std::endl;
                        (*quitGame)();
                    } catch (...) {
                        tempOut << "<!>    EXCEPTION thrown in " << system->getName() << ".tick : ";
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
            }
        }
        tempOut << "Terminating " << threadName << "\n\n";

        // Output the thread's reports to standard out.
        (*output)(tempOut.str().c_str());
    } else {
        tempOut << "\n<!>    Execution cannot continue after failed initialization of one or more systems!\n";
        (*output)(tempOut.str().c_str());
        (*quitGame)();
    }
    return 0;
}
