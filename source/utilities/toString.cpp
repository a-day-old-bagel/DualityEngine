/*
 * Created by Galen on 1/10/2016.
 *
 * These are quick helper functions to use in place of c++11's std::to_string and various std::sto_ functions.
 * Windows c++ compilers don't like them (known bug in GCC on windows)
 */

#include <iostream>
#include <sstream>
#include "toString.h"

namespace DualityEngine {
    std::string idToStr(DUA_id n) {
        try {
            char result[50];
            sprintf(result, "%lu", n);
            return std::string(result);
        } catch (...) {
            return "<ERROR>";
        }
    }
//    template<typename T>
//    T strTo(const char* str) {
//        std::istringstream ss(str);
//        T i;
//        ss >> i;
//        return i;
//    }
}
