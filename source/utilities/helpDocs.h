//
// Created by adayoldbagel on 1/8/16.
//

#ifndef DUALITYENGINE_HELPDOCS_H
#define DUALITYENGINE_HELPDOCS_H

#include <unordered_map>
#include "settings.h"

// unfortunately a typedef won't do what I want here...
#define DUA_COMP_TUPLE std::tuple<std::string, std::string, DUA_compFlag>

namespace DualityEngine {
    struct Docs {
        static const std::array<DUA_COMP_TUPLE, 16> componentCollections;
        static const std::unordered_map<std::string, std::string> commandUsages;
        static const std::unordered_map<std::string, std::string> commandExamples;
        static const std::unordered_map<std::string, std::string> commandHelps;
        static const std::unordered_map<std::string, std::string> componentHelps;
        static const std::unordered_map<std::string, std::pair<std::string, int>> componentArgs;
    };
}

#endif //DUALITYENGINE_HELPDOCS_H
