/*******************************************************************************
 * File:   Score.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 12:47 PM
 * 
 * Provides an int in which to keep score for an entity.
 ******************************************************************************/

#ifndef SCORE_H
#define	SCORE_H

#include "Settings.h"

namespace DualityEngine {
    
    struct Score
    {
        Score(){ score = 0; }
        Score(const DUA_scoreNum &score){ this->score = score; }
        DUA_scoreNum score;
    };
}

#endif	/* SCORE_H */

