/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Text
*/

#include "Text.hpp"

#include <iostream>

Arcade::Text::Text(const std::string& path) {
    TextProperties textProperties;

    textProperties.color = 0;
    textProperties.text = "";
    textProperties.characterSize = 0;
    this->_properties = textProperties;
    _path = path;
}
