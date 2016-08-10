//
// Created by volundr on 7/11/16.
//

#include "FontRepo.h"

namespace DualityEngine {

    FontRepo::FontRepo() {

    }

    FontRepo::~FontRepo() {

    }

    std::string FontRepo::assembleKey(const char* fontName, FontDescriptor& font) {
        std::stringstream keyBuilder;
        keyBuilder << fontName;
        keyBuilder << " " << font.panelW << "x" << font.panelH;
        keyBuilder << (font.sdf ? "_sdf" : "");
        return keyBuilder.str();
    }

    bool FontRepo::add(const char* fontName, FontDescriptor& font, std::stringstream& out) {
        if (font.sdf ? loadSDFfont(font, fontName, out) : loadFont(font, fontName, out)) {
            loadedFonts.emplace(assembleKey(fontName, font), font);
            return true;
        }
        return false;
    }

    int FontRepo::request(const char* fontName, FontDescriptor& font, std::stringstream& out) {
        loadedFontIterator result = loadedFonts.find(assembleKey(fontName, font));
        if (result == loadedFonts.end()) {
            if (add(fontName, font, out)) {
                out << "Font '" << assembleKey(fontName, font) << "' loaded into repo.\n";
                return 1;
            } else {
                out << "Font '" << assembleKey(fontName, font) << "' could not be loaded!\n";
            }
        } else {
            out << "Font '" << assembleKey(fontName, font) << "' retrieved from repo.\n";
            font = result->second;
        }
        return 0;
    }

}