/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Snake
*/

#include <vector>
#include "Snake.hpp"
#include "Sprite.hpp"
#include <algorithm>

Snake::Snake() : _gen(_rd()), _distrib(0, BOARD_SIZE) {
    this->_snakeSize = 3;
    this->initSnake();
    this->initWalls();
    this->initApples();
}

bool Snake::update(std::pair<int, int> mousePos, int input) {

}

void Snake::initSnake() {
    this->_objects["snake1"] = std::make_unique<Sprite>("./ressources/Snake");
    this->_objects["snake1"]->setPosition({ASSET_SIZE * (BOARD_SIZE / 2 + 2), ASSET_SIZE * (BOARD_SIZE / 2  + 1)});
    auto properties = std::get<IObject::SpriteProperties>(this->_objects["snake1"]->getProperties());
    properties.offset = HEAD_RIGHT;
    properties.size = {ASSET_SIZE, ASSET_SIZE};
    properties.textColor = COLOR(255, 0, 255, 0);
    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
    properties.textOffset = HEAD_RIGHT;
    properties.textOffset.first /= 4;
    properties.textOffset.second /= 4;
    this->_objects["snake1"]->setProperties(properties);

    this->_objects["snake2"] = std::make_unique<Sprite>("./ressources/Snake");
    this->_objects["snake2"]->setPosition({ASSET_SIZE * (BOARD_SIZE / 2 + 1), ASSET_SIZE * (BOARD_SIZE / 2 + 1)});
    properties = std::get<IObject::SpriteProperties>(this->_objects["snake2"]->getProperties());
    properties.offset = BODY_HORIZONTAL;
    properties.size = {ASSET_SIZE, ASSET_SIZE};
    properties.textColor = COLOR(255, 0, 255, 0);
    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
    properties.textOffset = BODY_HORIZONTAL;
    properties.textOffset.first /= 4;
    properties.textOffset.second /= 4;
    this->_objects["snake2"]->setProperties(properties);

    this->_objects["snake3"] = std::make_unique<Sprite>("./ressources/Snake");
    this->_objects["snake3"]->setPosition({ASSET_SIZE * (BOARD_SIZE / 2), ASSET_SIZE * (BOARD_SIZE / 2 + 1)});
    properties = std::get<IObject::SpriteProperties>(this->_objects["snake3"]->getProperties());
    properties.offset = TAIL_RIGHT;
    properties.size = {ASSET_SIZE, ASSET_SIZE};
    properties.textColor = COLOR(255, 0, 255, 0);
    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
    properties.textOffset = TAIL_RIGHT;
    properties.textOffset.first /= 4;
    properties.textOffset.second /= 4;
    this->_objects["snake3"]->setProperties(properties);
}

void Snake::createWall(std::pair<int, int> pos, int nb) {
    std::string name = "wall" + std::to_string(nb);
    this->_objects[name] = std::make_unique<Sprite>("./ressources/Snake");
    this->_objects[name]->setPosition(pos);
    auto properties = std::get<IObject::SpriteProperties>(this->_objects[name]->getProperties());
    properties.offset = WALL;
    properties.size = {ASSET_SIZE, ASSET_SIZE};
    properties.textColor = COLOR(255, 0, 0, 255);
    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
    properties.textOffset = WALL;
    properties.textOffset.first /= 4;
    properties.textOffset.second /= 4;
    this->_objects[name]->setProperties(properties);
}

void Snake::initWalls() {
    int nb = 0;
    for (int i = 0; i < BOARD_SIZE + 2; i++) {
        createWall({ASSET_SIZE * i, 0}, nb++);
        createWall({ASSET_SIZE * i, ASSET_SIZE * BOARD_SIZE}, nb++);
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        createWall({0, ASSET_SIZE * (i + 1)}, nb++);
        createWall({ASSET_SIZE * BOARD_SIZE, ASSET_SIZE * (i + 1)}, nb++);
    }
}

void Snake::createApple(int appleNb) {
    std::vector<std::pair<int, int>> physicalObjects;
    std::string name = "apple" + std::to_string(appleNb);

    for (auto& object : this->_objects)
        if (object.first.starts_with("snake") || object.first.starts_with("apple"))
            physicalObjects.emplace_back((object.second->getPosition().first / ASSET_SIZE) - 1, (object.second->getPosition().second / ASSET_SIZE) - 1);
    bool foundValidPlacement = false;
    while (!foundValidPlacement) {
        std::pair<int, int> pos = {this->_distrib(this->_gen), this->_distrib(this->_gen)};
        auto it = std::find(physicalObjects.begin(), physicalObjects.end(), pos);
        if (it == physicalObjects.end())
            continue;
        foundValidPlacement = true;
        this->_objects[name] = std::make_unique<Sprite>("./ressources/Snake");
        this->_objects[name]->setPosition({pos.first * (ASSET_SIZE + 1), pos.second * (ASSET_SIZE + 1)});
        auto properties = std::get<IObject::SpriteProperties>(this->_objects[name]->getProperties());
        properties.offset = APPLE;
        properties.size = {ASSET_SIZE, ASSET_SIZE};
        properties.textColor = COLOR(255, 255, 0, 0);
        properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
        properties.textOffset = APPLE;
        properties.textOffset.first /= 4;
        properties.textOffset.second /= 4;
        this->_objects[name]->setProperties(properties);
    }
}

void Snake::initApples() {
    this->createApple(1);
    this->createApple(2);
}
