/* 
 * File:   Engine.cpp
 * Author: adayoldbagel
 *
 * Created on January 27, 2015, 4:07 PM
 */

#include "../Headers/Engine.h"

//<editor-fold defaultstate="collapsed" desc="Constructor">
SystemEngine::SystemEngine(SDL_Thread** thread, std::string name)
{
    workThread = thread;
    threadData.threadName = name;
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
    *workThread = SDL_CreateThread (workThreadFunction, threadData.threadName.c_str(),
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
int workThreadFunction(void* data)
{
    System* system;
    ThreadData* threadData = (ThreadData*)data;
    std::vector<System*>* systems = &(threadData->systemsToExecute);
    std::stringstream* output = &threadData->output;
    
    bool escape = false;   
    
    // Initialization of each system running on this thread
    *output << "=========================================\n";
    for (int i = 0; i < systems->size(); i++)
    {
        *output << "Beginning initialization of " << systems->at(i)->getName() << "...\n\n";
        if (!(systems->at(i)->init()))
        {
            *output << systems->at(i)->getName() << " failed to initialize! Terminating " << threadData->threadName << "!\n";
            return -1;
        }
        else
            *output << systems->at(i)->getName() << " has initialized.\n\n";
    }
    
    // Printout of what's running on this thread
    *output << threadData->threadName << " is entering an engine loop running the following systems:\n";
    for (int i = 0; i < systems->size(); i++)
        *output << "    " << i + 1 << ". " << systems->at(i)->getName() << std::endl;
    *output << "=========================================\n\n";
    
    // Output the thread's reports to console.
    std::cout << output->str();
    
    // Clear the thread's output.
    output->str(std::string());
    output->clear();
    
    // Main engine loop
    while (!escape)
    {
        for (int i = 0; i < systems->size(); i++)
        {
            system = systems->at(i);
            if (system->isQuit())
            {
                *output << systems->at(i)->getName() << " has received a call to quit.\n";
                escape = true;                
            }
            else if (!system->isPaused())
                system->tick();
        }
    }
    *output << "Terminating " << threadData->threadName << " engine loop.\n\n";
    
    // Output the thread's reports to standard out.
    std::cout << output->str();
    
    // Clear the thread's output.
    output->str(std::string());
    output->clear();
}
//</editor-fold>