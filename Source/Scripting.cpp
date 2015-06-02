/*******************************************************************************
 * File:   Scripting.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include "../Headers/UserControl.h"
#include "Scripting.h"

using namespace DualityEngine;

//<editor-fold>

System_Scripting::System_Scripting(ComponentBank* bank, ScriptingDelegates* delegates)
                  : System(bank, "Scripting System", 0){
    this->dlgt = delegates;
}

System_Scripting::~System_Scripting(){
    dlgt = NULL;
}

bool System_Scripting::init(std::stringstream& output){
    return true;
}

void System_Scripting::tick(){
    while(!commandQueue.empty()){
        parseCommand(commandQueue.front());
        commandQueue.pop();
    }
}

void System_Scripting::submitCommand(const std::string& command){
    commandQueue.emplace(command);
}

void System_Scripting::parseCommand(const std::string& command){
    
    if(command.empty()) return;
    std::stringstream commandLine(command);
    std::vector<std::string> args;
    std::string temp;
    while (commandLine >> temp){
        args.push_back(temp);
        temp.clear();
    }
    const int numArgs = args.size();
    if (numArgs < 1) return;
   
    try{
        if (args[0] == "newent"){
            if (numArgs == 2){
                DUA_id newID = bank->createEntity(args[1].c_str());
                if (newID != DUA_NULL_ID){
                    dlgt->outputStr(std::to_string(newID));
                } else {
                    dlgt->outputStr("Failed to create " + args[1] + "!\n");
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "delent"){
            if (numArgs == 2){
                bank->deleteEntity(tryResolveID(args[1]));
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "add"){
            if (numArgs > 2){
                parseAddCommand(args);
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "remove"){
            if (numArgs == 3){
                parseRemoveCommand(args);
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "id"){
            if (numArgs == 2){
                std::vector<DUA_id> IDs;
                if (bank->getIDs(args[1], IDs)){
                    if (IDs.size() > 0){
                        dlgt->outputStr("Entity IDs associated with the name \"" + args[1] + "\": \n");
                        for (auto id : IDs){
                            dlgt->outputStr(std::to_string(id) + "\n");
                        }
                    } else {
                        dlgt->outputStr("No entities exist named \"" + args[1] + "\".\n");
                    }
                } else {
                    dlgt->output("ERROR: ComponentBank::getID threw an exception!\n");
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "help"){
            if (numArgs == 1){
                dlgt->output("You're in the console. You have access to the following commands:\n");
                for (auto command : commandUsages){
                    dlgt->outputStr("    " + command.second);
                }
                dlgt->output("Type \"help components\" to see a list of available components, \"help [command]\" for command-specific help.\n");
            } else if (numArgs == 2){
                if (commandHelps.count(args[1])){
                    dlgt->outputStr("The " + args[1] + " command " + commandHelps[args[1]] + "\n" +
                                    "Usage: " + commandUsages[args[1]] + "\n" +
                                    "Example: " + commandExamples[args[1]] + "\n");
                }else if (componentHelps.count(args[1])){
                    dlgt->outputStr("The " + args[1] + " component " + componentHelps[args[1]] + "\n" +
                                    "Arguments: " + componentArgs[args[1]].first + "\n");
                }else if (args[1] == "components"){
                    std::string compList;
                    for ( auto it = componentHelps.begin(); it != componentHelps.end(); ++it ) {
                        compList.append(it->first + ", ");
                    }
                    dlgt->outputStr("Available components are:\n" + compList + "\nUse \"help [component]\" for more info on a specific component.\n");
                } else {
                    dlgt->outputStr("No documentation for: " + args[1] + "\n");
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "comps"){
            if (numArgs == 2){
                std::string outStr = bank->listComponents(tryResolveID(args[1]));
                if (!outStr.empty()){
                    dlgt->outputStr(outStr);
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "name"){
            if (numArgs == 2){
                std::string outStr = bank->getName(tryResolveID(args[1]));
                if (!outStr.empty()){
                    dlgt->outputStr(outStr);
                }
            } else {
                handleBadUsage(args[0]);
            }
        } else if (args[0] == "new" || args[0] == "load" || args[0] == "save" || args[0] == "exit"){
            dlgt->output("Menu commands only available through menu (ESC to enter menu).\n");
        } else {
            dlgt->outputStr("Bad command: " + args[0] + ". Type \"help\" for a list of commands.\n");
        }
    }catch(const char* error){
        dlgt->output(error);
    }
}

void System_Scripting::parseAddCommand(const std::vector<std::string>& args){   
    
    try {
        int numCompArgs = args.size() - 3;
        if (componentHelps.count(args[1])){
            if (numCompArgs != componentArgs[args[1]].second){
                dlgt->outputStr("Wrong number of arguments for a " + args[1] + " component.\n");
                dlgt->outputStr(componentArgs[args[1]].first);
                return;
            }
        }
        DUA_id entID = tryResolveID(args[2]);
        
        if       (args[1] == "model"){
            bank->addModel(entID, args[3].c_str());
        }else if (args[1] == "position"){
            bank->addPosition(entID, tryResolveDbl(args[3]), tryResolveDbl(args[4]), tryResolveDbl(args[5]));
        }else if (args[1] == "poschild"){
            bank->addPositionChild(entID, tryResolveID(args[3]));
        }else if (args[1] == "posparent"){
            bank->addPositionParent(entID, tryResolveID(args[3]));
        }else if (args[1] == "rotation"){
            bank->addRotation(entID, tryResolveDbl(args[3]), tryResolveDbl(args[4]), tryResolveDbl(args[5]));
        }else if (args[1] == "linveloc"){
            bank->addPositionVeloc(entID, tryResolveDbl(args[3]), tryResolveDbl(args[4]), tryResolveDbl(args[5]));
        }else if (args[1] == "angveloc"){
            bank->addRotationVeloc(entID, tryResolveDbl(args[3]), tryResolveDbl(args[4]), tryResolveDbl(args[5]));
        }else if (args[1] == "control"){
            bank->addControl(entID);
        }else if (args[1] == "lambient"){
            bank->addAmbientLight(entID, tryResolveColor(args[3]), tryResolveColor(args[4]), tryResolveColor(args[5]));
        }else if (args[1] == "ldirect"){
            bank->addDirectionalLight(entID, tryResolveColor(args[3]), tryResolveColor(args[4]), tryResolveColor(args[5]), tryResolveDbl(args[6]), tryResolveDbl(args[7]), tryResolveDbl(args[8]));
        }else if (args[1] == "lpoint"){
            bank->addPointLight(entID, tryResolveColor(args[3]), tryResolveColor(args[4]), tryResolveColor(args[5]), tryResolveDbl(args[6]), tryResolveDbl(args[7]), tryResolveDbl(args[8]));
        }else if (args[1] == "owner"){
            bank->addOwner(entID, tryResolveID(args[3]));
        }else if (args[1] == "score"){
            bank->addScore(entID);
        }else if (args[1] == "collision"){
            bank->addCollision(entID);
        }else if (args[1] == "freecam"){
            bank->addCameraFree(entID, tryResolveFloat(args[3]), tryResolveFloat(args[4]), tryResolveFloat(args[5]));
        }else{
            dlgt->outputStr("Unknown component: " + args[1] + "\n");
        }
    } catch(const char* error) {
        dlgt->output(error);
    }
}

void System_Scripting::parseRemoveCommand(const std::vector<std::string>& args){
    
    try{
        DUA_id entID = tryResolveID(args[2]);
        if (entID == DUA_NULL_ID) return;
        
        if       (args[1] == "model"){
            bank->deleteModel(entID);
        }else if (args[1] == "position"){
            bank->deletePosition(entID);
        }else if (args[1] == "poschild"){
            bank->deletePositionChild(entID);
        }else if (args[1] == "posparent"){
            bank->deletePositionParent(entID);
        }else if (args[1] == "rotation"){
            bank->deleteRotation(entID);
        }else if (args[1] == "linveloc"){
            bank->deletePositionVeloc(entID);
        }else if (args[1] == "angveloc"){
            bank->deleteRotationVeloc(entID);
        }else if (args[1] == "control"){
            bank->deleteControl(entID);
        }else if (args[1] == "lambient"){
            bank->deleteAmbientLight(entID);
        }else if (args[1] == "ldirect"){
            bank->deleteDirectionalLight(entID);
        }else if (args[1] == "lpoint"){
            bank->deletePointLight(entID);
        }else if (args[1] == "owner"){
            bank->deleteOwner(entID);
        }else if (args[1] == "score"){
            bank->deleteScore(entID);
        }else if (args[1] == "collision"){
            bank->deleteCollision(entID);
        }else if (args[1] == "freecam"){
            bank->deleteCameraFree(entID);
        }else{
            dlgt->outputStr("Unknown component: " + args[1] + "\n");
        }
    } catch(const char* error) {
        dlgt->output(error);
    }
}

DUA_id System_Scripting::tryResolveID(const std::string& IDstring){
    DUA_id entID = DUA_NULL_ID;
    try {
        entID = DUA_STR_TO_ID(IDstring, 10);
        if (entID > std::numeric_limits<DUA_id>::max() || entID < 0) {
            throw std::out_of_range("DUA_id");
        }
    } catch (std::invalid_argument& invalidException) {
        throw std::string("Not a valid ID: " + IDstring + "\n").c_str();
    } catch (std::out_of_range& oorException) {
        throw std::string("ID out of range: " + IDstring + "\n").c_str();
    }
    return entID;
}

DUA_dbl System_Scripting::tryResolveDbl(const std::string& dblString){
    DUA_dbl dbl = -1;
    try {
        dbl = DUA_STR_TO_DBL(dblString);
//        if (dbl > std::numeric_limits<DUA_dbl>::max() || dbl > std::numeric_limits<DUA_dbl>::min()) {
//            throw std::out_of_range("DUA_dbl");
//        }
    } catch (std::invalid_argument& invalidException) {
        throw std::string("Not a valid value: " + dblString + "\n").c_str();
    } catch (std::out_of_range& oorException) {
        throw std::string("Value out of range: " + dblString + "\n").c_str();
    }
    return dbl;
}

DUA_float System_Scripting::tryResolveFloat(const std::string& floatString){
    DUA_float flt = -1;
    try {
        flt = DUA_STR_TO_FLOAT(floatString);
//        if (flt > std::numeric_limits<DUA_float>::max() || flt > std::numeric_limits<DUA_float>::min()) {
//            throw std::out_of_range("DUA_float");
//        }
    } catch (std::invalid_argument& invalidException) {
        throw std::string("Not a valid value: " + floatString + "\n").c_str();
    } catch (std::out_of_range& oorException) {
        throw std::string("Value out of range: " + floatString + "\n").c_str();
    }
    return flt;
}

DUA_colorByte System_Scripting::tryResolveColor(const std::string& colorValue){
    DUA_colorByte colorVal = -1;
    try {
        colorVal = DUA_STR_TO_COLOR(colorValue, 10);
        if (colorVal > std::numeric_limits<Uint8>::max() || colorVal < 0) {
            throw std::out_of_range("DUA_colorByte");
        }
    } catch (std::invalid_argument& invalidException) {
        throw std::string("Not a valid value: " + colorValue + "\n").c_str();
    } catch (std::out_of_range& oorException) {
        throw std::string("RGB value out of range: " + colorValue + "\n").c_str();
    }
    return colorVal;
}

void System_Scripting::handleBadUsage(const std::string& command){
    dlgt->outputStr("Incorrect usage of " + command + ". Use \"help " + command + "\" for more info. Correct usage is:");
    dlgt->outputStr("    " + commandUsages[command]);
}

//</editor-fold>