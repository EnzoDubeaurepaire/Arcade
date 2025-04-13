/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

#define TITLE_SIZE 20
#define USERNAME_SIZE 15
#define USERNAME_MAX_LENGTH 13

#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>

#include "IGameModule.hpp"
#include "Sprite.hpp"
#include "KeyCodes.hpp"
#include "Loader.hpp"
#include "Text.hpp"
#include "IObject.hpp"


namespace Arcade {
    class MainMenu final : public IGameModule {
    public:
        MainMenu(const std::shared_ptr<std::string>& loadedGame, const std::shared_ptr<std::string>& loadedDisplay);
        ~MainMenu() override = default;

        bool update(std::pair<int, int>pos, int input) override;
        std::map<std::string, std::unique_ptr<IObject>>& getObjects() override {return _objects;};
        std::string getName() const override {return "Main Menu";};
        std::size_t getScore() const override {return 0;};

        void updateGames(const std::vector<std::string>& games);
        void updateDisplay(const std::vector<std::string>& display);

        std::string getPlayerName() {return _playerName;};

    private:
        struct ScoreEntry {
            std::string game;
            std::string player;
            int score;
        };
        std::map<std::string, std::unique_ptr<IObject>> _objects;
        std::string _playerName;
        std::shared_ptr<std::string> _loadedGame;
        std::shared_ptr<std::string> _loadedDisplay;
        std::vector<std::string> _games;
        int _selectedGame;
        std::vector<std::string> _display;
        std::pair<std::size_t, std::size_t> _cursorPos;
        std::pair<int, int> _selectorPos;
        std::vector<ScoreEntry> _scoreEntries;
        std::string _currentGameScoreboard;
        bool _scoreboardActive;

        void removeCharFromPlayer();
        void addCharToPlayer(char c);
        void addTextObject(const std::string& name, std::pair<int, int> pos, int size, const std::string& text, u_int32_t color);
        void addScoreboardTextObject(const std::string& name, std::pair<int, int> pos, int size, const std::string& text, u_int32_t color);
        void updateDisplayText();
        void updateGamesText();
        void moveSelectorLeft();
        void moveSelectorRight();
        void moveSelectorUp();
        void moveSelectorDown();
        void select();
        void launch() const;
        void loadScores();
        void updateScoreboard();
        void clearScoreboard();
    };
}

#endif