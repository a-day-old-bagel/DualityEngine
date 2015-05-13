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
        std::stringstream log;
        std::vector<std::string> lines;
        int lineIterator = 0;
    public:
        void output (const char* text);
        std::string getLog ();
        std::string getLine (int line);
    };
}

#endif	/* CONSOLE_H */

