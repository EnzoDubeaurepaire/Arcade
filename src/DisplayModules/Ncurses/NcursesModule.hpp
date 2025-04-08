/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** ncurse module main menu
*/

#ifndef NCURSES_MODULE_H_
#define NCURSES_MODULE_H_

// Include KeyCodes.hpp first to get our definitions
#include "IDisplayModule.hpp"
#include "KeyCodes.hpp"

// Then undefine the conflicting keys before including ncurses.h
#undef KEY_BACKSPACE
#undef KEY_ENTER
#undef KEY_DOWN
#undef KEY_UP
#undef KEY_LEFT
#undef KEY_RIGHT
#undef KEY_HOME
#undef KEY_END
#undef KEY_PPAGE
#undef KEY_NPAGE
#undef KEY_IC
#undef KEY_DC
#undef KEY_F0
#undef KEY_MOUSE

#include <ncurses.h>
#include <map>
#include <fstream>
#include <memory>

class NcursesModule : public IDisplayModule {
public:
    NcursesModule() = default;
    ~NcursesModule() override = default;

    void initObject(std::map<std::string, std::unique_ptr<IObject>>&) override;
    int getInput() override;
    std::pair<int, int> getMousePos() const override { return {0, 0}; }
    void openWindow() override;
    void closeWindow() override;
    void display(std::map<std::string, std::unique_ptr<IObject>>&) override;
    std::string getName() const override { return "NCURSES"; }

private:
    WINDOW* _window{};
    std::map<std::string, std::string> _asciiArt;
    int mapKey(int ncursesKey);
};

#endif