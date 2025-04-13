/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** NcursesModule
*/

#ifndef NCURSESMODULE_H_
#define NCURSESMODULE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "KeyCodes.hpp"
#include "IDisplayModule.hpp"
#include "NcursesWrapper.hpp"

namespace Arcade {
    class NcursesModule final : public IDisplayModule {
    public:
        NcursesModule();
        ~NcursesModule();

        void initObject(std::map<std::string, std::unique_ptr<IObject>>&) override;
        int getInput() override;
        std::pair<int, int> getMousePos() const override {return scaleTermToGame(_lastMousePos);};
        std::string getName() const override {return "NCURSES";};

        void openWindow() override;
        void closeWindow() override;
        void display(std::map<std::string, std::unique_ptr<IObject>>&) override;

    private:
        std::pair<int, int> scaleCoords(int x, int y);
        std::pair<int, int> scaleTermToGame(std::pair<int, int> termPos) const;
        short getNearestColor(u_int32_t color);

        NcursesWrapper _ncurses;
        bool _isOpen;
        int _termWidth;
        int _termHeight;
        int _gameWidth;
        int _gameHeight;
        std::pair<int, int> _lastMousePos = {0, 0};

        std::vector<std::shared_ptr<std::string>> _textList;
        std::vector<std::shared_ptr<std::vector<std::string>>> _spriteList;
    };
}

#endif
