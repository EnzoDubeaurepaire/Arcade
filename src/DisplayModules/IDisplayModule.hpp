/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IDisplayModule
*/

#ifndef IDISPLAYMODULE_H
#define IDISPLAYMODULE_H
#include <memory>
#include "IObject.hpp"
#include <map>


class IDisplayModule
{
public:
    enum click {
        NOTHING = 0,
        RIGHT,
        LEFT
    };
    virtual ~IDisplayModule() = default;

    virtual void initObject(std::map<std::string, std::unique_ptr<IObject>>) = 0;

    virtual int getInput() const = 0;
    virtual std::pair<int, int> getMousePos() const = 0;
    virtual click getMouseState() const = 0;

    virtual void openWindow() = 0;
    virtual void closeWindow() = 0;

    virtual void display(std::map<std::string, std::unique_ptr<IObject>>) = 0;
    virtual std::string getName() const = 0;
};

#endif
