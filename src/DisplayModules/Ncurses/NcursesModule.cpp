/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcursesModule
*/

#include "NcursesModule.hpp"

#include <iostream>
#include <fstream>

NcursesModule::NcursesModule() : _window(nullptr), _isOpen(false),
                                 _gameWidth(800), _gameHeight(600) {}

NcursesModule::~NcursesModule() {
    closeWindow();
}

int NcursesModule::getInput() {
    if (!_isOpen)
        return 0;

    int ch = getch();

    if (ch == ERR)
        return 0;

    if (ch == CTRL('q') || ch == 17)
        return CTRL('q');

    switch (ch) {
    case KEY_LEFT:
        return K_LEFT;
    case KEY_RIGHT:
        return K_RIGHT;
    case KEY_UP:
        return K_UP;
    case KEY_DOWN:
        return K_DOWN;
    case KEY_BACKSPACE:
        return K_BACKSPACE;
    case 127:
        return K_BACKSPACE;
    case 10:
        return '\n';
    default:
        if (ch >= 'a' && ch <= 'z')
            return ch;
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 'a';
    }
    return 0;
}

void NcursesModule::openWindow() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(50);
    getmaxyx(stdscr, _termHeight, _termWidth);
    _window = stdscr;
    _isOpen = true;
    clear();
    refresh();
}

void NcursesModule::closeWindow() {
    if (_isOpen) {
        endwin();
        _isOpen = false;
    }
}

std::pair<int, int> NcursesModule::scaleCoords(int x, int y) {
    double percentX = static_cast<double>(x) / _gameWidth;
    double percentY = static_cast<double>(y) / _gameHeight;

    int termX = static_cast<int>(percentX * _termWidth) + 2;
    int termY = static_cast<int>(percentY * _termHeight) + 2;

    return {termX, termY};
}

void NcursesModule::display(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    if (!_isOpen)
        return;
    erase();
    int objectsDisplayed = 0;
    for (auto& [key, object] : objects) {
        if (object) {
            std::pair<int, int> pos = object->getPosition();
            auto [termX, termY] = scaleCoords(pos.first, pos.second);
            if (object->getType() == SPRITE) {
                char displayChar = '#';
                auto spriteValue = object->getSprite();
                if (spriteValue.has_value()) {
                    auto spritePtr = any_cast<std::shared_ptr<char>>(spriteValue);
                    if (spritePtr) {
                        displayChar = *spritePtr;
                    }
                }
                mvaddch(termY, termX, displayChar);
                objectsDisplayed++;
            }
            else if (object->getType() == TEXT) {
                auto props = std::get<IObject::TextProperties>(object->getProperties());
                std::string displayText = props.text;
                mvprintw(termY, termX, "%s", displayText.c_str());
                objectsDisplayed++;
            }
        }
    }
    refresh();
}

void NcursesModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& [key, object] : objects) {
        if (object->getType() == SPRITE) {
            char spriteChar = '#';
            std::string filePath = object->getTexturePath() + "/text.txt";
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::string line;
                if (std::getline(file, line) && !line.empty()) {
                    spriteChar = line[0];
                }
                file.close();
            }
            auto spritePtr = std::make_shared<char>(spriteChar);
            object->setTexture(std::make_shared<std::string>(object->getTexturePath()));
            object->setSprite(spritePtr);
        }
        else if (object->getType() == TEXT) {
            auto textStr = std::make_shared<std::string>("");
            object->setTexture(std::make_shared<std::string>(object->getTexturePath()));
            object->setSprite(textStr);
            auto props = std::get<IObject::TextProperties>(object->getProperties());

            if (props.text.empty() && key != "selector") {
                props.text = key;
                props.characterSize = 12;
                props.color = WHITE;
                object->setProperties(props);
            }
        }
    }
}