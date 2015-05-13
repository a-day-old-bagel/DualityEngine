/************************************************
 * File:   Game.cpp
 * Author: adayoldbagel (Galen Cochrane)
 *
 * Created on January 27, 2015, 9:14 PM
 ************************************************/

#include "../Headers/Game.h"

using namespace DualityEngine;

//<editor-fold defaultstate="collapsed" desc="Constructor">
Game::Game()
{
    nullifyPointers();
    makeDelegates();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
/**************************************
 * ~GAME : Game Destructor
 * de-allocates memory and frees pointers.
 *************************************/
Game::~Game()
{
    freeMemory();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Begin">
/**************************************
 * BEGIN
 * called once from where the game is
 * instantiated. Maybe will bring up a
 * main menu at some point...
 *************************************/
void Game::Begin()
{
    // right now, begin just starts a new game.
    NewGame();  
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Menu">
/**************************************
 * MENU
 *************************************/
void Game::Menu()
{
    pauseEngines();
    char input;
    bool menuDone = false;
    std::cout << "****************************************\n"
              << "* MENU:\n"
              << "*    Q: Quit\n"
              << "*    N: New Game\n"
              << "*    R: Resume\n"
              << "****************************************\n"
              << ">: ";
    std::cin >> input;
    
    while (!menuDone)
    {
        switch (input)
        {
            case 'Q':
            case 'q':
                Quit();
                menuDone = true;
                break;
            case 'N':
            case 'n':
                NewGame();
                menuDone = true;
                break;
            case 'R':
            case 'r':
                resumeEngines();
                menuDone = true;
                break;
            default:
                std::cout << "bad input.\n>: ";
                std::cin >> input;
                break;
        }
    }
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="New Game">
/**************************************
 * NEW GAME : Game Initializer
 * sets up game
 *************************************/
void Game::NewGame()
{    
    initializeSDLwindow();
    initializeECS();
    initializeEngines();
    engageEngines();
        
    // Wait for all game threads to exit, then the game is over.
    SDL_WaitThread(physicsThread, NULL);
    SDL_WaitThread(graphicsThread, NULL);
    
    //std::cout << consoleText;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Pause">
/**************************************
 * PAUSE
 *************************************/
void Game::Pause()
{
    pauseEngines();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume">
/**************************************
 * RESUME
 *************************************/
void Game::Resume()
{
    resumeEngines();
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Quit">
/**************************************
 * QUIT
 *************************************/
void Game::Quit()
{
    killEngines();
}
//</editor-fold>

//<editor-fold defaultstate="collapsed" desc="Initialize SDL Window">
/**************************************
 * INITIALIZE SDL WINDOW
 * creates the window, which is later
 * passed to the graphics engine.
 * I'm doing this outside of the
 * graphics engine because it alleviates
 * certain errors that occur when a new
 * game is made after one's been running.
 *************************************/
bool Game::initializeSDLwindow()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf ("SDL did not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Specify OpenGL version and profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Settings::GLmajorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Settings::GLminorVersion);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    // Create the SDL window
    window = SDL_CreateWindow("Game Engine",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    Settings::screenWidth, Settings::screenHeight,
                    SDL_WINDOW_OPENGL);
    
    // If the window couldn't be created for whatever reason
    if (window == NULL) {
        printf ("SDL window was not created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Initialize ECS">
/**************************************
 * INITIALIZE ECS
 * allocates a new component bank and
 * all the Systems.
 *************************************/
bool Game::initializeECS()
{
    bank = ComponentBank();   
    renderingSystem = new System_Render(&bank, window);
    physicsMoveSystem = new System_PhysMove(&bank);
    physicsCollisionSystem = new System_PhysCollide(&bank);
    userControlSystem = new System_UserControl(&bank, controlDelegates);
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Make Delegates">
/**************************************
 * MAKE DELEGATES
 *************************************/
bool Game::makeDelegates(){
    quitDelegate = new VoidDelegate(VOIDDELEGATE(&Game::Quit, this));
    outputDelegate = new StringDelegate(DelegateFactory<Console, void, const char*>()
                                        .Create<&Console::output>(&console));
    
    controlDelegates = new DelegateBag;
    controlDelegates->menu = VOIDDELEGATE(&Game::Menu, this);
    controlDelegates->quit = *quitDelegate;
    controlDelegates->output = *outputDelegate;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Initialize Engines">
/**************************************
 * INITIALIZE ENGINES
 * allocates all the engines and adds
 * to them the appropriate Systems.
 *************************************/
bool Game::initializeEngines()
{    
    graphicsEngine = new SystemEngine(&graphicsThread, "Duality Graphics Engine", outputDelegate, quitDelegate);
    graphicsEngine->addSystem(renderingSystem);
    
    physicsEngine = new SystemEngine(&physicsThread, "Duality Physics Engine", outputDelegate, quitDelegate);
    physicsEngine->addSystem(physicsMoveSystem);
    physicsEngine->addSystem(physicsCollisionSystem);
    physicsEngine->addSystem(userControlSystem);
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Start Engines">
/**************************************
 * START ENGINES
 * engages each engine, creating all
 * the threads in the process.
 *************************************/
bool Game::engageEngines()
{
    graphicsEngine->engage();
    physicsEngine->engage();      // PHYSICS SYSTEMS NOT YET IMPLEMENTED
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Pause Engines">
/**************************************
 * PAUSE ENGINES
 *************************************/
bool Game::pauseEngines()
{
    physicsMoveSystem->pause();
    physicsCollisionSystem->pause();
    renderingSystem->pause();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Resume Engines">
/**************************************
 * RESUME ENGINES
 *************************************/
bool Game::resumeEngines()
{
    physicsMoveSystem->resume();
    physicsCollisionSystem->resume();
    renderingSystem->resume();
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Kill Engines">
/**************************************
 * QUIT ENGINES
 * Tell each system to quit, which
 * should trigger their respective
 * engines to exit their loops.
 * Then wait for threads to exit.
 *************************************/
bool Game::killEngines()
{
    renderingSystem->quit();
    physicsMoveSystem->quit();
    physicsCollisionSystem->quit();
    userControlSystem->quit();
    
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Nullify Pointers">
/**************************************
 * NULLIFY POINTERS
 *************************************/
void Game::nullifyPointers()
{
    window = NULL;
    controlDelegates = NULL;
    quitDelegate = NULL;
    outputDelegate = NULL;
    
    renderingSystem = NULL;
    physicsMoveSystem = NULL;
    physicsCollisionSystem = NULL;
    userControlSystem = NULL;
    
    graphicsEngine = NULL;
    physicsEngine = NULL;
    graphicsThread = NULL;
    physicsThread = NULL;   
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Free Memory">
/**************************************
 * FREE MEMORY
 *************************************/
void Game::freeMemory()
{
    DU_POINTER_DELETE(controlDelegates);
    DU_POINTER_DELETE(quitDelegate);
    DU_POINTER_DELETE(outputDelegate);
    
    DU_POINTER_DELETE(renderingSystem);
    DU_POINTER_DELETE(physicsMoveSystem);
    DU_POINTER_DELETE(physicsCollisionSystem);    
    DU_POINTER_DELETE(userControlSystem);  
    
    DU_POINTER_DELETE(graphicsEngine);
    DU_POINTER_DELETE(physicsEngine);
}
//</editor-fold>