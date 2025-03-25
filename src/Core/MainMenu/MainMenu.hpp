/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_
#include "IGameModule.hpp"
#include "Sprite.hpp"
#include "KeyCodes.hpp"
#include "Text.hpp"


class MainMenu final : public IGameModule {
public:
    MainMenu();
    ~MainMenu() override = default;

    bool update(std::pair<int, int>pos, int input) override;
    std::map<std::string, std::unique_ptr<IObject>>& getObjects() override {return _objects;};
    std::string getName() const override {return "Main Menu";};
    std::size_t getScore() const override {return 0;};

private:
    std::map<std::string, std::unique_ptr<IObject>> _objects;
    std::string _playerName;

    void removeCharFromPlayer();
    void addCharToPlayer(char c);
};

#endif
