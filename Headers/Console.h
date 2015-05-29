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
#include <cstring>

namespace DualityEngine {
    class Console {
    private:
        std::vector<std::string> logLines;
        std::vector<std::string> submittedLines = {""};
        std::string pendingCommand = "";
        int submitLinePending = 0;
        int submitLineActive = 0;
        
        void refreshPendingCommand();
    public:
        
        int cursorPosition = 0;
        int logLineTraverser = 0;
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
        void traverseLog(int numLines);
        std::string submitCommand();
    };
}

#endif	/* CONSOLE_H */

