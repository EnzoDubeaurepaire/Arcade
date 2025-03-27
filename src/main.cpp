/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** main
*/

#include "Core.hpp"

int main(int ac, char **av) {
    if (ac != 2) {
        std::cerr << "Usage: " << av[0] << " <file>" << std::endl;
        return EXIT_FAILURE;
    }
    try {
        Core core(av[1]);
        core.run();
    } catch (const Core::CoreException& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
