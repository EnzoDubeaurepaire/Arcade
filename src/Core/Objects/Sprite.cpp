/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Sprite
*/

#include "Sprite.hpp"

Arcade::Sprite::Sprite(const std::string &path) {
    SpriteProperties spriteProperties;

    spriteProperties.scale = {1.f, 1.f};
    spriteProperties.textColor = WHITE;
    this->_properties = spriteProperties;
    this->_path = path;
}
