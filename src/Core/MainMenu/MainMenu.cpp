/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#include "MainMenu.hpp"

MainMenu::MainMenu() {

    _objects["selector"] = std::make_unique<Sprite>("./ressources/MainMenu");
    _objects["selector"]->setPosition({0, 0});
    _objects["selector"]->setSize({50, 50});
    _objects["selector"]->setOffset({0, 0});
}
