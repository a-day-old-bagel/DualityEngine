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
        Delegate<void(const DUA_ID&)> systemsDiscover;
        Delegate<void(const DUA_ID&)> systemsScrutinize;
        Delegate<void(const char*)>  output;
    };

}

#endif	/* BANKDELEGATES_H */

