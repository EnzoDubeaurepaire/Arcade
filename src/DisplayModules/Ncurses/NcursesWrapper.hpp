/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcursesWrapper
*/

#ifndef NCURSES_WRAPPER_H_
#define NCURSES_WRAPPER_H_

#include <ncurses.h>
#include <utility>
#include <iostream>

namespace Arcade {
    class NcursesWrapper {
    public:
        NcursesWrapper();
        ~NcursesWrapper();

        void initialize();
        void cleanup();

        int getKeyInput();
        void clear();
        void refresh();
        void getTerminalSize(int& width, int& height);
        void drawChar(int y, int x, char c, int colorPair = 0);
        void drawString(int y, int x, const std::string& str, int colorPair = 0);

        bool hasColors() const;
        void initColor(int pairId, short fg, short bg);
        void setColor(int pairId, bool on = true);

    private:
        WINDOW* _window;
        bool _isInitialized;

        NcursesWrapper(const NcursesWrapper&) = delete;
        NcursesWrapper& operator=(const NcursesWrapper&) = delete;
    };
}

#endif
