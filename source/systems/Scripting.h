/*******************************************************************************
 * File:   Scripting.h
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:38 AM
 * 
 * 
 ******************************************************************************/

#ifndef SCRIPTING_H
#define	SCRIPTING_H

#include "System.h"
#include <queue>

namespace DualityEngine {

    class System_Scripting : public System<System_Scripting>
    {
    private:
        
        std::queue<std::string> commandQueue;
        std::unordered_map<std::string, DUA_id> entityVariables;
        
        DUA_id prsID(const std::string& IDstring);
        int prsInt(const std::string& intString);
        DUA_dbl prsDbl(const std::string& dblString);
        DUA_float prsFlt(const std::string& floatString);
        DUA_colorByte prsClr(const std::string& colorValue);
        void parseCommand(const std::string& command);
        void parseAddCommand(const std::vector<std::string>& args);
        void parseKeyword_all(const std::vector<std::string>& args);
        void parseAssignment(const std::vector<std::string>& args);
        void parseRemoveCommand(const std::vector<std::string>& args);
        void handleBadUsage(const std::string& command);

    public:
        System_Scripting(Bank *);
        void onTick();
        bool onInit(std::stringstream&);
        void onClean();
        void submitCommand(const std::string&);
        void submitScript(const std::string&);
    };

}

#endif	/* SCRIPTING_H */

