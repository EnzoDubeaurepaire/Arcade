/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#include "MainMenu.hpp"

#include <iostream>
#include <SFML/Graphics/Color.hpp>


MainMenu::MainMenu(const std::shared_ptr<std::string>& loadedGame, const std::shared_ptr<std::string>& loadedDisplay) {
    this->_loadedGame = loadedGame;
    this->_loadedDisplay = loadedDisplay;

    this->_objects["selector"] = std::make_unique<Sprite>("./ressources/MainMenu");
    this->_objects["selector"]->setPosition({0, 0});
    auto spriteProperties = std::get<IObject::SpriteProperties>(this->_objects["selector"]->getProperties());
    spriteProperties.offset = {0, 0};
    spriteProperties.size = {50, 50};
    this->_objects["selector"]->setProperties(spriteProperties);

    this->addTextObject("playerName", {300, 300}, USERNAME_SIZE, "", WHITE);
    this->addTextObject("title", {150, 50}, TITLE_SIZE, "Arcacade", WHITE);
}

bool MainMenu::update(std::pair<int, int> pos, int input) {
    (void)pos;
//    this->updateDisplayText();
//    this->updateGamesText();
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
    auto textProperties = std::get<IObject::TextProperties>(this->_objects["playerName"]->getProperties());
    textProperties.text = this->_playerName;
    this->_objects["playerName"]->setProperties(textProperties);
    return true;
}

void MainMenu::removeCharFromPlayer() {
    if (this->_playerName.empty())
        return;
    this->_playerName.pop_back();
}

void MainMenu::addCharToPlayer(const char c) {
    if (this->_playerName.size() >= USERNAME_MAX_LENGTH)
        return;
    this->_playerName.push_back(c);
}

void MainMenu::updateDisplay(const std::vector<std::string> &display) {
    this->_display = display;
}

void MainMenu::updateGames(const std::vector<std::string> &games) {
    this->_games = games;
}

void MainMenu::addTextObject(const std::string& name, std::pair<int, int> pos, int size, const std::string& text, const u_int32_t color) {
    this->_objects[name] = std::make_unique<Text>("./ressources/MainMenu");
    this->_objects[name]->setPosition({pos.first, pos.second});
    auto properties = std::get<IObject::TextProperties>(this->_objects[name]->getProperties());
    properties.text = text;
    properties.color = color;
    properties.characterSize = size;
    this->_objects[name]->setProperties(properties);
}
