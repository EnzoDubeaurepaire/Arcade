/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** MainMenu
*/

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_
#include "IGameModule.hpp"

class MainMenu final : public IGameModule {
public:
    MainMenu() = default;
    ~MainMenu() override = default;

    bool update(std::pair<int, int>, IDisplayModule::click state, int input) override;
    void addObject(std::string name) override;
    void deleteObject(std::string name) override;
    std::map<std::string, std::unique_ptr<IObject>> getObjects() const override;
    std::string getName() const override {return "Main Menu";};

private:
    std::map<std::string, std::unique_ptr<IObject>> _objects;
};

#endif
