/*******************************************************************************
 * File:   Score.h
 * Author: Galen Cochrane *
 * Created on April 30, 2015, 12:47 PM
 * 
 * Provides an int in which to keep score for an entity.
 ******************************************************************************/

#ifndef SCORE_H
#define	SCORE_H

#include "Headers/Settings.h"

namespace DualityEngine {
    
    struct Score
    {
        Score(){ score = 0; }
        Score(const scoreNumber &score){ this->score = score; }
        scoreNumber score;
    };
}

#endif	/* SCORE_H */

