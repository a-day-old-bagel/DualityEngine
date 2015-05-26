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
        std::vector<std::string> logLines;
        std::vector<std::string> submittedLines = {""};
        std::string pendingCommand = "";
        int cursorPosition = 0;
        int submitLinePending = 0;
        int submitLineActive = 0;
        
        void refreshPendingCommand();
    public:
        
        bool consoleIsActive = false;
        bool menuIsActive = false;
        bool bodyHasChangedVisually = true;
        bool commHasChangedVisually = true;
        
        void output (const char* text);
        void outputStr (const std::string& text);
        std::string getLog ();
        std::string getLogLine (int line);
        std::string getLogLineFromBack (int lineFromBack);
        std::string getPendingCommand();
        void setState(bool console, bool menu);
        void addToCommand (const char* text);
        void applyBackspace();
        void applyDelete();
        void clearCommand();
        void upOneCommand();
        void downOneCommand();
        void leftCursor();
        void rightCursor();
        std::string submitCommand();
    };
}

#endif	/* CONSOLE_H */

