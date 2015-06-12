/* 
 * File:   Engine.cpp
 * Author: adayoldbagel
 *
 * Created on January 27, 2015, 4:07 PM
 */

#include "../Headers/Engine.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
SystemEngine::SystemEngine(SDL_Thread** thread, const char* name, Delegate<void(const char*)>* output, Delegate<void()>* quit)
{
    workThread = thread;
    threadData.threadName = name;
    threadData.output = output;
    threadData.quit = quit;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
SystemEngine::~SystemEngine()
{
    workThread = NULL;
    for (auto system : threadData.systemsToExecute)
        system = NULL;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Add System">
void SystemEngine::addSystem(System* system)
{
    threadData.systemsToExecute.push_back(system);
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Engage">
void SystemEngine::engage()
{
    *workThread = SDL_CreateThread (EngineThreadFunction, threadData.threadName.c_str(),
                                    (void*) &threadData);
    
//    threadData.systemsToExecute[0]->init();
//    threadData.systemsToExecute[0]->tick();
    
//    systemsToExecute.at(0)->init();
//    while (!systemsToExecute.at(0)->isQuit())
//    {
//        systemsToExecute.at(0)->tick();
//    }
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Work Thread Function">
#define THREAD_BEGIN_BLOCK_OUTPUT "@============@  " << threadName << " initialization log BEGINS  @============@\n\n"
#define THREAD_END_BLOCK_OUTPUT std::endl << "@============@  " << threadName << " initialization log ENDS  @============@\n\n"
int DualityEngine::EngineThreadFunction(void* data)
{
    System* system;
    ThreadData* threadData = (ThreadData*)data;
    std::vector<System*>* systems = &(threadData->systemsToExecute);
    Delegate<void(const char*)>* output = threadData->output;
    std::stringstream tempOut;
    std::string threadName = threadData->threadName;
    Delegate<void()>* quitGame = threadData->quit;
    
    bool escape = false;   
    
    // Initialization of each system running on this thread
    tempOut << THREAD_BEGIN_BLOCK_OUTPUT;
    for (int i = 0; i < systems->size(); i++)
    {
        system = systems->at(i);
        tempOut << "Beginning initialization of " << system->getName() << "...\n";
        try {
            if (!(system->init(tempOut)))
            {
                tempOut << system->getName() << " failed to initialize! Terminating "
                        << threadName << "!\n" << THREAD_END_BLOCK_OUTPUT;
                (*output)(tempOut.str().c_str());
                (*quitGame)();
                return -1;
            }
            else
                tempOut << system->getName() << " has initialized.\n\n";
        } catch (...) {
            tempOut << "EXCEPTION thrown: ";
            std::exception_ptr eptr = std::current_exception();
            try {
                std::rethrow_exception(eptr);
            } catch (const std::exception& e) {
                tempOut << "\"" << e.what() << "\". ";
            } catch (...) {
                tempOut << "Unknown Exception. ";
            }
            tempOut << "Terminating " << threadName << "!\n" << THREAD_END_BLOCK_OUTPUT;

            (*output)(tempOut.str().c_str());
            (*quitGame)();
            return -1;
        }
    }
    
    // Printout of what's running on this thread
    tempOut << threadName << " is entering main loop running the following systems:\n";
    for (int i = 0; i < systems->size(); i++)
        tempOut << "    " << i + 1 << ". " << systems->at(i)->getName() << std::endl;
    tempOut << THREAD_END_BLOCK_OUTPUT;
    
    // Output the thread's reports to console.
    (*output)(tempOut.str().c_str());
    
    // Clear the thread's output.
    tempOut.str(std::string());
    tempOut.clear();
    
    // Main engine loop
    while (!escape)
    {
        for (int i = 0; i < systems->size(); i++)
        {
            system = systems->at(i);
            if (system->isQuit()){
                tempOut << system->getName() << " has received a call to quit.\n";
                escape = true;                
            } else if (!system->isPaused()) {
                try{
                    system->tick();
                } catch (const char* err) {
                    tempOut << "ERROR in " << system->getName() << ": " << err << std::endl;
                    (*quitGame)();
                    escape = true;
                } catch (...) {
                    tempOut << "EXCEPTION thrown in " << system->getName() << ": ";
                    std::exception_ptr eptr = std::current_exception();
                    try {
                        std::rethrow_exception(eptr);
                    } catch (const std::exception& e) {
                        tempOut << "\"" << e.what() << "\"\n";
                    } catch (...) {
                        tempOut << "Unknown exception.\n";
                    }
                            
                    (*quitGame)();
                    escape = true;
                }
            } else if (system->isPaused()) {
                if (!system->isPauseConfirmed()){
                    system->confirmPaused();
//                    std::string foobar = system->getName() + " confirmed pause.\n";
//                    (*output)(foobar.c_str());                    
                }
            }
        }
    }
    tempOut << "Terminating " << threadName << "\n\n";
    
    // Output the thread's reports to standard out.
    (*output)(tempOut.str().c_str());
}
//</editor-fold>