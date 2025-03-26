/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IGameModule
*/

#ifndef IGAMEMODULE_H
#define IGAMEMODULE_H

// std includes
#include <map>
#include <memory>
#include <string>
#include <utility>

// project includes
#include "IObject.hpp"
#include "IDisplayModule.hpp"

// Game names
#define PACMAN "PACMAN"
#define SNAKE "SNAKE"
#define MINESWEEPER "MINESWEEPER"
#define NIBBLER "NIBBLER"
#define QIX "QIX"
#define CENTIPEDE "CENTIPEDE"
#define SOLARFOX "SOLARFOX"

// Definition of the IGameModule interface
class IGameModule
{
public:
    // Default virtual destructor
    virtual ~IGameModule() = default;

    // Update the objects according to the inputs
    virtual bool update(std::pair<int, int> mousePos, int input) = 0;

    // Returns a reference to the objects' list
    virtual std::map<std::string, std::unique_ptr<IObject>>& getObjects() = 0;
    // Returns the name of the game
    virtual std::string getName() const = 0;

    // Returns the score of the player
    virtual std::size_t getScore() const = 0;
};

#endif
