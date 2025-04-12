/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SfmlModule
*/

#include "SfmlModule.hpp"

#include <iostream>

int Arcade::SfmlModule::getInput() {
    sf::Event event{};
    while (this->_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.control && event.key.code == sf::Keyboard::Q))
            return CTRL('q');

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left)
                return K_LEFT;
            if (event.key.code == sf::Keyboard::Right)
                return K_RIGHT;
            if (event.key.code == sf::Keyboard::Up)
                return K_UP;
            if (event.key.code == sf::Keyboard::Down)
                return K_DOWN;
            if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z) {
                if (event.key.control)
                    return CTRL(event.key.code + 'a');
                return event.key.code + 'a';
            }
            if (event.key.code == sf::Keyboard::Backspace)
                return K_BACKSPACE;
            if (event.key.code == sf::Keyboard::Enter)
                return '\n';
            if (event.key.code == sf::Keyboard::Space)
                return ' ';
        }
    }
    return 0;
}

void Arcade::SfmlModule::openWindow() {
    this->_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(1920, 1080), "SFML Window");
    this->_window->setKeyRepeatEnabled(false);
}

void Arcade::SfmlModule::closeWindow() {
    if (this->_window->isOpen())
        this->_window->close();
}

void Arcade::SfmlModule::display(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    this->_window->clear();
    for (auto& object : objects) {
        if (object.second->getType() == SPRITE) {
            auto sprite(any_cast<std::shared_ptr<sf::Sprite>>(object.second->getSprite()));
            sprite->setPosition(static_cast<float>(object.second->getPosition().first), static_cast<float>(object.second->getPosition().second));
            IObject::Properties spriteProperties = object.second->getProperties();
            const sf::IntRect rect(std::get<IObject::SpriteProperties>(object.second->getProperties()).offset.first,
                std::get<IObject::SpriteProperties>(object.second->getProperties()).offset.second,
                std::get<IObject::SpriteProperties>(object.second->getProperties()).size.first,
                std::get<IObject::SpriteProperties>(object.second->getProperties()).size.second);
            sprite->setTextureRect(rect);
            this->_window->draw(*sprite);
        }
        if (object.second->getType() == TEXT) {
            auto text(any_cast<std::shared_ptr<sf::Text>>(object.second->getSprite()));
            text->setPosition(static_cast<float>(object.second->getPosition().first), static_cast<float>(object.second->getPosition().second));
            text->setString(std::get<IObject::TextProperties>(object.second->getProperties()).text);
            u_int32_t color = std::get<IObject::TextProperties>(object.second->getProperties()).color;
            text->setFillColor(sf::Color(GET_RED(color), GET_GREEN(color), GET_BLUE(color), GET_ALPHA(color)));
            text->setCharacterSize(std::get<IObject::TextProperties>(object.second->getProperties()).characterSize);
            this->_window->draw(*text);
        }
    }
    this->_window->display();
}

void Arcade::SfmlModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& object : objects) {
        if (std::string(object.second->getSprite().type().name()) == "St10shared_ptrIN2sf6SpriteEE" || std::string(object.second->getSprite().type().name()) == "St10shared_ptrIN2sf4TextEE")
            continue;
        if (object.second->getType() == SPRITE) {
            auto texture = std::make_shared<sf::Texture>();
            texture->loadFromFile("./assets/" + object.second->getTexturePath() + ".png");
            auto sprite = std::make_shared<sf::Sprite>();
            sprite->setTexture(*texture);
            object.second->setTexture(texture);
            object.second->setSprite(sprite);
        }
        if (object.second->getType() == TEXT) {
            auto font = std::make_shared<sf::Font>();
            font->loadFromFile("./assets/" + object.second->getTexturePath() + ".ttf");
            auto text = std::make_shared<sf::Text>();
            text->setFont(*font);
            object.second->setTexture(font);
            object.second->setSprite(text);
        }
    }
}

extern "C" {
    std::unique_ptr<Arcade::IDisplayModule> createInstanceIDisplay() {
        return std::make_unique<Arcade::SfmlModule>();
    }
}
