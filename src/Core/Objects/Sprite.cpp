/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Sprite
*/

#include "Sprite.hpp"

Sprite::Sprite(const std::string &path) {
    SpriteProperties spriteProperties;

    this->_properties = spriteProperties;
    this->_path = path;
}
