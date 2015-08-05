/****************************************
 * File:   main.cpp
 * Author: Galen Cochrane (adayoldbagel)
 *
 * Created on January 27, 2015, 2:26 PM
 ***************************************/

#include "../Headers/Game.h"
//#include <fenv.h>

/*************************************************
 * This looks simple enough, right?
 ************************************************/
int main()
{    
//    feenableexcept(FE_INVALID | FE_OVERFLOW);
    
    DualityEngine::Game().Main();
    return 0;
}

