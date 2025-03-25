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
            // Add Inputs here
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
        if (object.second->getType() == "Sprite") {
            auto sprite(any_cast<sf::Sprite>(object.second->getSprite()));
            auto texture(any_cast<sf::Texture>(object.second->getTexture()));
            sprite.setTexture(texture);
            sprite.setPosition(static_cast<float>(object.second->getPosition().first), static_cast<float>(object.second->getPosition().second));
            const sf::IntRect rect(object.second->getOffset().first, object.second->getOffset().second, object.second->getSize().first, object.second->getSize().second);
            sprite.setTextureRect(rect);
            this->_window->draw(sprite);
        }
    }
    this->_window->display();
}

void SfmlModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    for (auto& object : objects) {
        if (object.second->getType() == "Sprite") {
            sf::Texture texture;
            texture.loadFromFile(object.second->getTexturePath() + "/graphical.png");
            sf::Sprite sprite;
            sprite.setTexture(texture);
            object.second->setTexture(texture);
            object.second->setSprite(sprite);
        }
    }
}

