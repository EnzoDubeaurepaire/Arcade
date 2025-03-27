/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_
#include <list>
#include <vector>

#include "IGameModule.hpp"
#include "Sprite.hpp"
#include "KeyCodes.hpp"
#include "Loader.hpp"
#include "Text.hpp"


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
    

private:
    std::map<std::string, std::unique_ptr<IObject>> _objects;
    std::string _playerName;
    std::shared_ptr<std::string> _loadedGame;
    std::shared_ptr<std::string> _loadedDisplay;
    std::vector<std::string> _games;
    std::vector<std::string> _display;

    void removeCharFromPlayer();
    void addCharToPlayer(char c);
};

#endif
