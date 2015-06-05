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
        Delegate<void(const char*)>  output;
        Delegate<void(const std::string&)> outputStr;
    };

}

#endif	/* BANKDELEGATES_H */

