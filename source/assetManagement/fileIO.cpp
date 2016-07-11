#include "fileIO.h"

using namespace DualityEngine;

bool DualityEngine::ReadFile(const char* pFileName, std::string& outFile)
{
    std::ifstream f(pFileName);
    
    if (f.is_open()) {
        std::string line;
        while (std::getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }
        
        f.close();
        
        return true;
    }
    else {
        printf("\nunable to open file `%s`\n", pFileName);
        return false;
    }
}
