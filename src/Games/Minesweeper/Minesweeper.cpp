/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Minesweeper
*/

#include "Minesweeper.hpp"

namespace Arcade {

Minesweeper::Minesweeper() : _randomEngine(std::random_device{}()) {
    initGame();
}

void Minesweeper::initGame() {
    _board.assign(GRID_HEIGHT, std::vector<Cell>(GRID_WIDTH));
    _gameState = GameState::PLAYING;
    _flagsPlaced = 0;
    _cellsRevealed = 0;
    _firstClick = true;
    _score = 0;
    _elapsedTime = MAX_TIME;
    _startTime = std::chrono::steady_clock::now();
    _objects.clear();

    initBoard();
    createBoardObjects();
    createUIObjects();
    updateBoardDisplay();
    updateUIObjects();
}

void Minesweeper::initBoard() {
    for (int y = 0; y < GRID_HEIGHT; ++y)
        for (int x = 0; x < GRID_WIDTH; ++x)
            _board[y][x] = Cell{};
}

void Minesweeper::placeMines(int firstClickX, int firstClickY) {
    int minesToPlace = NUM_MINES;
    std::uniform_int_distribution<int> distX(0, GRID_WIDTH - 1);
    std::uniform_int_distribution<int> distY(0, GRID_HEIGHT - 1);

    while (minesToPlace > 0) {
        int x = distX(_randomEngine);
        int y = distY(_randomEngine);

        bool isNearFirstClick = false;
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (x == firstClickX + dx && y == firstClickY + dy) {
                    isNearFirstClick = true;
                    break;
                }
            }
            if (isNearFirstClick)
                break;
        }

        if (!isNearFirstClick && !_board[y][x].hasMine) {
            _board[y][x].hasMine = true;
            minesToPlace--;
        }
    }
    calculateNeighborMines();
}

void Minesweeper::calculateNeighborMines() {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            if (_board[y][x].hasMine)
                continue;

            int count = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0)
                        continue;

                    int nx = x + dx;
                    int ny = y + dy;
                    if (isValid(nx, ny) && _board[ny][nx].hasMine)
                        count++;
                }
            }
            _board[y][x].neighborMines = count;
        }
    }
}

void Minesweeper::createBoardObjects() {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            std::string key = getCellKey(x, y);
            auto offset = Sprites::TILE_CLOSED;

            setSpriteObject(
                key,
                {BOARD_OFFSET_X + x * TILE_SIZE, BOARD_OFFSET_Y + y * TILE_SIZE},
                {offset.first * ASSET_TILE_SIZE, offset.second * ASSET_TILE_SIZE},
                {ASSET_TILE_SIZE, ASSET_TILE_SIZE}
            );

            auto props = std::get<IObject::SpriteProperties>(_objects[key]->getProperties());
            props.scale = {(float)TILE_SIZE / ASSET_TILE_SIZE, (float)TILE_SIZE / ASSET_TILE_SIZE};
            _objects[key]->setProperties(props);
        }
    }
}

void Minesweeper::createUIObjects() {
    int facePosX = BOARD_OFFSET_X + (GRID_WIDTH * TILE_SIZE) / 2 - FACE_SIZE / 2;
    int facePosY = BOARD_OFFSET_Y - FACE_SIZE - 10;
    auto faceOffset = Sprites::FACE_DEFAULT;

    setSpriteObject(
        "face",
        {facePosX, facePosY},
        {faceOffset.first * ASSET_TILE_SIZE, faceOffset.second * ASSET_TILE_SIZE},
        {ASSET_TILE_SIZE, ASSET_TILE_SIZE}
    );

    auto faceProps = std::get<IObject::SpriteProperties>(_objects["face"]->getProperties());
    faceProps.scale = {(float)FACE_SIZE / ASSET_TILE_SIZE, (float)FACE_SIZE / ASSET_TILE_SIZE};
    _objects["face"]->setProperties(faceProps);

    int timerPosX = BOARD_OFFSET_X + GRID_WIDTH * TILE_SIZE - 3 * DIGIT_WIDTH - 2 * DIGIT_SPACING - 10;
    int flagPosX = BOARD_OFFSET_X + 10;
    int digitPosY = facePosY + (FACE_SIZE - DIGIT_HEIGHT) / 2;
    auto digitOffset = Sprites::DIGITS[0];

    for (int i = 0; i < 3; ++i) {
        setSpriteObject(
            getDigitKey("timer", i),
            {timerPosX + i * (DIGIT_WIDTH + DIGIT_SPACING), digitPosY},
            {digitOffset.first * ASSET_TILE_SIZE, digitOffset.second * ASSET_TILE_SIZE},
            {ASSET_TILE_SIZE, ASSET_TILE_SIZE}
        );

        auto props = std::get<IObject::SpriteProperties>(_objects[getDigitKey("timer", i)]->getProperties());
        props.scale = {(float)DIGIT_WIDTH / ASSET_TILE_SIZE, (float)DIGIT_HEIGHT / ASSET_TILE_SIZE};
        _objects[getDigitKey("timer", i)]->setProperties(props);
    }

    for (int i = 0; i < 3; ++i) {
        setSpriteObject(
            getDigitKey("flag", i),
            {flagPosX + i * (DIGIT_WIDTH + DIGIT_SPACING), digitPosY},
            {digitOffset.first * ASSET_TILE_SIZE, digitOffset.second * ASSET_TILE_SIZE},
            {ASSET_TILE_SIZE, ASSET_TILE_SIZE}
        );

        auto props = std::get<IObject::SpriteProperties>(_objects[getDigitKey("flag", i)]->getProperties());
        props.scale = {(float)DIGIT_WIDTH / ASSET_TILE_SIZE, (float)DIGIT_HEIGHT / ASSET_TILE_SIZE};
        _objects[getDigitKey("flag", i)]->setProperties(props);
    }

    setTextObject("score_text", {BOARD_OFFSET_X, BOARD_OFFSET_Y + GRID_HEIGHT * TILE_SIZE + 10}, "Score: 0", 20);
}

void Minesweeper::handleMouseClick(std::pair<int, int> mousePos, int inputType) {
    int facePosX = BOARD_OFFSET_X + (GRID_WIDTH * TILE_SIZE) / 2 - FACE_SIZE / 2;
    int facePosY = BOARD_OFFSET_Y - FACE_SIZE - 10;

    if (inputType == K_LCLICK &&
        mousePos.first >= facePosX && mousePos.first < facePosX + FACE_SIZE &&
        mousePos.second >= facePosY && mousePos.second < facePosY + FACE_SIZE) {
        initGame();
        return;
    }

    if (_gameState != GameState::PLAYING)
        return;

    int gridX = (mousePos.first - BOARD_OFFSET_X) / TILE_SIZE;
    int gridY = (mousePos.second - BOARD_OFFSET_Y) / TILE_SIZE;

    if (isValid(gridX, gridY)) {
        if (inputType == K_LCLICK) {
            if (_firstClick) {
                placeMines(gridX, gridY);
                _firstClick = false;
                _startTime = std::chrono::steady_clock::now();
            }
            revealCell(gridX, gridY);
        } else if (inputType == K_RCLICK) {
            toggleFlag(gridX, gridY);
        }
    }
}

void Minesweeper::revealCell(int x, int y) {
    if (!isValid(x, y) || _board[y][x].state == CellState::REVEALED ||
        _board[y][x].state == CellState::FLAGGED)
        return;

    if (_board[y][x].hasMine) {
        _board[y][x].state = CellState::REVEALED;
        gameOver(false);
        return;
    }

    _board[y][x].state = CellState::REVEALED;
    _cellsRevealed++;
    _score++;

    if (_board[y][x].neighborMines == 0)
        revealNeighbors(x, y);

    checkWinCondition();
}

void Minesweeper::revealNeighbors(int x, int y) {
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0)
                continue;
            revealCell(x + dx, y + dy);
        }
    }
}

void Minesweeper::toggleFlag(int x, int y) {
    if (!isValid(x, y) || _board[y][x].state == CellState::REVEALED)
        return;

    if (_board[y][x].state == CellState::HIDDEN) {
        _board[y][x].state = CellState::FLAGGED;
        _flagsPlaced++;
    } else if (_board[y][x].state == CellState::FLAGGED) {
        _board[y][x].state = CellState::HIDDEN;
        _flagsPlaced--;
    }
}

void Minesweeper::checkWinCondition() {
    if (_gameState != GameState::PLAYING)
        return;

    int totalCells = GRID_WIDTH * GRID_HEIGHT;
    if (_cellsRevealed == totalCells - NUM_MINES)
        gameOver(true);
}

void Minesweeper::gameOver(bool won) {
    if (_gameState != GameState::PLAYING)
        return;
    _gameState = won ? GameState::WON : GameState::LOST;

    if (_score > _highScore)
        _highScore = _score;

    if (won) {
        int timeBonus = std::max(0, MAX_TIME - _elapsedTime);
        _score += timeBonus;

        for (int y = 0; y < GRID_HEIGHT; ++y)
            for (int x = 0; x < GRID_WIDTH; ++x)
                if (_board[y][x].hasMine && _board[y][x].state != CellState::FLAGGED)
                    _board[y][x].state = CellState::FLAGGED;
    }

    updateBoardDisplay();
    updateUIObjects();
}

bool Minesweeper::update(std::pair<int, int> mousePos, int input) {
    if (input == 'r') {
        initGame();
        return false;
    }

    if (_gameState == GameState::PLAYING)
        updateTimer();

    if (input == K_LCLICK || input == K_RCLICK)
        handleMouseClick(mousePos, input);

    updateBoardDisplay();
    updateUIObjects();

    return false;
}

void Minesweeper::updateTimer() {
    if (_gameState != GameState::PLAYING || _firstClick)
        return;

    auto now = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();
    _elapsedTime = MAX_TIME - elapsed;

    if (_elapsedTime <= 0) {
        _elapsedTime = 0;
        gameOver(false);
    }
}

void Minesweeper::updateScoreDisplay() {
    if (_objects.count("score_text")) {
        auto props = std::get<IObject::TextProperties>(_objects["score_text"]->getProperties());
        props.text = "Score: " + std::to_string(_score);
        _objects["score_text"]->setProperties(props);
    }
}

void Minesweeper::updateCounterDisplay(const std::string& prefix, int value) {

    value = std::max(0, value);

    int digits[3] = {
        (value / 100) % 10,
        (value / 10) % 10,
        value % 10
    };

    for (int i = 0; i < 3; ++i) {
        std::string key = getDigitKey(prefix, i);
        if (_objects.count(key)) {
            auto offset = Sprites::DIGITS[digits[i]];
            updateSpriteOffset(key, offset);
        }
    }
}

void Minesweeper::updateFaceDisplay() {
    if (!_objects.count("face"))
        return;

    std::pair<int, int> offset;
    switch (_gameState) {
        case GameState::PLAYING:
            offset = Sprites::FACE_DEFAULT;
            break;
        case GameState::WON:
            offset = Sprites::FACE_WIN;
            break;
        case GameState::LOST:
            offset = Sprites::FACE_LOSE;
            break;
    }

    updateSpriteOffset("face", offset);
}

void Minesweeper::updateBoardDisplay() {
    bool revealAll = (_gameState == GameState::LOST || _gameState == GameState::WON);

    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            std::string key = getCellKey(x, y);

            if (_objects.count(key)) {
                auto offset = getSpriteOffsetForCell(_board[y][x], revealAll);
                updateSpriteOffset(key, offset);
            }
        }
    }
}

void Minesweeper::updateUIObjects() {
    updateCounterDisplay("flag", NUM_MINES - _flagsPlaced);
    updateFaceDisplay();
    updateScoreDisplay();
    updateCounterDisplay("timer", _elapsedTime);
}

void Minesweeper::updateSpriteOffset(const std::string& key, const std::pair<int, int>& offset) {
    if (!_objects.count(key))
        return;

    auto props = std::get<IObject::SpriteProperties>(_objects[key]->getProperties());
    props.offset = {offset.first * ASSET_TILE_SIZE, offset.second * ASSET_TILE_SIZE};
    props.textOffset = {props.offset.first / ASSET_TILE_SIZE, props.offset.second / ASSET_TILE_SIZE};
    _objects[key]->setProperties(props);
}

bool Minesweeper::isValid(int x, int y) const {
    return x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT;
}

std::string Minesweeper::getCellKey(int x, int y) const {
    return "cell_" + std::to_string(x) + "_" + std::to_string(y);
}

std::string Minesweeper::getDigitKey(const std::string& prefix, int index) const {
    return prefix + "_" + std::to_string(index);
}

std::pair<int, int> Minesweeper::getSpriteOffsetForCell(const Cell& cell, bool revealAll) const {
    if (revealAll) {
        if (cell.state == CellState::FLAGGED) {
            return cell.hasMine ? Sprites::TILE_FLAGGED : Sprites::TILE_FLAGGED_WRONG;
        }

        if (cell.hasMine) {
            return (_gameState == GameState::LOST && cell.state == CellState::REVEALED) ?
                  Sprites::TILE_MINE_EXPLODED : Sprites::TILE_MINE_REVEALED;
        }
    } else {
        if (cell.state == CellState::FLAGGED) return Sprites::TILE_FLAGGED;
        if (cell.state == CellState::HIDDEN) return Sprites::TILE_CLOSED;
    }

    int neighbors = std::min(8, std::max(0, cell.neighborMines));
    return Sprites::TILE_OPEN[neighbors];
}

void Minesweeper::setSpriteObject(
    const std::string& key,
    std::pair<int, int> pos,
    std::pair<int, int> offset,
    std::pair<int, int> size,
    const std::string& assetPath
) {
    _objects[key] = std::make_unique<Sprite>(assetPath);
    _objects[key]->setPosition(pos);

    auto props = std::get<IObject::SpriteProperties>(_objects[key]->getProperties());
    props.offset = offset;
    props.size = size;
    props.scale = {1.0f, 1.0f};
    props.textColor = WHITE;
    props.textOffset = {offset.first / ASSET_TILE_SIZE, offset.second / ASSET_TILE_SIZE};
    props.textSize = {1, 1};

    _objects[key]->setProperties(props);
}

void Minesweeper::setTextObject(
    const std::string& key,
    std::pair<int, int> pos,
    const std::string& text,
    int size,
    uint32_t color,
    const std::string& fontPath
) {
    _objects[key] = std::make_unique<Text>(fontPath);
    _objects[key]->setPosition(pos);

    auto props = std::get<IObject::TextProperties>(_objects[key]->getProperties());
    props.text = text;
    props.characterSize = size;
    props.color = color;

    _objects[key]->setProperties(props);
}

}

extern "C" {
    std::unique_ptr<Arcade::IGameModule> createInstanceIGame() {
        return std::make_unique<Arcade::Minesweeper>();
    }
}
