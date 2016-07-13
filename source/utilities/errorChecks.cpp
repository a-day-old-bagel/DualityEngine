//
// Created by volundr on 7/12/16.
//

#include "errorChecks.h"
#include <GL/glew.h>

namespace DualityEngine {
    /*
     * DO NOT call this function before the gl context has initialized.  It will make an infinite loop.
     * This is because glGetError always returns GL_INVALID_OPERATION before there exists a context.
     */
    void checkError(std::stringstream& engineOut, std::string context, int line /*= -1*/) {
        GLenum glErr = glGetError();
        if (glErr != GL_NO_ERROR) {
            engineOut << "<!>    GL error(s) detected (file " << context;
            if (line > 0) {
                engineOut << " at line " << line;
            }
            engineOut << "):\n";
            int loopGuard = 0;
            while (glErr != GL_NO_ERROR) {
                if (++loopGuard <= 10) {
                    engineOut << "\t\t\t" << gluErrorString(glErr) << std::endl;
                    glErr = glGetError();
                }
                else {
                    engineOut << "\t\t\t<!> Suppressing further errors...\n";
                    break;
                }
            }
        }
    }
}