#include "MainMenu.hpp"

#include <iostream>

Arcade::MainMenu::MainMenu(const std::shared_ptr<std::string>& loadedGame, const std::shared_ptr<std::string>& loadedDisplay) {
    this->_loadedGame = loadedGame;
    this->_loadedDisplay = loadedDisplay;
    this->_selectedGame = 0;
    this->_selectorPos = {0, 0};
    this->_scoreboardActive = false;
    this->_currentGameScoreboard = "";

    this->_objects["selector"] = std::make_unique<Sprite>("MainMenu/MainMenu");
    this->_objects["selector"]->setPosition({150, 300});
    auto spriteProperties = std::get<IObject::SpriteProperties>(this->_objects["selector"]->getProperties());
    spriteProperties.offset = {0, 0};
    spriteProperties.textOffset = {0, 0};
    spriteProperties.size = {50, 50};
    spriteProperties.textSize = {1, 1};
    this->_objects["selector"]->setProperties(spriteProperties);

    this->addTextObject("playerName", {200, 500}, USERNAME_SIZE, "", WHITE);
    this->addTextObject("title", {150, 50}, TITLE_SIZE, "Arcacade", WHITE);
    this->loadScores();
}

void Arcade::MainMenu::updateDisplayText() {
    for (std::size_t i = 0; i < this->_display.size(); i++) {
        if (!this->_objects.contains(this->_display[i])) {
            this->addTextObject(this->_display[i], {400, 300 + 30 * static_cast<int>(i)}, 16, this->_display[i], WHITE);
        }
    }
}

void Arcade::MainMenu::updateGamesText() {
    for (std::size_t i = 0; i < this->_games.size(); i++) {
        if (!this->_objects.contains(this->_games[i]) && this->_games[i] != "Main Menu") {
            this->addTextObject(this->_games[i], {200, 300 + 30 * static_cast<int>(i)}, 16, this->_games[i], WHITE);
        }
    }
}

void Arcade::MainMenu::loadScores() {
    std::ifstream file(".save");
    if (!file.is_open())
        return;

    _scoreEntries.clear();
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string game, player;
        int score;

        if (iss >> game >> player >> score) {
            _scoreEntries.push_back({game, player, score});
        }
    }
}

void Arcade::MainMenu::clearScoreboard() {
    std::vector<std::string> keys;
    for (const auto& [key, _] : _objects) {
        if (key.find("scoreboard_") == 0) {
            keys.push_back(key);
        }
    }
    for (const auto& key : keys) {
        _objects.erase(key);
    }
    _scoreboardActive = false;
    _currentGameScoreboard = "";
}

void Arcade::MainMenu::updateScoreboard() {
    clearScoreboard();

    if (_games.empty() || _selectedGame < 0 || _selectedGame >= static_cast<int>(_games.size()))
        return;
    std::string selectedGame = _games[_selectedGame];
    _currentGameScoreboard = selectedGame;
    int verticalLineX = 1400;
    for (int i = 0; i < 300; i++) {
        addScoreboardTextObject("separator_" + std::to_string(i), {verticalLineX, 10 + i * 10}, 20, "|", WHITE);
    }
    std::vector<ScoreEntry> filteredScores;
    for (const auto& entry : _scoreEntries) {
        if (entry.game == selectedGame) {
            filteredScores.push_back(entry);
        }
    }
    std::sort(filteredScores.begin(), filteredScores.end(),
        [](const ScoreEntry& a, const ScoreEntry& b) { return a.score > b.score; });
    int scoreboardX = 1480;
    addScoreboardTextObject("title", {scoreboardX, 100}, 24, "High Scores", WHITE);
    size_t maxToShow = std::min(static_cast<size_t>(0), filteredScores.size());
    for (size_t i = 0; i < maxToShow; i++) {
        int baseYPos = 160 + static_cast<int>(i) * 80;
        addScoreboardTextObject(
            "score_player_" + std::to_string(i),
            {scoreboardX, baseYPos},
            18,
            "name: " + filteredScores[i].player,
            COLOR(255, 255, 0, 0)
        );
        addScoreboardTextObject(
            "score_value_" + std::to_string(i),
            {scoreboardX, baseYPos + 30},
            18,
            "score: " + std::to_string(filteredScores[i].score),
            COLOR(255, 0, 0, 255)
        );
    }
    if (filteredScores.empty()) {
        addScoreboardTextObject("empty", {scoreboardX, 160}, 18, "No scores yet", WHITE);
    }
    _scoreboardActive = true;
}

bool Arcade::MainMenu::update(std::pair<int, int> pos, int input) {
    (void)pos;

    std::vector<std::string> toRemove;
    for (const auto& [key, _] : this->_objects) {
        if (std::find(this->_display.begin(), this->_display.end(), key) == this->_display.end() &&
            std::find(this->_games.begin(), this->_games.end(), key) == this->_games.end() &&
            key != "selector" && key != "playerName" && key != "title" &&
            key.find("scoreboard_") != 0) {
            toRemove.push_back(key);
        }
    }

    for (const auto& key : toRemove) {
        this->_objects.erase(key);
    }
    this->updateDisplayText();
    this->updateGamesText();

    switch (input) {
    case 0:
        break;
    case K_LEFT:
        this->moveSelectorLeft();
        break;
    case K_RIGHT:
        this->moveSelectorRight();
        break;
    case K_UP:
        this->moveSelectorUp();
        break;
    case K_DOWN:
        this->moveSelectorDown();
        break;
    case K_BACKSPACE:
        this->removeCharFromPlayer();
        break;
    case ' ':
        this->select();
        break;
    case '\n':
        this->launch();
        break;
    default:
        if (input >= 'a' && input <= 'z')
            this->addCharToPlayer(static_cast<char>(input));
        break;
    }

    if (this->_games.empty())
        this->moveSelectorRight();

    auto textProperties = std::get<IObject::TextProperties>(this->_objects["playerName"]->getProperties());
    textProperties.text = "Enter player name: " + this->_playerName;
    this->_objects["playerName"]->setProperties(textProperties);

    return false;
}

void Arcade::MainMenu::removeCharFromPlayer() {
    if (this->_playerName.empty())
        return;
    this->_playerName.pop_back();
}

void Arcade::MainMenu::addCharToPlayer(const char c) {
    if (this->_playerName.size() >= USERNAME_MAX_LENGTH)
        return;
    this->_playerName.push_back(c);
}

void Arcade::MainMenu::updateDisplay(const std::vector<std::string> &display) {
    this->_display = display;
}

void Arcade::MainMenu::updateGames(const std::vector<std::string> &games) {
    bool gamesChanged = false;
    if (this->_games.size() != games.size()) {
        gamesChanged = true;
    } else {
        for (size_t i = 0; i < games.size(); i++) {
            if (i >= this->_games.size() || this->_games[i] != games[i]) {
                gamesChanged = true;
                break;
            }
        }
    }

    this->_games = games;
    if (gamesChanged && _scoreboardActive) {
        clearScoreboard();
    }
}

void Arcade::MainMenu::addTextObject(const std::string& name, std::pair<int, int> pos, int size, const std::string& text, const uint32_t color) {
    this->_objects[name] = std::make_unique<Text>("MainMenu/font");
    this->_objects[name]->setPosition({pos.first, pos.second});
    auto properties = std::get<IObject::TextProperties>(this->_objects[name]->getProperties());
    properties.text = text;
    properties.color = color;
    properties.characterSize = size;
    this->_objects[name]->setProperties(properties);
}

void Arcade::MainMenu::addScoreboardTextObject(const std::string& name, std::pair<int, int> pos, int size, const std::string& text, const uint32_t color) {
    std::string fullName = "scoreboard_" + name;
    this->_objects[fullName] = std::make_unique<Text>("MainMenu/font");
    this->_objects[fullName]->setPosition({pos.first, pos.second});
    auto properties = std::get<IObject::TextProperties>(this->_objects[fullName]->getProperties());
    properties.text = text;
    properties.color = color;
    properties.characterSize = size;
    this->_objects[fullName]->setProperties(properties);
}

void Arcade::MainMenu::moveSelectorDown() {
    auto [x, y] = _objects["selector"]->getPosition();
    if (this->_selectorPos.first == 0) {
        if (this->_selectorPos.second < static_cast<int>(this->_games.size()) - 1) {
            _objects["selector"]->setPosition({x, y + 30});
            this->_selectorPos.second++;
        }
    } else {
        if (this->_selectorPos.second < static_cast<int>(this->_display.size()) - 1) {
            _objects["selector"]->setPosition({x, y + 30});
            this->_selectorPos.second++;
        }
    }
}

void Arcade::MainMenu::moveSelectorUp() {
    auto [x, y] = _objects["selector"]->getPosition();
    if (this->_selectorPos.second > 0) {
        _objects["selector"]->setPosition({x, y - 30});
        this->_selectorPos.second--;
    }
}

void Arcade::MainMenu::moveSelectorLeft() {
    if (this->_selectorPos.first == 1) {
        auto [x, y] = _objects["selector"]->getPosition();
        _objects["selector"]->setPosition({x - 200, y});
        this->_selectorPos.first = 0;
        if (this->_selectorPos.second > static_cast<int>(this->_games.size()) - 1) {
            _objects["selector"]->setPosition({x - 200, y - (200 * (this->_selectorPos.second - this->_games.size()))});
            this->_selectorPos.second = this->_games.size() - 1;
        }
    }
}

void Arcade::MainMenu::moveSelectorRight() {
    if (this->_selectorPos.first == 0) {
        auto [x, y] = _objects["selector"]->getPosition();
        _objects["selector"]->setPosition({x + 200, y});
        this->_selectorPos.first = 1;
        if (this->_selectorPos.second > static_cast<int>(this->_display.size()) - 1) {
            _objects["selector"]->setPosition({x + 200, y - (200 * (this->_selectorPos.second - this->_display.size()))});
            this->_selectorPos.second = this->_display.size() - 1;
        }
    }
}

void Arcade::MainMenu::select() {
    if (this->_selectorPos.first == 0 && !this->_games.empty() &&
        this->_selectorPos.second >= 0 && this->_selectorPos.second < static_cast<int>(this->_games.size())) {

        this->_selectedGame = this->_selectorPos.second;
        for (const auto& gameName : this->_games) {
            if (this->_objects.count(gameName)) {
                auto properties = std::get<IObject::TextProperties>(this->_objects[gameName]->getProperties());
                properties.color = WHITE;
                this->_objects[gameName]->setProperties(properties);
            }
        }

        auto properties = std::get<IObject::TextProperties>(this->_objects[this->_games[this->_selectorPos.second]]->getProperties());
        properties.color = COLOR(255, 172, 172, 172);
        this->_objects[this->_games[this->_selectorPos.second]]->setProperties(properties);
        loadScores();
        updateScoreboard();
    }

    if (this->_selectorPos.first == 1 && !this->_display.empty() &&
        this->_selectorPos.second >= 0 && this->_selectorPos.second < static_cast<int>(this->_display.size())) {

        *this->_loadedDisplay = this->_display[this->_selectorPos.second];
        for (const auto& displayName : this->_display) {
            if (this->_objects.count(displayName)) {
                auto props = std::get<IObject::TextProperties>(this->_objects[displayName]->getProperties());
                props.color = WHITE;
                this->_objects[displayName]->setProperties(props);
            }
        }

        auto selectedProps = std::get<IObject::TextProperties>(this->_objects[this->_display[this->_selectorPos.second]]->getProperties());
        selectedProps.color = COLOR(255, 172, 172, 172);
        this->_objects[this->_display[this->_selectorPos.second]]->setProperties(selectedProps);
    }
}

void Arcade::MainMenu::launch() const {
    if (this->_playerName.empty())
        return;
    if (this->_games.empty())
        return;
    *this->_loadedGame = this->_games[this->_selectedGame];
}