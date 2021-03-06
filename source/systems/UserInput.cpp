/*******************************************************************************
 * File:   UserInput.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include <SDL_video.h>
#include "UserInput.h"

namespace DualityEngine {

    const std::string System_UserInput::menuText = "****************************************\n*  MENU - ENTER ONE OF THESE OPTIONS:  *\n*     new, load, save, exit, help      *\n****************************************\n";

    System_UserInput::System_UserInput(Bank* bank)
            : System<System_UserInput>(bank, "User Input System", 0) {
        consoleIsActive = false;
        MenuIsActive = false;
    }

    System_UserInput::~System_UserInput() {
        //Destroy window
        SDL_DestroyWindow(bank->pWindow);
        bank->pWindow = NULL;

        //Quit SDL subsystems
        SDL_Quit();
    }

    bool System_UserInput::onInit(std::stringstream& output) {

        output << "SDL recognizes the following render drivers: ";
        int numDrivers = SDL_GetNumRenderDrivers();
        for (int i = 0; i < numDrivers; ++i) {
            SDL_RendererInfo info;
            SDL_GetRenderDriverInfo(i, &info);
            output << info.name << ((i < numDrivers - 1) ? ", " : "\n");
        }
        output << "\nSDL recognizes the following video drivers:\n";
        numDrivers = SDL_GetNumVideoDrivers();
        const char* driverName;
        for (int i = 0; i < numDrivers; ++i) {
            driverName = SDL_GetVideoDriver(i);
            if (SDL_VideoInit(driverName) == 0) {
                SDL_VideoQuit();
                output << DUA_TAB << DUA_TAB << "Driver " << driverName << " works.\n";
            }
            else {
                output << DUA_TAB << DUA_ERR << "Driver " << driverName << " fails.\n" << DUA_ERREND;
            }
        }

        // Initialize SDL overall.
        // This needs to be done in this thread instead of in the graphics thread to prevent
        // massive event handing slowdowns. Will see if works in Windows.
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            output << DUA_ERR << "SDL did not initialize! SDL Error: " << SDL_GetError() << std::endl << DUA_ERREND;
            return false;
        }

        #ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
        if (SDL_SetRelativeMouseMode(SDL_TRUE) < 0) {
            output << DUA_ERR << "SDL could not enable relative mouse mode: " << SDL_GetError() << std::endl <<
            DUA_ERREND;
            return false;
        }
        #endif

        SDL_DisplayMode dm;
        int numDisps = SDL_GetNumVideoDisplays();
        if (SDL_GetDesktopDisplayMode(Settings::Display::whichMonitor, &dm) ||
            Settings::Display::whichMonitor > numDisps) {
            int failedDisplay = Settings::Display::whichMonitor;
            Settings::Display::whichMonitor = 0;
            if (SDL_GetDesktopDisplayMode(Settings::Display::whichMonitor, &dm)) {
                output << DUA_ERR << "SDL cannot find any displays!\n" << DUA_ERREND;
                return false;
            } else {
                output << DUA_ERR << "Requested display " << failedDisplay;
                output << " could not be found!\n" << DUA_ERREND << "Using display 0 instead.\n";
            }
        } else {
            output << "Using display " << Settings::Display::whichMonitor << std::endl;
        }
        output << "Display reports " << dm.w << "x" << dm.h << "@" << dm.refresh_rate << std::endl;
        Settings::Display::monitorOffsetX = SDL_WINDOWPOS_CENTERED_DISPLAY(Settings::Display::whichMonitor);
        Settings::Display::monitorOffsetY = SDL_WINDOWPOS_CENTERED_DISPLAY(Settings::Display::whichMonitor);

        #ifdef DUA_FULLSCREEN
        Settings::Display::windowResX = dm.w;
        Settings::Display::windowResY = dm.h;
        #endif

        Settings::Display::screenAspectRatio =
                (float) Settings::Display::windowResX / (float) Settings::Display::windowResY;

        Settings::Console::width = Settings::Display::windowResX;
        Settings::Console::height = Settings::Display::windowResY / 2;

        // Use only core features of gl version (deprecated features illegal)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Specify OpenGL version and profile
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DUA_GLVERSION_MAJOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DUA_GLVERSION_MINOR);

        // Turn on double buffering with a 24bit Z buffer.
        // You may need to change this to 16 or 32 for your system
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        // Create the SDL window
        bank->pWindow = SDL_CreateWindow("Duality Engine",
                                         Settings::Display::monitorOffsetX,
                                         Settings::Display::monitorOffsetY,
                                         Settings::Display::windowResX, Settings::Display::windowResY,
                //600, 600,
                                         DUA_SDL_SCREENOPTIONS);
        // If the window couldn't be created for whatever reason
        if (bank->pWindow == NULL) {
            output << DUA_ERR << "SDL window was not created! SDL Error: " << SDL_GetError() << std::endl << DUA_ERREND;
            return false;
        }
        else {
            output << "SDL window created.\n";
        }


        return true;
    }

    void System_UserInput::onTick() {

        // Get current keyboard state
        const Uint8* keyStates = SDL_GetKeyboardState(NULL);

        // Handle events on SDL event queue
        while (SDL_PollEvent(&sdlEvent) != 0) {
            switch (sdlEvent.type) {
                case (SDL_MOUSEMOTION):
                    if (!consoleIsActive) {
                        handleMouseMotion(sdlEvent.motion.xrel, sdlEvent.motion.yrel);
                    }
                    break;
                case (SDL_KEYDOWN):
                    handleKeyDown(sdlEvent);
                    break;
                case (SDL_TEXTINPUT):
                    if (consoleIsActive) {
                        // Add whatever text the user input to the console unless it was CTRL-C or CTRL-V
                        if (!((keyStates[SDL_SCANCODE_C] || keyStates[SDL_SCANCODE_V])
                              && SDL_GetModState() & KMOD_CTRL)) {
                            bank->dlgt->appendToCommand(sdlEvent.text.text);
                        }
                    }
                    break;
                case (SDL_MOUSEWHEEL):
                    if (consoleIsActive) {
                        bank->dlgt->logTraverse(sdlEvent.wheel.y);
                    }
                    break;
                case (SDL_QUIT):
                    bank->dlgt->quit();
                    bank->dlgt->outputStr(DUA_ERR + "FORCED EXIT" + DUA_ERREND);
                    break;
                default:
                    break;
            }
        }

        if (!consoleIsActive) {
            if (bank->activeControlID != DUA_NULL_ID) {
                handleControlKeys(keyStates);
            }
        }
    }

    void System_UserInput::onClean() {

    }

    void System_UserInput::parseMenuCommand(const std::string& command) {

        if (command.empty()) return;
        std::stringstream commandLine(command);
        std::string arg0;
        commandLine >> arg0;

        if (arg0 == "exit" || arg0 == "new" || arg0 == "save" || arg0 == "load") {
            bank->dlgt->submitScriptCommand(command);
        } else if (arg0 == "help") {
            bank->dlgt->submitScriptCommand(command);
        } else {
            bank->dlgt->outputStr(DUA_ERR + "Not a menu option: " + arg0 +
                                  ". Only commands 'new', 'load', 'save', 'exit', and 'help' may be accessed from the menu. " +
                                  "Press ESC to leave the menu. ~ key accesses console during play." + DUA_ERREND);
        }
    }

    void System_UserInput::handleKeyDown(SDL_Event& event) {
        if (consoleIsActive) {
            switch (event.key.keysym.sym) {
                case (SDLK_BACKQUOTE):
                    keyPressed_backQuote();
                    return;
                case (SDLK_ESCAPE):
                    keyPressed_esc();
                    return;
                case (SDLK_BACKSPACE):
                    bank->dlgt->backspaceCommand();
                    return;
                case (SDLK_DELETE):
                    bank->dlgt->deleteCommand();
                    return;
                case (SDLK_RETURN): {
                    std::string command = bank->dlgt->consoleEnter();
                    if (MenuIsActive) {
                        parseMenuCommand(command);
                    } else {
                        bank->dlgt->submitScriptCommand(command);
                    }
                    return;
                }
                case (SDLK_UP):
                    if (SDL_GetModState() & KMOD_SHIFT) {
                        bank->dlgt->logTraverse(1);
                    } else {
                        bank->dlgt->upOneCommand();
                    }
                    return;
                case (SDLK_DOWN):
                    if (SDL_GetModState() & KMOD_SHIFT) {
                        bank->dlgt->logTraverse(-1);
                    } else {
                        bank->dlgt->downOneCommand();
                    }
                    return;
                case (SDLK_LEFT):
                    bank->dlgt->leftCursor();
                    return;
                case (SDLK_RIGHT):
                    bank->dlgt->rightCursor();
                    return;
                case (SDLK_PAGEUP):
                    bank->dlgt->logTraverse(1);
                    return;
                case (SDLK_PAGEDOWN):
                    bank->dlgt->logTraverse(-1);
                    return;
                case (SDLK_c):
                    if (SDL_GetModState() & KMOD_CTRL) {
                        SDL_SetClipboardText(bank->dlgt->getCurrentLogLine().c_str());
                    }
                    return;
                case (SDLK_v):
                    if (SDL_GetModState() & KMOD_CTRL) {
                        bank->dlgt->appendToCommand(SDL_GetClipboardText());
                    }
                    return;
                case (SDLK_q) :
                    if (SDL_GetModState() & KMOD_CTRL) {
                        bank->dlgt->quit();
                    }
                    return;
                default:
                    return;
            }
        } else {
            switch (bank->currentControlType) {
                case ControlTypes::SPACE:
                    switch (event.key.keysym.sym) {
                        case (SDLK_l):
                            bank->pSpaceControlCurrent->autoBrakeLinear = !bank->pSpaceControlCurrent->autoBrakeLinear;
                            return;
                        case (SDLK_k):
                            bank->pSpaceControlCurrent->autoBrakeAngular = !bank->pSpaceControlCurrent->autoBrakeAngular;
                            return;
                        default:
                            break;
                    }
                    break;
                case ControlTypes::NONE:
                    break;
                default:
                    break;
            }
            switch (event.key.keysym.sym) {
                case (SDLK_BACKQUOTE):
                    keyPressed_backQuote();
                    return;
                case (SDLK_ESCAPE):
                    keyPressed_esc();
                    return;
                case SDLK_F12:
                    bank->dlgt->screenShot();
                default:
                    break;
            }
        }
    }

    void System_UserInput::handleControlKeys(const Uint8* keyStates) {

        switch (bank->currentControlType) {
            case ControlTypes::SPACE:
                if (keyStates[SDL_SCANCODE_W]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::FORWARD, 1);
                }
                if (keyStates[SDL_SCANCODE_S]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::BACKWARD, 1);
                }
                if (keyStates[SDL_SCANCODE_A]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::LEFT, 1);
                }
                if (keyStates[SDL_SCANCODE_D]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::RIGHT, 1);
                }
                if (keyStates[SDL_SCANCODE_LSHIFT]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::UP, 1);
                }
                if (keyStates[SDL_SCANCODE_LCTRL]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::DOWN, 1);
                }
                if (keyStates[SDL_SCANCODE_UP]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHPOS, 1);
                }
                if (keyStates[SDL_SCANCODE_DOWN]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHNEG, 1);
                }
                if (keyStates[SDL_SCANCODE_LEFT]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::YAWPOS, 1);
                }
                if (keyStates[SDL_SCANCODE_RIGHT]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::YAWNEG, 1);
                }
                if (keyStates[SDL_SCANCODE_Q]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::ROLLPOS, 1);
                }
                if (keyStates[SDL_SCANCODE_E]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::ROLLNEG, 1);
                }
                if (keyStates[SDL_SCANCODE_SPACE]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::LINBRAKE, 1);
                }
                if (keyStates[SDL_SCANCODE_C]) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::ANGBRAKE, 1);
                }
                if (keyStates[SDL_SCANCODE_LEFTBRACKET]) {
                    bank->pSpaceControlCurrent->rotSenseMod = -0.01f;
                }
                if (keyStates[SDL_SCANCODE_RIGHTBRACKET]) {
                    bank->pSpaceControlCurrent->rotSenseMod = 0.01f;
                }
                break;
            case ControlTypes::NONE:
                break;
            default:
                break;
        }
    }

    void System_UserInput::handleMouseMotion(int x, int y) {
        switch (bank->currentControlType) {
            case ControlTypes::SPACE:
                if (x >= 0) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::YAWNEG, x * 0.06f);
                } else {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::YAWPOS, x * -0.06f);
                }
                if (y >= 0) {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHNEG, y * 0.06f);
                } else {
                    bank->pSpaceControlCurrent->applyInput(ControlSS::PITCHPOS, y * -0.06f);
                }
                bank->pSpaceControlCurrent->angBrakeEngageCounter = 0;
                break;
            case ControlTypes::NONE:
                break;
            default:
                break;
        }
    }

    void System_UserInput::keyPressed_backQuote() {
        if (!MenuIsActive) {
            consoleIsActive = !consoleIsActive;
            consoleIsActive ? SDL_StartTextInput() : SDL_StopTextInput();
            #ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
            SDL_SetRelativeMouseMode(consoleIsActive ? SDL_FALSE : SDL_TRUE);
            #endif
            bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
        }
    }

    void System_UserInput::keyPressed_esc() {
        MenuIsActive = !MenuIsActive;
        if (MenuIsActive) {
            if (!consoleIsActive) {
                consoleIsActive = true;
                SDL_StartTextInput();
                #ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
                SDL_SetRelativeMouseMode(SDL_FALSE);
                #endif
            }
            bank->dlgt->clearCommand();
            bank->dlgt->output(menuText.c_str());
            bank->dlgt->pause();
        } else {
            consoleIsActive = false;
            SDL_StopTextInput();
            #ifndef DUA_DEBUG_DISABLE_SDL_RELATIVE_MOUSE
            SDL_SetRelativeMouseMode(SDL_TRUE);
            #endif
            bank->dlgt->resume();
        }
        bank->dlgt->setConsoleState(consoleIsActive, MenuIsActive);
    }

}

