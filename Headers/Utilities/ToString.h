/*
 * Created by Galen on 1/10/2016.
 *
 * These are quick helper functions to use in place of c++11's std::to_string and various std::sto_ functions.
 * Windows c++ compilers don't like them (known bug in GCC on windows)
 */

#ifndef DUALITYENGINE_TOSTRING_H
#define DUALITYENGINE_TOSTRING_H

#include "Settings.h"

namespace DualityEngine {
    std::string idToStr(DUA_id n);
//    template<typename T>
//    T strTo(const char* str);
}

#endif //DUALITYENGINE_TOSTRING_H
