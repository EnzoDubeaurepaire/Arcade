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
#include "Text.hpp"


Arcade::Snake::Snake() : _gen(_rd()), _distrib(1, BOARD_SIZE - 1) {
    this->_snakeSize = 1;
    this->initSnake();
    this->initWalls();
    this->initApples();
    this->startClock();
    this->initScore();
}

void Arcade::Snake::initScore() {
    this->_objects["score"] = std::make_unique<Text>("Snake/font");
    this->_objects["score"]->setPosition({ASSET_SIZE * (BOARD_SIZE + 3), 32});
    auto properties = std::get<IObject::TextProperties>(this->_objects["score"]->getProperties());
    properties.color = WHITE;
    properties.text = "Score: " + std::to_string(_snakeSize);
    properties.characterSize = 64;
    this->_objects["score"]->setProperties(properties);
}

void Arcade::Snake::updateScore() {
    auto properties = std::get<IObject::TextProperties>(this->_objects["score"]->getProperties());
    properties.text = "Score: " + std::to_string(_snakeSize);
    this->_objects["score"]->setProperties(properties);
}

void Arcade::Snake::moveSnake(int input) {
    if (input != 0)
        this->_lastInput = input;
    if (this->getElapsedTime() < 0.2)
        return;
    const auto oldDirection = this->getSnakeOrientation();
    this->startClock();
    for (int i = this->_snakeSize; i > 1; i--) {
        const auto snakePos = this->_objects["snake" + std::to_string(i - 1)]->getPosition();
        this->_objects["snake" + std::to_string(i)]->setPosition(snakePos);
    }
    auto snakePos = this->_objects["snake" + std::to_string(1)]->getPosition();
    Orientation direction;
    switch (this->_lastInput) {
        case K_UP:
            direction = UP;
            break;
        case K_DOWN:
            direction = DOWN;
            break;
        case K_LEFT:
            direction = LEFT;
            break;
        case K_RIGHT:
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

void Arcade::Snake::updateSnake() {
    for (size_t i = 1; i <= this->_snakeSize; i++) {
        auto properties = std::get<IObject::SpriteProperties>(this->_objects["snake" + std::to_string(i)]->getProperties());
        properties.offset = this->getSnakePartType(i);
        properties.textOffset = properties.offset;
        properties.textOffset.first /= 4;
        properties.textOffset.second /= 4;
        this->_objects["snake" + std::to_string(i)]->setProperties(properties);
    }
}

bool Arcade::Snake::collideWall() {
    std::vector<std::pair<int, int>> walls;

    for (auto& object : this->_objects)
        if (object.first.starts_with("wall") || (object.first.starts_with("snake") && object.first != "snake1"))
            walls.emplace_back(object.second->getPosition());
    std::pair<int, int> snakeHeadPos = this->_objects["snake1"]->getPosition();
    auto it = std::find(walls.begin(), walls.end(), snakeHeadPos);
    if (it == walls.end())
        return false;
    return true;
}

bool Arcade::Snake::checkApples() {
    for (auto& object : this->_objects)
        if (object.first.starts_with("apple"))
            if (object.second->getPosition() == this->_objects["snake1"]->getPosition()) {
                this->createApple(object.first.back() - '0');
                if (this->addSnakePart())
                    return true;
            }
    return false;
}

bool Arcade::Snake::addSnakePart() {
    std::pair<int, int> tailPos = this->_objects["snake" + std::to_string(this->_snakeSize)]->getPosition();

    std::vector<std::pair<int, int>> possiblePos;
    std::vector valid = {true, true, true, true};
    possiblePos.emplace_back(tailPos.first - ASSET_SIZE, tailPos.second);
    possiblePos.emplace_back(tailPos.first + ASSET_SIZE, tailPos.second);
    possiblePos.emplace_back(tailPos.first, tailPos.second - ASSET_SIZE);
    possiblePos.emplace_back(tailPos.first, tailPos.second + ASSET_SIZE);
    for (auto& object : this->_objects) {
        for (int i = 0; i < 4; i++)
            if (object.second->getPosition() == possiblePos[i])
                valid[i] = false;
    }
    int n = 0;
    for (; !valid[n] && n < 4; n++) {}
    if (n == 4)
        return true;
    const std::pair<int, int> newPos = possiblePos[n];
    const std::string newName = "snake" + std::to_string(this->_snakeSize + 1);
    this->_objects[newName] = std::make_unique<Sprite>("Snake/Snake");
    this->_objects[newName]->setPosition(newPos);
    auto properties = std::get<IObject::SpriteProperties>(this->_objects[newName]->getProperties());
    properties.offset = HEAD_RIGHT;
    properties.size = {ASSET_SIZE, ASSET_SIZE};
    properties.textColor = COLOR(255, 0, 255, 0);
    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
    properties.textOffset = HEAD_RIGHT;
    properties.textOffset.first /= 4;
    properties.textOffset.second /= 4;
    this->_objects[newName]->setProperties(properties);
    this->_snakeSize++;
    return false;
}

bool Arcade::Snake::update(std::pair<int, int> mousePos, int input) {
    (void)mousePos;
    if (input == 'r') {
        this->_objects.clear();
        this->_score = _snakeSize;
        this->_snakeSize = 1;
        this->initSnake();
        this->initWalls();
        this->initApples();
        this->startClock();
        this->initScore();
        return false;
    }
    this->moveSnake(input);
    if (this->collideWall()) {
        this->_objects.clear();
        this->_score = _snakeSize;
        this->_snakeSize = 1;
        this->initSnake();
        this->initWalls();
        this->initApples();
        this->startClock();
        this->initScore();
        return true;
    }
    if (this->checkApples())
        return true;
    this->updateSnake();
    this->updateScore();
    return false;
}

void Arcade::Snake::initSnake() {
    this->_objects["snake1"] = std::make_unique<Sprite>("Snake/Snake");
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

    this->addSnakePart();
    this->addSnakePart();
    this->addSnakePart();
//    this->_objects["snake2"] = std::make_unique<Sprite>("Snake/Snake");
//    this->_objects["snake2"]->setPosition({ASSET_SIZE * (BOARD_SIZE / 2 + 1), ASSET_SIZE * (BOARD_SIZE / 2 + 1)});
//    properties = std::get<IObject::SpriteProperties>(this->_objects["snake2"]->getProperties());
//    properties.offset = BODY_HORIZONTAL;
//    properties.size = {ASSET_SIZE, ASSET_SIZE};
//    properties.textColor = COLOR(255, 0, 255, 0);
//    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
//    properties.textOffset = BODY_HORIZONTAL;
//    properties.textOffset.first /= 4;
//    properties.textOffset.second /= 4;
//    this->_objects["snake2"]->setProperties(properties);

//    this->_objects["snake3"] = std::make_unique<Sprite>("Snake/Snake");
//    this->_objects["snake3"]->setPosition({ASSET_SIZE * (BOARD_SIZE / 2), ASSET_SIZE * (BOARD_SIZE / 2 + 1)});
//    properties = std::get<IObject::SpriteProperties>(this->_objects["snake3"]->getProperties());
//    properties.offset = TAIL_RIGHT;
//    properties.size = {ASSET_SIZE, ASSET_SIZE};
//    properties.textColor = COLOR(255, 0, 255, 0);
//    properties.textSize = {ASSET_SIZE / 4, ASSET_SIZE / 4};
//    properties.textOffset = TAIL_RIGHT;
//    properties.textOffset.first /= 4;
//    properties.textOffset.second /= 4;
//    this->_objects["snake3"]->setProperties(properties);
}

void Arcade::Snake::createWall(std::pair<int, int> pos, int nb) {
    std::string name = "wall" + std::to_string(nb);
    this->_objects[name] = std::make_unique<Sprite>("Snake/Snake");
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

void Arcade::Snake::initWalls() {
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

void Arcade::Snake::createApple(int appleNb) {
    std::vector<std::pair<int, int>> physicalObjects;
    std::string name = "apple" + std::to_string(appleNb);

    for (auto& object : this->_objects)
        if (object.first.starts_with("snake") || object.first.starts_with("apple"))
            physicalObjects.emplace_back(object.second->getPosition());
    bool foundValidPlacement = false;
    while (!foundValidPlacement) {
        std::pair<int, int> pos = {this->_distrib(this->_gen) * ASSET_SIZE, this->_distrib(this->_gen) * ASSET_SIZE};
        auto it = std::find(physicalObjects.begin(), physicalObjects.end(), pos);
        if (it != physicalObjects.end())
            continue;
        foundValidPlacement = true;
        this->_objects[name] = std::make_unique<Sprite>("Snake/Snake");
        this->_objects[name]->setPosition(pos);
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

void Arcade::Snake::initApples() {
    this->createApple(1);
    this->createApple(2);
}

void Arcade::Snake::startClock() {
    this->clock_start = std::chrono::steady_clock::now();
}

double Arcade::Snake::getElapsedTime() const {
    const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - this->clock_start;
    return elapsed.count();
}

Arcade::Snake::Orientation Arcade::Snake::getSnakeOrientation() {
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

std::pair<int, int> Arcade::Snake::getSnakePartType(int snakePart) {
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
    std::unique_ptr<Arcade::IGameModule> createInstanceIGame() {
        return std::make_unique<Arcade::Snake>();
    }
}
