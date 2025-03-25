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
            auto sprite(any_cast<sf::Sprite>(object.second->getSprite()));
            auto texture(any_cast<sf::Texture>(object.second->getTexture()));
            sprite.setTexture(texture);
            sprite.setPosition(static_cast<float>(object.second->getPosition().first), static_cast<float>(object.second->getPosition().second));
            const sf::IntRect rect(object.second->getOffset().first, object.second->getOffset().second, object.second->getSize().first, object.second->getSize().second);
            sprite.setTextureRect(rect);
            this->_window->draw(sprite);
        }
        if (object.second->getType() == TEXT) {
            auto text(any_cast<sf::Text>(object.second->getSprite()));
            auto font(std::any_cast<sf::Font>(object.second->getTexture()));
            text.setPosition(static_cast<float>(object.second->getPosition().first), static_cast<float>(object.second->getPosition().second));
            text.setFont(font);
            text.setString(object.second->getText());
            text.setFillColor(sf::Color::White);
            text.setCharacterSize(object.second->getSize().first);
            this->_window->draw(text);
        }
    }
    this->_window->display();
}

void SfmlModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& object : objects) {
        if (object.second->getType() == SPRITE) {
            sf::Texture texture;
            texture.loadFromFile(object.second->getTexturePath() + "/graphical.png");
            sf::Sprite sprite;
            sprite.setTexture(texture);
            object.second->setTexture(texture);
            object.second->setSprite(sprite);
        }
        if (object.second->getType() == TEXT) {
            sf::Font font;
            font.loadFromFile(object.second->getTexturePath() + "/font.ttf");
            sf::Text text;
            text.setFont(font);
            object.second->setTexture(font);
            object.second->setSprite(text);
        }
    }
}

