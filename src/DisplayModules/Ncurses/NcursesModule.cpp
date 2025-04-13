/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcursesModule
*/

#include "NcursesModule.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

short Arcade::NcursesModule::getNearestColor(u_int32_t color) {
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

Arcade::NcursesModule::NcursesModule() : _isOpen(false),
                                         _gameWidth(1920), _gameHeight(1080) {}

Arcade::NcursesModule::~NcursesModule() = default;

int Arcade::NcursesModule::getInput() {
    if (!_isOpen)
        return 0;

    int ch = _ncurses.getKeyInput();

    if (ch == KEY_MOUSE) {
        MEVENT event;
        if (getmouse(&event) == OK) {
            _lastMousePos = {event.x + 1, event.y + 1};
            if (event.bstate & (BUTTON1_PRESSED | BUTTON1_CLICKED))
                return K_LCLICK;
            if (event.bstate & (BUTTON3_PRESSED | BUTTON3_CLICKED))
                return K_RCLICK;
            if (event.bstate & (BUTTON2_PRESSED | BUTTON2_CLICKED))
                return K_MCLICK;
        }
        return 0;
    }

    if (ch == ERR)
        return 0;

    if (ch == CTRL('Q'))
        return CTRL('q');
    if (ch == CTRL('D'))
        return CTRL('d');
    if (ch == CTRL('G'))
        return CTRL('g');

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
    case ' ':
        return ' ';
    case K_ESC:
        return K_ESC;
    case 'r':
        return 'r';
    default:
        if (ch >= 'a' && ch <= 'z')
            return ch;
        if (ch >= 'A' && ch <= 'Z')
            return ch - 'A' + 'a';
    }
    return 0;
}

void Arcade::NcursesModule::openWindow() {
    _ncurses.initialize();
    _ncurses.getTerminalSize(_termWidth, _termHeight);
    _isOpen = true;

    mousemask(ALL_MOUSE_EVENTS, NULL);

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

void Arcade::NcursesModule::closeWindow() {
    if (_isOpen) {
        _ncurses.cleanup();
        _isOpen = false;
    }
}

std::pair<int, int> Arcade::NcursesModule::scaleCoords(int x, int y) {
    double percentX = static_cast<double>(x) / _gameWidth;
    double percentY = static_cast<double>(y) / _gameHeight;

    int termX = static_cast<int>(percentX * _termWidth);
    int termY = static_cast<int>(percentY * _termHeight);

    return {termX, termY};
}

std::pair<int, int> Arcade::NcursesModule::scaleTermToGame(std::pair<int, int> termPos) const {
    double percentX = static_cast<double>(termPos.first) / _termWidth;
    double percentY = static_cast<double>(termPos.second) / _termHeight;

    int gameX = static_cast<int>(percentX * _gameWidth);
    int gameY = static_cast<int>(percentY * _gameHeight);

    return {gameX, gameY};
}

void Arcade::NcursesModule::display(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    if (!_isOpen)
        return;

    _ncurses.getTerminalSize(_termWidth, _termHeight);
    _ncurses.clear();

    for (auto& [key, object] : objects) {
        if (object) {
            std::pair<int, int> pos = object->getPosition();
            auto [termX, termY] = this->scaleCoords(pos.first, pos.second);

            short colorPair = 1;

            if (object->getType() == SPRITE) {
                auto texture = any_cast<std::shared_ptr<std::vector<std::string>>>(object->getTexture());
                std::vector<std::string> sprite;
                const auto props = std::get<IObject::SpriteProperties>(object->getProperties());

                if (static_cast<int>(texture->size()) < props.textOffset.second + props.textSize.second)
                    return;
                sprite.resize(props.textSize.second);
                for (int i = 0; i < props.textSize.second; i++) {
                    sprite[i] = (*texture)[i + props.textOffset.second].substr(props.textOffset.first, props.textSize.first);
                }
                if (_ncurses.hasColors()) {
                    short colorNum = this->getNearestColor(props.textColor);
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
                for (const auto& line : sprite)
                    this->_ncurses.drawString(termY, termX, line, colorPair);
            }
            else if (object->getType() == TEXT) {
                IObject::TextProperties props = std::get<IObject::TextProperties>(object->getProperties());
                std::string displayText = props.text;
                if (_ncurses.hasColors()) {
                    short colorNum = this->getNearestColor(props.color);
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

void Arcade::NcursesModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& [key, object] : objects) {
        if (std::string(object->getSprite().type().name()) == "St10shared_ptrINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEE" || std::string(object->getTexture().type().name()) == "St10shared_ptrISt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS6_EEE")
            continue;
        if (object->getSprite().has_value())
            object->getSprite().reset();
        if (object->getTexture().has_value())
            object->getTexture().reset();
        if (object->getType() == SPRITE) {
            auto texture = std::make_shared<std::vector<std::string>>();
            std::string filePath = "./assets/" + object->getTexturePath() + ".txt";
            std::ifstream file(filePath);
            if (file.is_open()) {
                std::string line;
                while (std::getline(file, line)) {
                    texture->push_back(line);
                }
                file.close();
            } else {
                std::cerr << "Failed to load text file: " << filePath << std::endl;
            }
            object->setTexture(texture);
            this->_spriteList.push_back(texture);
        }
        else if (object->getType() == TEXT) {
            auto textStr = std::make_shared<std::string>("");
            object->setSprite(textStr);
            this->_textList.push_back(textStr);
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

extern "C" {
std::unique_ptr<Arcade::IDisplayModule> createInstanceIDisplay() {
    return std::make_unique<Arcade::NcursesModule>();
}
}
