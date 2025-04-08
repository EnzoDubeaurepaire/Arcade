/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** NcursesModule
*/

#include "NcursesModule.hpp"
#include <string>

void NcursesModule::openWindow() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    this->_window = newwin(LINES, COLS, 0, 0);
    keypad(_window, TRUE);
}

void NcursesModule::closeWindow() {
    endwin();
}

int NcursesModule::mapKey(int ncursesKey) {
    switch (ncursesKey) {
    case '\033':
        return KEY_ESC;
    case KEY_BACKSPACE:
        return 8;  // Our KEY_BACKSPACE value
    case KEY_UP:
        return 259;  // Our KEY_UP value
    case KEY_DOWN:
        return 258;  // Our KEY_DOWN value
    case KEY_LEFT:
        return 260;  // Our KEY_LEFT value
    case KEY_RIGHT:
        return 261;  // Our KEY_RIGHT value
    case KEY_ENTER:
    case '\n':
        return 10;  // Our KEY_ENTER value
    case ERR:
        return 0;
    default:
        if (ncursesKey >= 'A' && ncursesKey <= 'Z')
            return ncursesKey + ('a' - 'A');
        return ncursesKey;
    }
}

int NcursesModule::getInput() {
    int ch = wgetch(_window);
    if (ch == 17) // CTRL+Q
        return CTRL('q');
    return mapKey(ch);
}

void NcursesModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& obj : objects) {
        if (obj.second->getType() == TEXT) {
            std::ifstream file(obj.second->getTexturePath() + "/text.txt");
            std::string line;
            std::string content;
            while (std::getline(file, line))
                content += line + "\n";
            this->_asciiArt[obj.first] = content;
        }
    }
}

void NcursesModule::display(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    werase(_window);
    box(_window, 0, 0);

    // Center title
    for (const auto& obj : objects) {
        if (obj.first == "title") {
            auto props = std::get<IObject::TextProperties>(obj.second->getProperties());
            std::string title = props.text;
            int x = (COLS - title.length()) / 2;
            mvwprintw(_window, 2, x, "%s", title.c_str());
        }
    }

    // Display libraries with arrow and color
    for (const auto& obj : objects) {
        if (obj.second->getType() == TEXT && obj.first != "title" && obj.first != "selector") {
            auto pos = obj.second->getPosition();
            auto props = std::get<IObject::TextProperties>(obj.second->getProperties());

            // Center items horizontally, moved slightly to the right
            int x = (COLS / 2) + 10;

            // Check if this is the selected item
            bool isSelected = false;
            auto selector = objects.find("selector");
            if (selector != objects.end()) {
                auto selectorPos = selector->second->getPosition();
                isSelected = (selectorPos.second == pos.second);
            }

            if (isSelected) {
                wattron(_window, COLOR_PAIR(1));
                mvwprintw(_window, pos.second / 20, x - 2, ">");
                mvwprintw(_window, pos.second / 20, x, "%s", props.text.c_str());
                wattroff(_window, COLOR_PAIR(1));
            } else {
                mvwprintw(_window, pos.second / 20, x, "%s", props.text.c_str());
            }
        }
    }

    wrefresh(_window);
}