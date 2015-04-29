/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/

#include "../Headers/System.h"

using namespace DualityEngine;

System::System(ComponentBank* bank, std::string name)
{
    this->bank = bank;
    this->name = name;
}

System::~System()
{
    bank = NULL;
}

std::string System::getName()
{
    return name;
}

void System::discoverID(IDnumber ID)
{
    // If the entity (the ID) isn't already in the list of IDs
    if (std::find(registeredIDs.begin(), registeredIDs.end(), ID) == registeredIDs.end())
    {
        // If the entity (the ID) has all the required components
        if (((bank->getSoulPtr(ID)->components) & requiredComponents) == requiredComponents)
            registeredIDs.push_back(ID);    // add the entity (ID) to the list
    }
}

void System::scrutinizeID(IDnumber ID)
{
    // get location (if any) of ID in registeredIDs
    std::vector<uint_fast32_t>::iterator it =
        std::find(registeredIDs.begin(), registeredIDs.end(), ID);
    // if the ID is in registeredIDs...
    if (it != registeredIDs.end())
    {
        // if the entity no longer has the required components
        if (((bank->getSoulPtr(ID)->components) & requiredComponents) != requiredComponents)
            registeredIDs.erase(it);    // remove that entity (ID) from the list
    }
}

void System::pause()
{
    if (!paused)
        paused = true;
}

void System::resume()
{
    if (paused)
        paused = false;
}

void System::quit()
{
    if (!escape)
        escape = true;
}

bool System::isPaused()
{
    return paused;
}

bool System::isQuit()
{
    return escape;
}