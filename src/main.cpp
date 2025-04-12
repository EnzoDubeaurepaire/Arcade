/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** main
*/

#include "Core.hpp"

int main(int ac, char **av) {
    if (ac != 2 || std::string(av[1]) == "--help") {
        std::cout << "Usage: ./arcade <display_module.so>\n\n"
                  << "Description:\n"
                  << "  A modular arcade game engine that allows switching between games and graphic libraries at runtime.\n"
                  << "  Provide a display module (.so file) as an argument to start.\n\n"
                  << "Controls:\n"
                  << "  Global:\n"
                  << "    Ctrl + Q  : Quit the program\n"
                  << "    Ctrl + D  : Change display module\n"
                  << "    Ctrl + G  : Change game\n"
                  << "    Escape    : Return to main menu\n"
                  << "    R         : Restart current game\n\n"
                  << "  Main Menu:\n"
                  << "    Arrow Keys: Move selection cursor\n"
                  << "    Space     : Select a game or display module\n"
                  << "    Enter     : Launch the selected game\n\n"
                  << "  Snake Game:\n"
                  << "    Arrow Keys: Control the snake's movement\n\n"
                  << "Example:\n"
                  << "  ./arcade ./lib/sfml_display.so\n";
        return EXIT_FAILURE;
    }
    try {
        Arcade::Core core(av[1]);
        core.run();
    } catch (const Arcade::Core::CoreException& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
