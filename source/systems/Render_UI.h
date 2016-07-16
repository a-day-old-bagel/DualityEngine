/*
 * File:   Render_Console.h
 * Author: adayoldbagel
 *
 * Created on May 19, 2015, 1:47 PM
 *
 * This is a system that, when instantiated and initialized, sets up (generates) all the
 * resources needed to display a graphical command line interface (console) to the screen.
 * it can be placed wherever, be of whatever size, of whatever colors, and whatever font.
 * upon instantiation, values are passed for margin sizes (horiz and vert), character spacing
 * (horiz and vert), character sizes in pixels (horiz and vert), position of console quad (x and y),
 * width/height of console quad, and font file to access. Freetype 2 is used to read fonts and
 * convert them to bitmaps.  Monspaced fonts are recommended and some tweaking of this file
 * will probably be required to make any one font look good.
 *      The render() method is called to draw the console each frame.
 *      This class also requires a pointer to a DualityEngine::Console class instance from
 * which it can draw current text, cursor positions, etc.
 *
 * PROBLEMS TO FIX: right now scrolling (pgUp, pgDown) in the log scrolls by the unwrapped line,
 *                  meaning that if a single line is displayed across three lines because it was
 *                  too long to fit in one line, pressing pgUp will scroll the whole three lines
 *                  upward instead of the usual one line when that multi-line entry is crossed.
 *                      This is a minor problem and may not need fixing.  So far, all other known
 *                  bugs have been fixed (29 MAY 2015)
 *
 *                  It needs more abstraction.  A generic text box or UI should be possible. This is too ad-hoc.
 */

#ifndef RENDERCONSOLE_H
#define	RENDERCONSOLE_H

// This prevents a ton of compiler warnings
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include "System.h"
#include "Console.h"
#include "settings.h"
#include "loadShaders.h"
#include "FontRepo.h"
#include "TextField.h"
#include "ConsoleDrawer.h"

namespace DualityEngine {

    class System_Render_UI : public System<System_Render_UI>
    {
    private:
        FontDescriptor font;
        FontRepo fontRepo;

        Console* console;
        ConsoleDrawer consoleDrawer;

    public:
        System_Render_UI(Bank * bank, Console* console);
        void tick();
        bool init(std::stringstream& output);
    };

}

#endif


