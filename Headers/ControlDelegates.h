/*******************************************************************************
 * File:   ControlDelegates.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 3:56 PM
 * 
 * 
 ******************************************************************************/

#ifndef CONTROLDELEGATES_H
#define	CONTROLDELEGATES_H

#include "Delegate.h"

namespace DualityEngine {

    struct ControlDelegates {
        Delegate<void()> quit;
        Delegate<void()> backspaceCommand;
        Delegate<void()> clearCommand;
        Delegate<void(const char*)> output;
        Delegate<void(const char*)> appendToCommand;        
        Delegate<std::string()> submitCommand;
        Delegate<std::string()> getLastConsoleLine;
        Delegate<void(bool, bool)> setConsoleState;
    };

}

#endif	/* CONTROLDELEGATES_H */

