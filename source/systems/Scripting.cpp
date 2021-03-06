/*******************************************************************************
 * File:   Scripting.cpp
 * Author: Galen Cochrane *
 * Created on February 24, 2015, 1:39 AM
 * 
 * 
 ******************************************************************************/

#include <SDL.h>
#include <fstream>
#include "Scripting.h"

#define DUA_COMPCOLL(x, y) std::get<y>(Docs::componentCollections[x])

namespace DualityEngine {

	bool isPositiveInteger(const char* str) {
		std::string temp(str);
		for (auto c : temp) {
			if (c < '0' || c > '9') {
				return false;
			}
		}
		return true;
	}

    template<typename T>
    T strTo(const char* str) {
        std::istringstream ss(str);
        T i;
        ss >> i;
        return i;
    }

    System_Scripting::System_Scripting(Bank *bank)
            : System<System_Scripting>(bank, "Scripting System", 0) {
        entityVariables = {};
    }

    bool System_Scripting::onInit(std::stringstream &output) {
        return true;
    }

    void System_Scripting::onTick() {
        while (!commandQueue.empty()) {
            parseCommand(commandQueue.front());
            commandQueue.pop();
        }
        SDL_Delay(20);
    }

    void System_Scripting::onClean() {
        entityVariables.clear();
    }

    void System_Scripting::submitScript(const std::string &fileName) {
        std::string filePath = "assets/scripts/" + fileName + ".dua";
        std::vector<std::string> lines;
        std::string lineReader;
        std::ifstream infile(filePath, std::ios_base::in);
        while (getline(infile, lineReader, '\n')) {
            lines.push_back(lineReader);
        }

        if (lines.empty()) {
            bank->dlgt->outputStr("<!>    " + filePath + ": file either not found, unreadable, or empty.\n");
        } else {
            for (uint i = 0; i < lines.size();) {
                if (lines.at(i).empty() || lines.at(i).at(0) == '#') {
                    lines.erase(lines.begin() + i);
                } else {
                    ++i;
                }
            }

            int numHeaderLinesToErase = 1;

            if (lines.size() < 2) {
                bank->dlgt->outputStr("<!>    " + filePath + ": not a valid Duality script.\n");
            } else {
                std::stringstream firstTwoLines(lines.at(0) + " " + lines.at(1));
                std::string headerTypeDeclaration, headerVersion, headerNumEntities;
                firstTwoLines >> headerTypeDeclaration;
                firstTwoLines >> headerVersion;
                firstTwoLines >> headerNumEntities;
                if (headerNumEntities != "numberOfEntities") {
                    bank->dlgt->outputStr(filePath + ": \"numberOfEntities\" not found. Continuing.");
                } else {
                    numHeaderLinesToErase = 2;
                    headerNumEntities.clear();
                    firstTwoLines >> headerNumEntities;
                    // DO SOMETHING WITH NUMENTITIES - RESERVE SPACE IN HASH TABLES, ETC.
                }
                if (headerTypeDeclaration != "DualityEngineScript" || headerVersion != DUA_VERSION) {
                    bank->dlgt->outputStr("<!>    " + filePath + ": invalid Duality script or wrong version.");
                } else {

                    lines.erase(lines.begin(), lines.begin() + numHeaderLinesToErase);

                    for (auto line : lines) {
                        submitCommand(line);
                    }
                }
            }
        }
    }

    void System_Scripting::submitCommand(const std::string &command) {
        commandQueue.emplace(command);
    }

    void System_Scripting::parseCommand(const std::string &command) {

        if (command.empty()) return;
        std::stringstream commandLine(command);
        std::vector<std::string> args;
        std::string temp;
        while (commandLine >> temp) {
            args.push_back(temp);
            temp.clear();
        }
        const int numArgs = (int)args.size();
        if (numArgs < 1) return;

        try {
            if (args[0] == "newent") {
                if (numArgs == 2) {
                    DUA_id newID = bank->spawnEntity(args[1].c_str());
                    if (newID != DUA_NULL_ID) {
                        bank->dlgt->outputStr(idToStr(newID));
                    } else {
                        bank->dlgt->outputStr("<!>    Failed to create " + args[1] + "!\n");
                    }
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "delent") {
                if (numArgs == 2) {
                    bank->deleteEntity(prsID(args[1]));
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "add") {
                if (numArgs > 2) {
                    parseAddCommand(args);
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "remove") {
                if (numArgs == 3) {
                    parseRemoveCommand(args);
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "id") {
                if (numArgs == 2) {
                    std::vector<DUA_id> IDs;
                    if (bank->getIDs(args[1], IDs)) {
                        if (IDs.size() > 0) {
                            bank->dlgt->outputStr("Entity IDs associated with the name \"" + args[1] + "\": \n");
                            for (auto id : IDs) {
                                bank->dlgt->outputStr(idToStr(id) + "\n");
                            }
                        } else {
                            bank->dlgt->outputStr("<!>    No entities exist named \"" + args[1] + "\".\n");
                        }
                    } else {
                        bank->dlgt->output("<!>    ERROR: bank::getID threw an exception!\n");
                    }
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "help") {
                if (numArgs == 1) {
                    bank->dlgt->output("Here are the existing commands:\n");
                    for (auto cmdUsages : Docs::commandUsages) {
                        bank->dlgt->outputStr("    " + cmdUsages.second);
                    }
                    bank->dlgt->output(
                            "Type \"help components\" to see a list of available components, \"help [command]\" for command-specific help. ~ key accesses console during play. Use PageUp/PageDown or SHIFT + up/down arrow to scroll.");
                } else if (numArgs == 2) {
                    if (Docs::commandHelps.count(args[1])) {
                        bank->dlgt->outputStr("The " + args[1] + " command " + Docs::commandHelps.at(args[1]) + "\n" +
                                              "Usage: " + Docs::commandUsages.at(args[1]) + "\n" +
                                              "Example: " + Docs::commandExamples.at(args[1]) + "\n");
                    } else if (Docs::componentHelps.count(args[1])) {
                        bank->dlgt->outputStr("The " + args[1] + " component " + Docs::componentHelps.at(args[1]) + "\n" +
                                              "Arguments: " + Docs::componentArgs.at(args[1]).first + "\n");
                    } else if (args[1] == "components") {
                        std::string compList;
                        for (auto it = Docs::componentHelps.begin(); it != Docs::componentHelps.end(); ++it) {
                            compList.append(it->first + ", ");
                        }
                        bank->dlgt->outputStr("Available components are:\n" + compList +
                                              "\nUse \"help [component]\" for more info on a specific component.\n");
                    } else {
                        bank->dlgt->outputStr("<!>    No documentation for: " + args[1] + "\n");
                    }
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "comps") {
                if (numArgs == 2) {
                    std::string outStr = bank->listComponentsVerbose(prsID(args[1]));
                    if (!outStr.empty()) {
                        bank->dlgt->outputStr(outStr);
                    }
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "name") {
                if (numArgs == 2) {
                    std::string outStr = bank->getNameVerbose(prsID(args[1]));
                    if (!outStr.empty()) {
                        bank->dlgt->outputStr(outStr);
                    }
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "cam") {
                if (numArgs == 2) {
                    bank->switchToCam(prsID(args[1]));
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "control") {
                if (numArgs == 3) {
                    if (args[2] == "spaceship") {
                        bank->switchToControl(prsID(args[1]), ControlTypes::SPACE);
                    } else if (args[2] == "walking") {
                        bank->dlgt->output("<!>    walking control interface not yet implemented");
                    } else if (args[2] == "none") {
                        bank->switchToControl(DUA_NULL_ID, ControlTypes::NONE);
                    } else {
                        handleBadUsage(args[0]);
                    }

                } else if (numArgs == 2) {
                    if (args[1] == "none") {
                        bank->switchToControl(DUA_NULL_ID, ControlTypes::NONE);
                    } else {
                        handleBadUsage(args[0]);
                    }
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "exit") {
                if (numArgs == 1) {
                    bank->dlgt->quit();
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "new") {
                if (numArgs == 1) {
                    bank->dlgt->newGame();
                } else if (numArgs == 2) {
                    bank->dlgt->newGame();
                    bank->dlgt->runScript(args[1]);
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "run") {
                if (numArgs == 2) {
                    bank->dlgt->runScript(args[1]);
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "load") {
                if (numArgs == 2) {
                    bank->dlgt->output("<!>    load game command not yet implemented\n");
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (args[0] == "save") {
                if (numArgs == 2) {
                    bank->dlgt->output("<!>    save game command not yet implemented\n");
                } else {
                    handleBadUsage(args[0]);
                }
            } else if (numArgs > 1 && args[1] == "=") {
                parseAssignment(args);
            } else if (args[0] == "sky") {
                if (numArgs == 3) {
                    bank->dlgt->switchSky(args[1], args[2]);
                } else {
                    handleBadUsage(args[0]);
                }
			}
			else if (args[0] == "clear" || args[0] == "cls") {
				if (numArgs == 2){
//					std::string::size_type sz;   // alias of size_t
					int i_dec = 0;
					try {
//						i_dec = std::stoi(args[1], &sz);
                        i_dec = prsInt(args[1]);
					}
					catch (...) {
						bank->dlgt->outputStr("ERROR: ");
					}
					if (i_dec <= 0) {
						bank->dlgt->outputStr(args[1] + " is not greater than 0.");
					}
					else {
						for (int i = 0; i < i_dec; ++i) {
							bank->dlgt->outputStr("\n");
						}
					}
				}
				else {
					bank->dlgt->outputStr("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
				}
			} else {
                bank->dlgt->outputStr("<!>    Bad command: " + args[0] + ". Type \"help\" for a list of commands.\n");
            }
        } catch (const char *error) {
            bank->dlgt->output(error);
        }
    }

    void System_Scripting::parseAssignment(const std::vector<std::string> &args) {
        int numRhsArgs = (int)args.size() - 2;
        if (args[2] == "newent") {
            if (numRhsArgs == 2) {
                DUA_id newID = bank->spawnEntity(args[0].c_str());
                if (newID != DUA_NULL_ID) {
                    entityVariables[args[0]] = newID;
                    bank->dlgt->outputStr(
                            "Entity " + idToStr(newID) + " has been assigned variable: (CTRL-C to copy)\n" +
                            args[0]);
                } else {
                    bank->dlgt->outputStr("<!>    Failed to create " + args[1] + "!\n");
                }
            } else {
                handleBadUsage(args[2]);
            }
        } else {
            bank->dlgt->output("<!>    Bad assignment. Use \"[variable] = newent [name]\".");
        }
    }

    void System_Scripting::parseAddCommand(const std::vector<std::string> &args) {

        try {
            int numCompArgs = (int)args.size() - 3;
            if (Docs::componentHelps.count(args[1])) {
                if (numCompArgs != Docs::componentArgs.at(args[1]).second) {
                    if (numCompArgs == 2 && args[3] == "all") {
                        parseKeyword_all(args);
                        return;
                    }
                    bank->dlgt->outputStr("<!>    Wrong number of arguments for a " + args[1] + " component.\n");
                    bank->dlgt->outputStr(Docs::componentArgs.at(args[1]).first);
                    return;
                }
            }
            DUA_id entID = prsID(args[2]);

            if (args[1] == DUA_COMPCOLL(1, 1)) {
                bank->addModel(entID, args[3].c_str());
            } else if (args[1] == DUA_COMPCOLL(2, 1)) {
                bank->addPosition(entID, prsDbl(args[3]), prsDbl(args[4]), prsDbl(args[5]));
            } else if (args[1] == DUA_COMPCOLL(3, 1)) {
                bank->addSpatialChild(entID, prsID(args[3]));
            } else if (args[1] == DUA_COMPCOLL(4, 1)) {
                bank->addSpatialParent(entID, prsID(args[3]));
            } else if (args[1] == DUA_COMPCOLL(7, 1)) {
                bank->addOrientation(entID, prsDbl(args[3]), prsDbl(args[4]), prsDbl(args[5]));
            } else if (args[1] == DUA_COMPCOLL(5, 1)) {
                bank->addLinearVeloc(entID, prsDbl(args[3]), prsDbl(args[4]), prsDbl(args[5]));
            } else if (args[1] == DUA_COMPCOLL(8, 1)) {
                bank->addAngularVeloc(entID, prsDbl(args[3]), prsDbl(args[4]), prsDbl(args[5]));
            } else if (args[1] == DUA_COMPCOLL(9, 1)) {
                bank->addSpaceControl(entID, prsFlt(args[3]), prsFlt(args[4]), prsFlt(args[5]), prsFlt(args[6]),
                                      prsFlt(args[7]), prsFlt(args[8]), prsFlt(args[9]), prsFlt(args[10]),
                                      prsFlt(args[11]), prsFlt(args[12]), prsFlt(args[13]), prsFlt(args[14]));
            } else if (args[1] == DUA_COMPCOLL(12, 1)) {
                bank->addAmbientLight(entID, prsClr(args[3]), prsClr(args[4]), prsClr(args[5]));
            } else if (args[1] == DUA_COMPCOLL(11, 1)) {
                bank->addDirectionalLight(entID, prsClr(args[3]), prsClr(args[4]), prsClr(args[5]), prsDbl(args[6]),
                                          prsDbl(args[7]), prsDbl(args[8]));
            } else if (args[1] == DUA_COMPCOLL(10, 1)) {
                bank->addPointLight(entID, prsClr(args[3]), prsClr(args[4]), prsClr(args[5]), prsDbl(args[6]),
                                    prsDbl(args[7]), prsDbl(args[8]));
            } else if (args[1] == DUA_COMPCOLL(13, 1)) {
                bank->addOwner(entID, prsID(args[3]));
            } else if (args[1] == DUA_COMPCOLL(14, 1)) {
                bank->addScore(entID);
            } else if (args[1] == DUA_COMPCOLL(6, 1)) {
                bank->addCollision(entID);
            } else if (args[1] == DUA_COMPCOLL(15, 1)) {
                bank->addCameraFree(entID, prsFlt(args[3]), prsFlt(args[4]), prsFlt(args[5]), prsDbl(args[6]),
                                    prsDbl(args[7]), prsDbl(args[8]), prsDbl(args[9]), prsDbl(args[10]),
                                    prsDbl(args[11]), prsDbl(args[12]), prsDbl(args[13]), prsDbl(args[14]));
            } else {
                bank->dlgt->outputStr("<!>    Unknown component: " + args[1] + "\n");
            }
        } catch (const char *error) {
            bank->dlgt->output(error);
        }
    }

    void System_Scripting::parseKeyword_all(const std::vector<std::string> &args) {
        try {
            DUA_id entID = prsID(args[2]);
            if (args[1] == DUA_COMPCOLL(9, 1)) {
                float pwr = prsFlt(args[4]);
                bank->addSpaceControl(entID, pwr, pwr, pwr, pwr, pwr, pwr, pwr, pwr, pwr, pwr, pwr, pwr);
            } else {
                bank->dlgt->outputStr("<!>    \"all\" keyword not supported for component: " + args[1] + "\n");
            }
        } catch (const char *error) {
            bank->dlgt->output(error);
        }
    }

    void System_Scripting::parseRemoveCommand(const std::vector<std::string> &args) {

        try {
            DUA_id entID = prsID(args[2]);
            if (entID == DUA_NULL_ID) return;

            if (args[1] == DUA_COMPCOLL(1, 1)) {
                bank->deleteModel(entID);
            } else if (args[1] == DUA_COMPCOLL(2, 1)) {
                bank->deletePosition(entID);
            } else if (args[1] == DUA_COMPCOLL(3, 1)) {
                bank->deleteSpatialChild(entID);
            } else if (args[1] == DUA_COMPCOLL(4, 1)) {
                bank->deleteSpatialParent(entID);
            } else if (args[1] == DUA_COMPCOLL(7, 1)) {
                bank->deleteOrientation(entID);
            } else if (args[1] == DUA_COMPCOLL(5, 1)) {
                bank->deleteLinearVeloc(entID);
            } else if (args[1] == DUA_COMPCOLL(8, 1)) {
                bank->deleteAngularVeloc(entID);
            } else if (args[1] == DUA_COMPCOLL(9, 1)) {
                bank->deleteSpaceControl(entID);
            } else if (args[1] == DUA_COMPCOLL(12, 1)) {
                bank->deleteAmbientLight(entID);
            } else if (args[1] == DUA_COMPCOLL(11, 1)) {
                bank->deleteDirectionalLight(entID);
            } else if (args[1] == DUA_COMPCOLL(10, 1)) {
                bank->deletePointLight(entID);
            } else if (args[1] == DUA_COMPCOLL(13, 1)) {
                bank->deleteOwner(entID);
            } else if (args[1] == DUA_COMPCOLL(14, 1)) {
                bank->deleteScore(entID);
            } else if (args[1] == DUA_COMPCOLL(6, 1)) {
                bank->deleteCollision(entID);
            } else if (args[1] == DUA_COMPCOLL(15, 1)) {
                bank->deleteCameraFree(entID);
            } else {
                bank->dlgt->outputStr("<!>    Unknown component: " + args[1] + "\n");
            }
        } catch (const char *error) {
            bank->dlgt->output(error);
        }
    }

    DUA_id System_Scripting::prsID(const std::string &IDstring) {
        DUA_id entID;
		try {
			if (isPositiveInteger(IDstring.c_str())) {
				entID = strTo<DUA_id>(IDstring.c_str());
				if (entID > std::numeric_limits<DUA_id>::max() || entID < 0) {
					throw std::out_of_range("DUA_id");
				}
			}
			else {
				entID = entityVariables.at(IDstring);
			}
		}
		catch (std::out_of_range &oorException) {
			throw std::string("<!>    Not a valid ID or variable name: " + IDstring + "\n").c_str();
		}
        //try {
        //    entID = strTo<DUA_id>(IDstring.c_str());
        //    if (entID > std::numeric_limits<DUA_id>::max() || entID < 0) {
        //        throw std::out_of_range("DUA_id");
        //    }
        //} catch (
        //        std::invalid_argument &invalidException) { // BAD BAD BAD - Should not use exceptions for normal operation - right now used every time a variable name is used in a script or in the console.
        //    try {
        //        entID = entityVariables.at(IDstring);
        //    } catch (std::out_of_range &oorException) {
        //        throw std::string("<!>    Not a valid ID or variable name: " + IDstring + "\n").c_str();
        //    }
        //} catch (std::out_of_range &oorException) {
        //    throw std::string("<!>    ID out of range: " + IDstring + "\n").c_str();
        //}
        return entID;
    }

    int System_Scripting::prsInt(const std::string &intString) {
        int val = 0;
        try {
            val = strTo<int>(intString.c_str());
        } catch (std::invalid_argument &invalidException) {
            throw std::string("<!>    Not a valid value: " + intString + "\n").c_str();
        } catch (std::out_of_range &oorException) {
            throw std::string("<!>    Value out of range: " + intString + "\n").c_str();
        }
        return val;
    }

    DUA_dbl System_Scripting::prsDbl(const std::string &dblString) {
        DUA_dbl dbl;
        try {
            dbl = strTo<DUA_dbl>(dblString.c_str());
//        if (dbl > std::numeric_limits<DUA_dbl>::max() || dbl > std::numeric_limits<DUA_dbl>::min()) {
//            throw std::out_of_range("DUA_dbl");
//        }
        } catch (std::invalid_argument &invalidException) {
            throw std::string("<!>    Not a valid value: " + dblString + "\n").c_str();
        } catch (std::out_of_range &oorException) {
            throw std::string("<!>    Value out of range: " + dblString + "\n").c_str();
        }
        return dbl;
    }

    DUA_float System_Scripting::prsFlt(const std::string &floatString) {
        DUA_float flt;
        try {
            flt = strTo<DUA_float>(floatString.c_str());
        } catch (std::invalid_argument &invalidException) {
            throw std::string("<!>    Not a valid value: " + floatString + "\n").c_str();
        } catch (std::out_of_range &oorException) {
            throw std::string("<!>    Value out of range: " + floatString + "\n").c_str();
        }
        return flt;
    }

    DUA_colorByte System_Scripting::prsClr(const std::string &colorValue) {
        DUA_colorByte colorVal;
        try {
            colorVal = strTo<DUA_colorByte>(colorValue.c_str());
            if (colorVal > 255 || colorVal < 0) {
                throw std::out_of_range("DUA_colorByte");
            }
        } catch (std::invalid_argument &invalidException) {
            throw std::string("<!>    Not a valid value: " + colorValue + "\n").c_str();
        } catch (std::out_of_range &oorException) {
            throw std::string("<!>    RGB value out of range: " + colorValue + "\n").c_str();
        }
        return colorVal;
    }

    void System_Scripting::handleBadUsage(const std::string &command) {
        bank->dlgt->outputStr("<!>    Incorrect usage of " + command + ". Use \"help " + command +
                              "\" for more info. Correct usage is:");
        bank->dlgt->outputStr("    " + Docs::commandUsages.at(command));
    }

}
