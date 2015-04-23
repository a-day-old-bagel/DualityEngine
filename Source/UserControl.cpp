/*******************************************************************************
 * File:   UserControl.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include "../Headers/UserControl.h"

//<editor-fold>

System_UserControl::System_UserControl(ComponentBank* bank, DelegateBag* delegates) : System(bank, "Control System")
{
    this->delegates = delegates;
}

System_UserControl::~System_UserControl()
{
    //POINTER_DELETE(menu);
    delegates = NULL;
}

bool System_UserControl::init()
{
    return true;
}


void System_UserControl::tick()
{
    //Handle events on queue
    while(SDL_PollEvent(&sdlEvent) != 0)
    {
        //User requests quit
        if(sdlEvent.type == SDL_QUIT)
        {
            delegates->quit();
        }
    }

    //Set texture based on current keystate
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    if( currentKeyStates[ SDL_SCANCODE_ESCAPE ] )
    {
        delegates->menu();
    }
}

//</editor-fold>