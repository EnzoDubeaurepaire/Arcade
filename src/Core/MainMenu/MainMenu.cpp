/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#include "MainMenu.hpp"

#include <iostream>


MainMenu::MainMenu(const std::shared_ptr<std::string>& loadedGame, const std::shared_ptr<std::string>& loadedDisplay) {
    this->_loadedGame = loadedGame;
    this->_loadedDisplay = loadedDisplay;

    _objects["selector"] = std::make_unique<Sprite>("./ressources/MainMenu");
    _objects["selector"]->setPosition({0, 0});
    _objects["selector"]->setSize({50, 50});
    _objects["selector"]->setOffset({0, 0});

    _objects["playerName"] = std::make_unique<Text>("./ressources/MainMenu");
    _objects["playerName"]->setPosition({300, 200});
    _objects["playerName"]->setSize({24, 0});
}

bool MainMenu::update(std::pair<int, int> pos, int input) {
    (void)pos;
    auto [x, y] = _objects["selector"]->getPosition();
    switch (input) {
        case 0:
            break;
        case KEY_LEFT:
            _objects["selector"]->setPosition({x - 50, y});
            break;
        case KEY_RIGHT:
            _objects["selector"]->setPosition({x + 50, y});
            break;
        case KEY_UP:
            _objects["selector"]->setPosition({x, y - 50});
            break;
        case KEY_DOWN:
            _objects["selector"]->setPosition({x, y + 50});
            break;
        case KEY_BACKSPACE:
            this->removeCharFromPlayer();
            break;
        default:
            if (input >= 'a' && input <= 'z')
                this->addCharToPlayer(static_cast<char>(input));
            break;
    }
    _objects["playerName"]->setText(this->_playerName);
    return true;
}

void MainMenu::removeCharFromPlayer() {
    if (this->_playerName.empty())
        return;
    this->_playerName.pop_back();
}

void MainMenu::addCharToPlayer(const char c) {
    if (this->_playerName.size() >= 20)
        return;
    this->_playerName.push_back(c);
}

void MainMenu::updateDisplay(const std::vector<std::string> &display) {
    this->_display = display;
}

void MainMenu::updateGames(const std::vector<std::string> &games) {
    this->_games = games;
}

