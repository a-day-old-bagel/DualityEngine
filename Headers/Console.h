/*******************************************************************************
 * File:   Console.h
 * Author: Galen Cochrane *
 * Created on May 12, 2015, 9:15 PM
 * 
 * 
 ******************************************************************************/

#ifndef CONSOLE_H
#define	CONSOLE_H

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

namespace DualityEngine {
    class Console {
    private:
        std::vector<std::string> lines;
        std::string pendingCommand;
        bool consoleIsActive = false;
        bool menuIsActive = false;
        bool textHasChanged = false;
    public:
        void output (const char* text);
        std::string getLog ();
        std::string getLine (int line);
        std::string getLast ();
        void setState(bool console, bool menu);
        void addToCommand (const char* text);
        void eraseOneCharFromCommand();
        void clearCommand();
        std::string submitCommand();
    };
}

#endif	/* CONSOLE_H */

