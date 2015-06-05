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
        Delegate<void()> newGame;
        Delegate<void()> pause;
        Delegate<void()> resume;
        Delegate<void()> backspaceCommand;
        Delegate<void()> deleteCommand;
        Delegate<void()> clearCommand;
        Delegate<void()> upOneCommand;
        Delegate<void()> downOneCommand;
        Delegate<void()> leftCursor;
        Delegate<void()> rightCursor;
        Delegate<void(const char*)> output;
        Delegate<void(const std::string&)> outputStr;
        Delegate<void(const char*)> appendToCommand;        
        Delegate<std::string()> consoleEnter;
        Delegate<void(const std::string&)> submitScriptCommand;
        Delegate<std::string(int)> getLogFromBack;
        Delegate<void(bool, bool)> setConsoleState;
        Delegate<void(int)> logTraverse;
    };

}

#endif	/* CONTROLDELEGATES_H */

