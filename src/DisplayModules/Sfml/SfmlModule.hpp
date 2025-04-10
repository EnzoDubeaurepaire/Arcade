/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SfmlModule
*/

#ifndef SFMLMODULE_H_
#define SFMLMODULE_H_

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "KeyCodes.hpp"

#include "IDisplayModule.hpp"

namespace Arcade {
    class SfmlModule final : public IDisplayModule {
    public:
        SfmlModule() = default;
        ~SfmlModule() override = default;

        void initObject(std::map<std::string, std::unique_ptr<IObject>>&) override;

        int getInput() override;
        std::pair<int, int> getMousePos() const override {return {0, 0};};

        std::string getName() const override {return "SFML";};

        void openWindow() override;
        void closeWindow() override;

        void display(std::map<std::string, std::unique_ptr<IObject>>&) override;

    private:
        std::unique_ptr<sf::RenderWindow> _window;
    };
}

#endif
