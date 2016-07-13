//
// Created by volundr on 7/11/16.
//

#ifndef DUALITYENGINE_FONTREPO_H
#define DUALITYENGINE_FONTREPO_H

#include <string>
#include <unordered_map>
#include <SDL_mutex.h>
#include "loadFont.h"

namespace DualityEngine {
    class FontRepo {
        typedef std::unordered_map<std::string, FontDescriptor>::const_iterator loadedFontIterator;
        std::unordered_map<std::string, FontDescriptor> loadedFonts;
        std::string assembleKey(const char* fontName, FontDescriptor& font);
        void add(const char* fontName, FontDescriptor& font, std::stringstream& out);
    public:
        FontRepo();
        ~FontRepo();
        int request(const char* fontName, FontDescriptor& font, std::stringstream& out);
    };
}

#endif //DUALITYENGINE_FONTREPO_H
