/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SfmlModule
*/

#include "SfmlModule.hpp"

#include <iostream>

int SfmlModule::getInput() {
    sf::Event event{};
    while (this->_window->pollEvent(event)) {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.control && event.key.code == sf::Keyboard::Q))
            return CTRL('q');

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left)
                return KEY_LEFT;
            if (event.key.code == sf::Keyboard::Right)
                return KEY_RIGHT;
            if (event.key.code == sf::Keyboard::Up)
                return KEY_UP;
            if (event.key.code == sf::Keyboard::Down)
                return KEY_DOWN;
            if (event.key.code >= sf::Keyboard::A && event.key.code <= sf::Keyboard::Z)
                return event.key.code + 'a';
            if (event.key.code == sf::Keyboard::Backspace)
                return KEY_BACKSPACE;
            if (event.key.code == sf::Keyboard::Enter)
                return '\n';
            if (event.key.code == sf::Keyboard::Space)
                return ' ';
        }
    }
    return 0;
}

void SfmlModule::openWindow() {
    this->_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "SFML Window");
}

void SfmlModule::closeWindow() {
    if (this->_window->isOpen())
        this->_window->close();
}

void SfmlModule::display(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    this->_window->clear();
    for (auto& object : objects) {
        if (object.second->getType() == SPRITE) {
            auto sprite(any_cast<std::shared_ptr<sf::Sprite>>(object.second->getSprite()));
            auto texture(any_cast<std::shared_ptr<sf::Texture>>(object.second->getTexture()));
            sprite->setTexture(*texture);
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
            auto font(any_cast<std::shared_ptr<sf::Font>>(object.second->getTexture()));
            text->setPosition(static_cast<float>(object.second->getPosition().first), static_cast<float>(object.second->getPosition().second));
            text->setFont(*font);
            text->setString(std::get<IObject::TextProperties>(object.second->getProperties()).text);
            u_int32_t color = std::get<IObject::TextProperties>(object.second->getProperties()).color;
            text->setFillColor(sf::Color(GET_RED(color), GET_GREEN(color), GET_BLUE(color), GET_ALPHA(color)));
            text->setCharacterSize(std::get<IObject::TextProperties>(object.second->getProperties()).characterSize);
            this->_window->draw(*text);
        }
    }
    this->_window->display();
}

void SfmlModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& object : objects) {
        if (object.second->getType() == SPRITE) {
            auto texture = std::make_shared<sf::Texture>();
            texture->loadFromFile(object.second->getTexturePath() + "/graphical.png");
            auto sprite = std::make_shared<sf::Sprite>();
            sprite->setTexture(*texture);
            object.second->setTexture(texture);
            object.second->setSprite(sprite);
        }
        if (object.second->getType() == TEXT) {
            auto font = std::make_shared<sf::Font>();
            font->loadFromFile(object.second->getTexturePath() + "/font.ttf");
            auto text = std::make_shared<sf::Text>();
            text->setFont(*font);
            object.second->setTexture(font);
            object.second->setSprite(text);
        }
    }
}

