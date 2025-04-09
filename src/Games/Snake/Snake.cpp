/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Snake
*/

#include <vector>
#include "Snake.hpp"

#include "KeyCodes.hpp"
#include "Sprite.hpp"


Snake::Snake() : _gen(_rd()), _distrib(0, BOARD_SIZE) {
    this->_snakeSize = 3;
    this->initSnake();
    this->initWalls();
    this->initApples();
    this->startClock();
}

void Snake::moveSnake(int input) {
    if (this->getElapsedTime() < 0.5)
        return;
    const auto oldDirection = this->getSnakeOrientation();
    this->startClock();
    for (int i = this->_snakeSize; i > 1; i--) {
        const auto snakePos = this->_objects["snake" + std::to_string(i - 1)]->getPosition();
        this->_objects["snake" + std::to_string(i)]->setPosition(snakePos);
    }
    auto snakePos = this->_objects["snake" + std::to_string(1)]->getPosition();
    Orientation direction;
    switch (input) {
        case KEY_UP:
            direction = UP;
            break;
        case KEY_DOWN:
            direction = DOWN;
            break;
        case KEY_LEFT:
            direction = LEFT;
            break;
        case KEY_RIGHT:
            direction = RIGHT;
            break;
        default:
            direction = oldDirection;
            break;
    }
    if ((direction == UP && oldDirection == DOWN) || (direction == DOWN && oldDirection == UP) || (direction == LEFT && oldDirection == RIGHT) || (direction == RIGHT && oldDirection == LEFT))
        direction = oldDirection;
    switch (direction) {
        case UP:
            snakePos.second -= ASSET_SIZE;
            break;
        case DOWN:
            snakePos.second += ASSET_SIZE;
            break;
        case LEFT:
            snakePos.first -= ASSET_SIZE;
            break;
        default:
            snakePos.first += ASSET_SIZE;
            break;
    }
    this->_objects["snake1"]->setPosition(snakePos);
}

void Snake::updateSnake() {
    for (size_t i = 1; i <= this->_snakeSize; i++) {
        auto properties = std::get<IObject::SpriteProperties>(this->_objects["snake" + std::to_string(i)]->getProperties());
        properties.offset = this->getSnakePartType(i);
        properties.textOffset = properties.offset;
        properties.textOffset.first /= 4;
        properties.textOffset.second /= 4;
        this->_objects["snake" + std::to_string(i)]->setProperties(properties);
    }
}

bool Snake::update(std::pair<int, int> mousePos, int input) {
    (void)mousePos;
    this->moveSnake(input);
    this->updateSnake();
    return false;
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
        createWall({ASSET_SIZE * (BOARD_SIZE + 1), ASSET_SIZE * (i + 1)}, nb++);
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
        if (it != physicalObjects.end())
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

void Snake::startClock() {
    this->clock_start = std::chrono::steady_clock::now();
}

double Snake::getElapsedTime() const {
    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - this->clock_start;
    return elapsed.count();
}

Snake::Orientation Snake::getSnakeOrientation() {
    std::string snakeA = "snake1";
    std::string snakeB = "snake2";
    if (this->_objects["snake1"]->getPosition().first == this->_objects["snake2"]->getPosition().first) {
        if (this->_objects["snake1"]->getPosition().second < this->_objects["snake2"]->getPosition().second)
            return UP;
        return DOWN;
    }
    if (this->_objects["snake1"]->getPosition().first < this->_objects["snake2"]->getPosition().first)
        return LEFT;
    return RIGHT;
}

std::pair<int, int> Snake::getSnakePartType(int snakePart) {
    if (snakePart == 1)
        switch (this->getSnakeOrientation()) {
            case UP:
                return HEAD_UP;
            case DOWN:
                return HEAD_DOWN;
            case LEFT:
                return HEAD_LEFT;
            default:
                return HEAD_RIGHT;
        }
    if (snakePart == static_cast<int>(this->_snakeSize)) {
        std::pair<int, int> tail = this->_objects["snake" + std::to_string(snakePart)]->getPosition();
        std::pair<int, int> tailPrev = this->_objects["snake" + std::to_string(snakePart - 1)]->getPosition();
        if (tail.first == tailPrev.first) {
            if (tail.second < tailPrev.second)
                return TAIL_DOWN;
            return TAIL_UP;
        }
        if (tail.first < tailPrev.first)
            return TAIL_RIGHT;
        return TAIL_LEFT;
    }
    std::pair<int, int> partPrev = this->_objects["snake" + std::to_string(snakePart - 1)]->getPosition();
    std::pair<int, int> part = this->_objects["snake" + std::to_string(snakePart)]->getPosition();
    std::pair<int, int> partSucc = this->_objects["snake" + std::to_string(snakePart + 1)]->getPosition();
    Orientation prevDirection;
    Orientation succDirection;
    if (part.first == partPrev.first) {
        if (part.second < partPrev.second)
            prevDirection = DOWN;
        else
            prevDirection = UP;
    } else {
        if (part.first < partPrev.first)
            prevDirection = RIGHT;
        else
            prevDirection = LEFT;
    }
    if (part.first == partSucc.first) {
        if (part.second < partSucc.second)
            succDirection = DOWN;
        else
            succDirection = UP;
    } else {
        if (part.first < partSucc.first)
            succDirection = RIGHT;
        else
            succDirection = LEFT;
    }
    if ((succDirection == LEFT && prevDirection == RIGHT) || (succDirection == RIGHT && prevDirection == LEFT))
        return BODY_HORIZONTAL;
    if ((succDirection == UP && prevDirection == DOWN) || (succDirection == DOWN && prevDirection == UP))
        return BODY_VERTICAL;
    if ((succDirection == UP && prevDirection == LEFT) || (succDirection == LEFT && prevDirection == UP))
        return CORNER_UP_LEFT;
    if ((succDirection == UP && prevDirection == RIGHT) || (succDirection == RIGHT && prevDirection == UP))
        return CORNER_UP_RIGHT;
    if ((succDirection == LEFT && prevDirection == DOWN) || (succDirection == DOWN && prevDirection == LEFT))
        return CORNER_DOWN_LEFT;
    if ((succDirection == RIGHT && prevDirection == DOWN) || (succDirection == DOWN && prevDirection == RIGHT))
        return CORNER_DOWN_RIGHT;
    return BODY_HORIZONTAL;
}


extern "C" {
    std::unique_ptr<IGameModule> createInstanceIGame() {
        return std::make_unique<Snake>();
    }
}
