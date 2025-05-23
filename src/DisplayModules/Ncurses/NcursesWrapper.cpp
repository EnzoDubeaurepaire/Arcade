/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcursesWrapper
*/

#include "NcursesWrapper.hpp"

Arcade::NcursesWrapper::NcursesWrapper() : _window(nullptr), _isInitialized(false) {}

Arcade::NcursesWrapper::~NcursesWrapper() {
    if (_isInitialized)
        cleanup();
}

void Arcade::NcursesWrapper::initialize() {
    if (_isInitialized)
        return;

    _window = initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(50);

    mousemask(ALL_MOUSE_EVENTS, NULL);

    if (has_colors()) {
        start_color();
    }
    clear();
    refresh();
    _isInitialized = true;
}

void Arcade::NcursesWrapper::cleanup() {
    if (!_isInitialized)
        return;
    if (has_colors()) {
        for (int i = 1; i < COLOR_PAIRS; i++) {
            init_pair(i, COLOR_WHITE, COLOR_BLACK);
        }
    }
    use_default_colors();
    clear();
    refresh();
    mousemask(0, NULL);
    curs_set(1);
    echo();
    nocbreak();
    keypad(stdscr, FALSE);
    endwin();
    #ifdef NCURSES_INTERNALS
    if (cur_term != NULL) {
            del_curterm(cur_term);
        }
        set_curterm(NULL);
    #endif
    _isInitialized = false;
}

int Arcade::NcursesWrapper::getKeyInput() {
    return getch();
}

void Arcade::NcursesWrapper::clear() {
    erase();
}

void Arcade::NcursesWrapper::refresh() {
    ::refresh();
}

void Arcade::NcursesWrapper::getTerminalSize(int& width, int& height) {
    getmaxyx(stdscr, height, width);
}

void Arcade::NcursesWrapper::drawChar(int y, int x, char c, int colorPair) {
    if (colorPair > 0)
        attron(COLOR_PAIR(colorPair));

    mvaddch(y, x, c);

    if (colorPair > 0)
        attroff(COLOR_PAIR(colorPair));
}

void Arcade::NcursesWrapper::drawString(int y, int x, const std::string& str, int colorPair) {
    if (colorPair > 0)
        attron(COLOR_PAIR(colorPair));

    mvprintw(y, x, "%s", str.c_str());
    if (colorPair > 0)
        attroff(COLOR_PAIR(colorPair));
}

bool Arcade::NcursesWrapper::hasColors() const {
    return ::has_colors();
}

void Arcade::NcursesWrapper::initColor(int pairId, short fg, short bg) {
    init_pair(pairId, fg, bg);
}

void Arcade::NcursesWrapper::setColor(int pairId, bool on) {
    if (on)
        attron(COLOR_PAIR(pairId));
    else
        attroff(COLOR_PAIR(pairId));
}
