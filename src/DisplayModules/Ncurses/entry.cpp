/*
** EPITECH PROJECT, 2023
** arcade
** File description:
** entry ncurse mainmenu
*/

#include "NcursesModule.hpp"

extern "C" {
std::unique_ptr<IDisplayModule> createInstanceIDisplay() {
    return std::make_unique<NcursesModule>();
}
}