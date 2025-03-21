/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IGameModule
*/

#ifndef IGAMEMODULE_H
#define IGAMEMODULE_H
#include <map>
#include <memory>
#include <string>
#include <utility>
#include "IObject.hpp"

class IGameModule
{
public:
    enum click {
        NOTHING = 0,
        RIGHT,
        LEFT
    };
    virtual ~IGameModule() = default;

    virtual bool update(std::pair<int, int>, click state, int input) = 0;

    virtual void addObject(std::string name) = 0;
    virtual void deleteObject(std::string name) = 0;
    virtual std::map<std::string, std::unique_ptr<IObject>> getObjects() const = 0;
};

#endif
