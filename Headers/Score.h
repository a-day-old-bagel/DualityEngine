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
        Score(const DU_SCORENUM &score){ this->score = score; }
        DU_SCORENUM score;
    };
}

#endif	/* SCORE_H */

