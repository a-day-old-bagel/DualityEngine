/*******************************************************************************
 * File:   ControlDelegateBag.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 3:56 PM
 * 
 * 
 ******************************************************************************/

#ifndef CONTROLDELEGATEBAG_H
#define	CONTROLDELEGATEBAG_H

#include "Delegate.h"

namespace DualityEngine {

    struct DelegateBag
    {
        VoidDelegate menu;
        VoidDelegate quit;
    };

}

#endif	/* CONTROLDELEGATEBAG_H */

