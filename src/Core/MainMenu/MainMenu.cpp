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
    this->_selectedGame = 0;

    this->_objects["selector"] = std::make_unique<Sprite>("./ressources/MainMenu");
    this->_objects["selector"]->setPosition({150, 300});
    auto spriteProperties = std::get<IObject::SpriteProperties>(this->_objects["selector"]->getProperties());
    spriteProperties.offset = {0, 0};
    spriteProperties.size = {50, 50};
    this->_objects["selector"]->setProperties(spriteProperties);

    this->addTextObject("playerName", {200, 500}, USERNAME_SIZE, "", WHITE);
    this->addTextObject("title", {150, 50}, TITLE_SIZE, "Arcacade", WHITE);
}

void MainMenu::updateDisplayText() {
    for (std::size_t i = 0; i < this->_display.size(); i++)
        if (!this->_objects.contains(this->_display[i]))
            this->addTextObject(this->_display[i], {400, 300 + 30 * i}, 16, this->_display[i], WHITE);
}

void MainMenu::updateGamesText() {
    for (std::size_t i = 0; i < this->_games.size(); i++) {
        if (!this->_objects.contains(this->_games[i]) && this->_games[i] != "Main Menu")
            this->addTextObject(this->_games[i], {200, 300 + 30 * i}, 16, this->_games[i], WHITE);
    }
}


bool MainMenu::update(std::pair<int, int> pos, int input) {
    (void)pos;
    std::vector<std::string> toRemove;
    for (const auto& object : this->_objects) {
        if (std::find(this->_display.begin(), this->_display.end(), object.first) == this->_display.end() &&
            std::find(this->_games.begin(), this->_games.end(), object.first) == this->_games.end() &&
            object.first != "selector" && object.first != "playerName" && object.first != "title")
            toRemove.push_back(object.first);
    }
    for (const auto& item : toRemove)
        this->_objects.erase(item);
    this->updateDisplayText();
    this->updateGamesText();
    switch (input) {
        case 0:
            break;
        case KEY_LEFT:
            this->moveSelectorLeft();
            break;
        case KEY_RIGHT:
            this->moveSelectorRight();
            break;
        case KEY_UP:
            this->moveSelectorUp();
            break;
        case KEY_DOWN:
            this->moveSelectorDown();
            break;
        case KEY_BACKSPACE:
            this->removeCharFromPlayer();
            break;
        case ' ':
            this->select();
            break;
        case '\n':
            this->launch();
            break;
        default:
            if (input >= 'a' && input <= 'z')
                this->addCharToPlayer(static_cast<char>(input));
            break;
    }
    if (this->_games.empty())
        this->moveSelectorRight();
    auto textProperties = std::get<IObject::TextProperties>(this->_objects["playerName"]->getProperties());
    textProperties.text = "Enter player name: " + this->_playerName;
    this->_objects["playerName"]->setProperties(textProperties);
    return false;
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

void MainMenu::addTextObject(const std::string& name, std::pair<int, int> pos, int size, const std::string& text, const uint32_t color) {
    this->_objects[name] = std::make_unique<Text>("./ressources/MainMenu");
    this->_objects[name]->setPosition({pos.first, pos.second});
    auto properties = std::get<IObject::TextProperties>(this->_objects[name]->getProperties());
    properties.text = text;
    properties.color = color;
    properties.characterSize = size;
    this->_objects[name]->setProperties(properties);
}

void MainMenu::moveSelectorDown() {
    auto [x, y] = _objects["selector"]->getPosition();
    if (this->_selectorPos.first == 0) {
        if (static_cast<int>(this->_games.size()) < this->_selectorPos.second) {
            _objects["selector"]->setPosition({x, y + 30});
            this->_selectorPos.second++;
        }
    } else {
        if (static_cast<int>(this->_display.size()) < this->_selectorPos.second) {
            _objects["selector"]->setPosition({x, y + 30});
            this->_selectorPos.second++;
        }
    }
}

void MainMenu::moveSelectorUp() {
    auto [x, y] = _objects["selector"]->getPosition();
    if (this->_selectorPos.first == 0) {
        if (this->_selectorPos.second > 0) {
            _objects["selector"]->setPosition({x, y - 30});
            this->_selectorPos.second--;
        }
    } else {
        if (this->_selectorPos.second > 0) {
            _objects["selector"]->setPosition({x, y - 30});
            this->_selectorPos.second--;
        }
    }
}

void MainMenu::moveSelectorLeft() {
    if (this->_selectorPos.first == 1) {
        auto [x, y] = _objects["selector"]->getPosition();
        _objects["selector"]->setPosition({x - 200, y});
        this->_selectorPos.first = 0;
        if (this->_selectorPos.second > static_cast<int>(this->_games.size()) - 1) {
            _objects["selector"]->setPosition({x - 200, y - (200 * (this->_selectorPos.second - this->_games.size()))});
            this->_selectorPos.second = this->_games.size() - 1;
        }
    }
}

void MainMenu::moveSelectorRight() {
    if (this->_selectorPos.first == 0) {
        auto [x, y] = _objects["selector"]->getPosition();
        _objects["selector"]->setPosition({x + 200, y});
        this->_selectorPos.first = 1;
        if (this->_selectorPos.second > static_cast<int>(this->_display.size()) - 1) {
            _objects["selector"]->setPosition({x + 200, y - (200 * (this->_selectorPos.second - this->_display.size()))});
            this->_selectorPos.second = this->_display.size() - 1;
        }
    }
}

void MainMenu::select() {
    if (this->_selectorPos.first == 0) {
        this->_selectedGame = this->_selectorPos.second;
        for (const auto& gameName : this->_games) {
            auto properties = std::get<IObject::TextProperties>(this->_objects[gameName]->getProperties());
            properties.color = WHITE;
            this->_objects[gameName]->setProperties(properties);
        }
        auto properties = std::get<IObject::TextProperties>(this->_objects[this->_games[this->_selectorPos.second]]->getProperties());
        properties.color = COLOR(255, 172, 172, 172);
        this->_objects[this->_games[this->_selectorPos.second]]->setProperties(properties);
    }
    if (this->_selectorPos.first == 1) {
        *this->_loadedDisplay = this->_display[this->_selectorPos.second];
        for (const auto& displayName : this->_display) {
            auto properties = std::get<IObject::TextProperties>(this->_objects[displayName]->getProperties());
            properties.color = WHITE;
            this->_objects[displayName]->setProperties(properties);
        }
        auto properties = std::get<IObject::TextProperties>(this->_objects[this->_display[this->_selectorPos.second]]->getProperties());
        properties.color = COLOR(255, 172, 172, 172);
        this->_objects[this->_display[this->_selectorPos.second]]->setProperties(properties);
    }
}

void MainMenu::launch() const {
    if (this->_playerName.empty())
        return;
    if (this->_games.empty())
        return;
    *this->_loadedGame = this->_games[this->_selectedGame];
}
