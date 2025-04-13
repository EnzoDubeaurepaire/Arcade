/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Minesweeper
*/

#ifndef MINESWEEPER_HPP_
#define MINESWEEPER_HPP_

#include "IGameModule.hpp"
#include "IObject.hpp"
#include "Sprite.hpp"
#include "Text.hpp"
#include "KeyCodes.hpp"

#include <random>
#include <chrono>
#include <array>

namespace Arcade {

const int GRID_WIDTH = 16;
const int GRID_HEIGHT = 16;
const int NUM_MINES = 40;
const int TILE_SIZE = 32;
const int BOARD_OFFSET_X = 100;
const int BOARD_OFFSET_Y = 150;
const int FACE_SIZE = TILE_SIZE * 2;
const int DIGIT_WIDTH = TILE_SIZE / 2;
const int DIGIT_HEIGHT = TILE_SIZE;
const int DIGIT_SPACING = 5;
const int MAX_TIME = 999;
const int ASSET_TILE_SIZE = 64;

namespace Sprites {

    const std::pair<int, int> TILE_CLOSED = {0, 0};
    const std::pair<int, int> TILE_FLAGGED = {1, 0};
    const std::pair<int, int> TILE_FLAGGED_WRONG = {2, 0};
    const std::pair<int, int> TILE_MINE_EXPLODED = {3, 0};
    const std::pair<int, int> TILE_MINE_REVEALED = {4, 0};

    const std::array<std::pair<int, int>, 9> TILE_OPEN = {{
        {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1},
        {5, 1}, {6, 1}, {7, 1}, {8, 1}
    }};

    const std::pair<int, int> FACE_DEFAULT = {0, 2};
    const std::pair<int, int> FACE_WIN = {1, 2};
    const std::pair<int, int> FACE_LOSE = {2, 2};

    const std::array<std::pair<int, int>, 10> DIGITS = {{
        {0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3},
        {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}
    }};
}

class Minesweeper final : public IGameModule {
private:
    enum class CellState { HIDDEN, REVEALED, FLAGGED };
    enum class GameState { PLAYING, WON, LOST };

    struct Cell {
        CellState state = CellState::HIDDEN;
        bool hasMine = false;
        int neighborMines = 0;
    };

    std::vector<std::vector<Cell>> _board;
    GameState _gameState = GameState::PLAYING;
    int _flagsPlaced = 0;
    int _cellsRevealed = 0;
    bool _firstClick = true;
    size_t _score = 0;
    std::chrono::steady_clock::time_point _startTime;
    int _elapsedTime = 0;
    std::map<std::string, std::unique_ptr<IObject>> _objects;
    std::mt19937 _randomEngine;

    void initGame();
    void initBoard();
    void placeMines(int firstClickX, int firstClickY);
    void calculateNeighborMines();

    void createBoardObjects();
    void createUIObjects();

    void handleMouseClick(std::pair<int, int> mousePos, int inputType);
    void revealCell(int x, int y);
    void revealNeighbors(int x, int y);
    void toggleFlag(int x, int y);
    void checkWinCondition();
    void gameOver(bool won);

    void updateTimer();
    void updateScoreDisplay();
    void updateCounterDisplay(const std::string& prefix, int value);
    void updateFaceDisplay();
    void updateBoardDisplay();
    void updateUIObjects();

    bool isValid(int x, int y) const;
    std::string getCellKey(int x, int y) const;
    std::string getDigitKey(const std::string& prefix, int index) const;
    std::pair<int, int> getSpriteOffsetForCell(const Cell& cell, bool revealAll = false) const;

    void setSpriteObject(
        const std::string& key,
        std::pair<int, int> pos,
        std::pair<int, int> offset,
        std::pair<int, int> size,
        const std::string& assetPath = "Minesweeper/Minesweeper"
    );

    void updateSpriteOffset(
        const std::string& key,
        const std::pair<int, int>& offset
    );

    void setTextObject(
        const std::string& key,
        std::pair<int, int> pos,
        const std::string& text,
        int size = 16,
        uint32_t color = WHITE,
        const std::string& fontPath = "Minesweeper/font"
    );

public:
    Minesweeper();
    ~Minesweeper() override = default;

    bool update(std::pair<int, int> mousePos, int input) override;
    std::map<std::string, std::unique_ptr<IObject>>& getObjects() override { return _objects; }
    std::string getName() const override { return MINESWEEPER; }
    std::size_t getScore() const override { return _score; }
};

}

#endif
