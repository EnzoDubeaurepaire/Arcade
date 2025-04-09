#include "NcursesModule.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

short NcursesModule::getNearestColor(u_int32_t color) {
    unsigned char r = GET_RED(color);
    unsigned char g = GET_GREEN(color);
    unsigned char b = GET_BLUE(color);

    if (r > 200 && g > 200 && b > 200) return COLOR_WHITE;
    if (r < 50 && g < 50 && b < 50) return COLOR_BLACK;
    if (r > 200 && g < 100 && b < 100) return COLOR_RED;
    if (r < 100 && g > 200 && b < 100) return COLOR_GREEN;
    if (r < 100 && g < 100 && b > 200) return COLOR_BLUE;
    if (r > 200 && g > 200 && b < 100) return COLOR_YELLOW;
    if (r > 200 && g < 100 && b > 200) return COLOR_MAGENTA;
    if (r < 100 && g > 200 && b > 200) return COLOR_CYAN;
    return COLOR_WHITE;
}

NcursesModule::NcursesModule() : _isOpen(false),
                                 _gameWidth(800), _gameHeight(600) {}

NcursesModule::~NcursesModule() {
    closeWindow();
}

int NcursesModule::getInput() {
    if (!_isOpen)
        return 0;

    int ch = _ncurses.getKeyInput();

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
    _ncurses.initialize();
    _ncurses.getTerminalSize(_termWidth, _termHeight);
    _isOpen = true;

    if (_ncurses.hasColors()) {
        _ncurses.initColor(1, COLOR_WHITE, COLOR_BLACK);
        _ncurses.initColor(2, COLOR_RED, COLOR_BLACK);
        _ncurses.initColor(3, COLOR_GREEN, COLOR_BLACK);
        _ncurses.initColor(4, COLOR_BLUE, COLOR_BLACK);
        _ncurses.initColor(5, COLOR_YELLOW, COLOR_BLACK);
        _ncurses.initColor(6, COLOR_MAGENTA, COLOR_BLACK);
        _ncurses.initColor(7, COLOR_CYAN, COLOR_BLACK);
    }
}

void NcursesModule::closeWindow() {
    if (_isOpen) {
        _ncurses.cleanup();
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

    _ncurses.clear();

    for (auto& [key, object] : objects) {
        if (object) {
            std::pair<int, int> pos = object->getPosition();
            auto [termX, termY] = scaleCoords(pos.first, pos.second);

            short colorPair = 1;

            if (object->getType() == SPRITE) {
                char displayChar = '#';
                auto spriteValue = object->getSprite();
                if (spriteValue.has_value()) {
                    auto spritePtr = any_cast<std::shared_ptr<char>>(spriteValue);
                    if (spritePtr) {
                        displayChar = *spritePtr;
                    }
                }

                IObject::SpriteProperties props = std::get<IObject::SpriteProperties>(object->getProperties());
                if (_ncurses.hasColors()) {
                    short colorNum = getNearestColor(props.textColor);
                    switch (colorNum) {
                    case COLOR_RED: colorPair = 2; break;
                    case COLOR_GREEN: colorPair = 3; break;
                    case COLOR_BLUE: colorPair = 4; break;
                    case COLOR_YELLOW: colorPair = 5; break;
                    case COLOR_MAGENTA: colorPair = 6; break;
                    case COLOR_CYAN: colorPair = 7; break;
                    default: colorPair = 1; break;
                    }
                }

                _ncurses.drawChar(termY, termX, displayChar, colorPair);
            }
            else if (object->getType() == TEXT) {
                IObject::TextProperties props = std::get<IObject::TextProperties>(object->getProperties());
                std::string displayText = props.text;

                if (_ncurses.hasColors()) {
                    short colorNum = getNearestColor(props.color);
                    switch (colorNum) {
                    case COLOR_RED: colorPair = 2; break;
                    case COLOR_GREEN: colorPair = 3; break;
                    case COLOR_BLUE: colorPair = 4; break;
                    case COLOR_YELLOW: colorPair = 5; break;
                    case COLOR_MAGENTA: colorPair = 6; break;
                    case COLOR_CYAN: colorPair = 7; break;
                    default: colorPair = 1; break;
                    }
                }

                _ncurses.drawString(termY, termX, displayText, colorPair);
            }
        }
    }

    _ncurses.refresh();
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