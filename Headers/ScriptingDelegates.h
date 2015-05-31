/*******************************************************************************
 * File:   ScriptingDelegates.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 3:56 PM
 * 
 * 
 ******************************************************************************/

#ifndef SCRIPTINGDELEGATES_H
#define	SCRIPTINGDELEGATES_H

#include "Delegate.h"

namespace DualityEngine {

    struct ScriptingDelegates {
        Delegate<void(const char*)> output;
        Delegate<void(const std::string&)> outputStr;
    };

}

#endif	/* SCRIPTINGDELEGATES_H */

