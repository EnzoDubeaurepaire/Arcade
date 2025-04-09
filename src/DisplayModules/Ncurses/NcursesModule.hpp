/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcursesModule
*/

#ifndef NCURSESMODULE_H_
#define NCURSESMODULE_H_

#include <ncurses.h>
#include <map>
#include <memory>
#include <string>
#include "KeyCodes.hpp"

#include "IDisplayModule.hpp"

class NcursesModule final : public IDisplayModule {
public:
    NcursesModule();
    ~NcursesModule();

    void initObject(std::map<std::string, std::unique_ptr<IObject>>&) override;

    int getInput() override;
    std::pair<int, int> getMousePos() const override {return {0, 0};};

    std::string getName() const override {return "NCURSES";};

    void openWindow() override;
    void closeWindow() override;

    void display(std::map<std::string, std::unique_ptr<IObject>>&) override;

private:
    // Scale game coordinates to terminal coordinates
    std::pair<int, int> scaleCoords(int x, int y);

    WINDOW* _window;
    bool _isOpen;

    // Terminal dimensions
    int _termWidth;
    int _termHeight;

    // Game coordinate system (default 800x600)
    int _gameWidth;
    int _gameHeight;
};

extern "C" {
std::unique_ptr<IDisplayModule> createInstanceIDisplay() {
    return std::make_unique<NcursesModule>();
}
}

#endif