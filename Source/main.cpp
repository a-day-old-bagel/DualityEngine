/****************************************
 * File:   main.cpp
 * Author: Galen Cochrane (adayoldbagel)
 *
 * Created on January 27, 2015, 2:26 PM
 ***************************************/

#include "../Headers/Game.h"
#include "../Headers/Matrix.h"
#include "../Headers/Functions.h"

/*************************************************
 * This looks simple enough, right?
 ************************************************/
int main()
{
    DualityEngine::Mat4x4d testMat0;
    testMat0.print();
    testMat0(0,0) = 1;
    testMat0(1,1) = 1;
    testMat0(2,2) = 1;
    testMat0(3,3) = 1;
    testMat0.print();
    testMat0(0,3) = 7;
    testMat0(1,3) = 8;
    testMat0(2,3) = 9;
    testMat0.print();
    
    DualityEngine::Vec4d testVec0{{3}, {3}, {3}, {1}};
    testVec0.print();
    testVec0 = testMat0 * testVec0;
    testVec0.print();
    
    
    
    DualityEngine::Game().Main();
    return 0;
}

