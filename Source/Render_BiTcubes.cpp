/****************************************************************
 * Galen Cochrane, 1 FEB 2015
 ****************************************************************/
#include "../Headers/Render_BiTcubes.h"

using namespace DualityEngine;



#define UN
#define US
#define UE
#define UW
#define DN
#define DS
#define DE
#define DW
#define NU
#define ND
#define NE
#define NW
#define SU
#define SD
#define SE
#define SW
#define EU
#define ED
#define EN
#define ES
#define WU
#define WD
#define WN
#define WS





//<editor-fold defaultstate="collapsed" desc="Constructor">
System_Render_BiTcubes::System_Render_BiTcubes(ComponentBank* bank)
                  : System(bank, "Sky Rendering System", 0) 
{
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Destructor">
System_Render_BiTcubes::~System_Render_BiTcubes()
{
    
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Init">
bool System_Render_BiTcubes::init(std::stringstream& engineOut)
{    
    // Load graphics assets and buffer them to GPU
    if(!setUpResources(engineOut)) {
        engineOut << "Unable to initialize sky resources!" << std::endl;
        return false;
    }
    return true;
}
//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Set Up Resources">

bool System_Render_BiTcubes::setUpResources(std::stringstream& engineOut)
{
    bool success = true;
    
             
    return success;
}

//</editor-fold>
//<editor-fold defaultstate="collapsed" desc="Tick">
void System_Render_BiTcubes::tick()
{    
    
}
//</editor-fold>