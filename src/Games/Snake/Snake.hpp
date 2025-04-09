/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Snake
*/

#ifndef SNAKE_H
#define SNAKE_H

#include <random>
#include "IGameModule.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>

#define ASSET_SIZE 64
#define BOARD_SIZE 15
#define BODY_HORIZONTAL {ASSET_SIZE, ASSET_SIZE * 3}
#define BODY_VERTICAL {0, ASSET_SIZE * 3}
#define APPLE {ASSET_SIZE * 2, ASSET_SIZE * 3}
#define WALL {ASSET_SIZE * 3, ASSET_SIZE * 3}
#define HEAD_UP {0, ASSET_SIZE}
#define HEAD_RIGHT {ASSET_SIZE, ASSET_SIZE}
#define HEAD_DOWN {ASSET_SIZE * 2, ASSET_SIZE}
#define HEAD_LEFT {ASSET_SIZE * 3, ASSET_SIZE}
#define TAIL_RIGHT {0, ASSET_SIZE * 2}
#define TAIL_LEFT {ASSET_SIZE, ASSET_SIZE * 2}
#define TAIL_DOWN {ASSET_SIZE * 2, ASSET_SIZE * 2}
#define TAIL_UP {ASSET_SIZE * 3, ASSET_SIZE * 2}
#define CORNER_UP_LEFT {0, 0}
#define CORNER_DOWN_LEFT {ASSET_SIZE, 0}
#define CORNER_UP_RIGHT {ASSET_SIZE * 2, 0}
#define CORNER_DOWN_RIGHT {ASSET_SIZE * 3, 0}

class Snake final : public IGameModule {
public:
    Snake();
    ~Snake() override = default;

    bool update(std::pair<int, int> mousePos, int input) override;

    std::map<std::string, std::unique_ptr<IObject>>& getObjects() override {return this->_objects;};

    std::size_t getScore() const override {return this->_snakeSize;};

    std::string getName() const override {return SNAKE;};

private:
    enum Orientation {
        LEFT,
        RIGHT,
        DOWN,
        UP
    };
    std::map<std::string, std::unique_ptr<IObject>> _objects;
    size_t _snakeSize;

    std::random_device _rd;
    std::mt19937 _gen;
    std::uniform_int_distribution<> _distrib;

    void initSnake();
    void initWalls();
    void initApples();

    void createWall(std::pair<int, int>, int);
    void createApple(int appleNb);

    void startClock();
    double getElapsedTime() const;

    Orientation getSnakeOrientation();
    std::pair<int, int> getSnakePartType(int snakePart);
    std::chrono::steady_clock::time_point clock_start;

    void moveSnake(int input);
    void updateSnake();

    bool collideWall();
    void checkApples();

    void addSnakePart();
};

#endif