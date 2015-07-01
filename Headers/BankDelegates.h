/*******************************************************************************
 * File:   BankDelegates.h
 * Author: Galen Cochrane *
 * Created on May 14, 2015, 2:10 AM
 * 
 * 
 ******************************************************************************/

#ifndef BANKDELEGATES_H
#define	BANKDELEGATES_H

#include "Delegate.h"
#include "Settings.h"

namespace DualityEngine {

    struct BankDelegates {
        Delegate<void(const DUA_id&)> systemsDiscover;
        Delegate<void(const DUA_id&)> systemsScrutinize;
        Delegate<void(const DUA_id&, const DUA_compFlag&)> systemsForceRemove;
        Delegate<void()> quit;
        Delegate<void()> newGame;
        Delegate<void()> pause;
        Delegate<void()> resume;
        Delegate<void(const std::string&)> runScript;
        Delegate<bool()> pauseDependentSystems;
        Delegate<bool()> waitForDependentSystemsPause;
        Delegate<bool()> resumeDependentSystems;
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
        Delegate<std::string()> getCurrentLogLine;
        Delegate<void(bool, bool)> setConsoleState;
        Delegate<void(int)> logTraverse;
        Delegate<void(std::string&, std::string&)> switchSky;
        
    };

}

#endif	/* BANKDELEGATES_H */

