//
// Created by volundr on 7/12/16.
//

#ifndef DUALITYENGINE_ERRORCHECKS_H
#define DUALITYENGINE_ERRORCHECKS_H

#include <string>
#include <sstream>

namespace DualityEngine {
    void checkError(std::stringstream& output, std::string context, int line = -1);
}

#endif //DUALITYENGINE_ERRORCHECKS_H
