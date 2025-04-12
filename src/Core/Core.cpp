/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#include "Core.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>

#include "MainMenu.hpp"

Arcade::Core::Core(const std::string& baseDisplay) {
    this->_loadedGame = std::make_shared<std::string>("Main Menu");
    this->_loadedDisplay = std::make_shared<std::string>("");
    this->_gameModules["Main Menu"] = std::pair(std::make_unique<DynamicLibrary>(), std::make_unique<MainMenu>(this->_loadedGame, this->_loadedDisplay));
    this->loadFirstLib(baseDisplay);
    this->updateLibraries();
}

void Arcade::Core::loadFirstLib(const std::string& name) {
    if (!std::filesystem::exists("./lib") || !std::filesystem::is_directory("./lib")) {
        throw CoreException("Libraries not found");
    }
    try {
        auto lib = std::make_unique<DynamicLibrary>(name);
        void *sym = lib->getSymbol("createInstanceIDisplay");
        if (!sym) {
            throw CoreException("Could not find symbol in '" + name + "'");
        }
        auto *displayCreator = reinterpret_cast<std::unique_ptr<IDisplayModule>(*)()>(sym);
        auto display = displayCreator();
        const std::string str = display->getName();
        this->_displayModules[str] = std::pair(std::move(lib), std::move(display));
        this->loadDisplay(str);
    } catch (DynamicLibrary::DynamicLibraryException&) {
        throw CoreException("Could not load library '" + name + "'");
    }
}

Arcade::IGameModule& Arcade::Core::getGame(const std::string& name) {
    return *(this->_gameModules[name].second);
}

Arcade::IDisplayModule& Arcade::Core::getDisplay(const std::string& name) {
    return *(this->_displayModules[name].second);
}

void Arcade::Core::updateLibraries() {
    if (!std::filesystem::exists("./lib") || !std::filesystem::is_directory("./lib")) {
        throw CoreException("Libraries not found");
    }
    for (const auto& entry : std::filesystem::directory_iterator("./lib")) {
        if (entry.is_regular_file() && entry.path().extension() == ".so" && std::find(this->_libraries.begin(), this->_libraries.end(), entry.path().string()) == this->_libraries.end()) {
            try {
                this->_libraries.push_back(entry.path().string());
                auto lib = std::make_unique<DynamicLibrary>(entry.path().string());
                void *sym = lib->getSymbol("createInstanceIGame");
                if (sym) {
                    auto *gameCreator = reinterpret_cast<std::unique_ptr<IGameModule>(*)()>(sym);
                    auto game = gameCreator();
                    std::string str = game->getName();
                    if (!this->_gameModules.contains(str))
                        this->_gameModules[str] = std::pair(std::move(lib), std::move(game));
                } else {
                    sym = lib->getSymbol("createInstanceIDisplay");
                    if (sym) {
                        auto *displayCreator = reinterpret_cast<std::unique_ptr<IDisplayModule>(*)()>(sym);
                        auto display = displayCreator();
                        std::string str = display->getName();
                        if (!this->_displayModules.contains(str))
                            this->_displayModules[str] = std::pair(std::move(lib), std::move(display));
                    }
                }
            } catch (DynamicLibrary::DynamicLibraryException& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
}

void Arcade::Core::handleInputs() {
    this->_input = this->getDisplay(*this->_loadedDisplay).getInput();
    this->_mousePos = this->getDisplay(*this->_loadedDisplay).getMousePos();

    switch (this->_input) {
        case CTRL('q'):
            this->_running = false;
            break;
        case K_ESC:
            this->unloadGame(*this->_loadedDisplay);
            this->loadGame("Main Menu");
            break;
        case CTRL('d'):
            this->goToNextDisplay();
            break;
        case CTRL('g'):
            this->goToNextGame();
            break;
        default:
            break;
    }
}

std::vector<std::string> Arcade::Core::getDisplayList() const {
    std::vector<std::string> vec;

    for (const auto& pair : this->_displayModules)
        vec.push_back(pair.first);
    return vec;
}

std::vector<std::string> Arcade::Core::getGamesList() const {
    std::vector<std::string> vec;

    for (const auto& pair : this->_gameModules)
        if (pair.first != "Main Menu")
        vec.push_back(pair.first);
    return vec;
}


void Arcade::Core::updateLoadedGame() {
    if (*this->_loadedGame == "Main Menu") {
        try {
            dynamic_cast<MainMenu&>(this->getGame("Main Menu")).updateDisplay(this->getDisplayList());
            dynamic_cast<MainMenu&>(this->getGame("Main Menu")).updateGames(this->getGamesList());
        } catch (const std::bad_cast& e) {
            throw CoreException(e.what());
        }
    }
    const std::string oldLoadedDisplay = *this->_loadedDisplay;
    const bool updateGame = this->getGame(*this->_loadedGame).update(this->_mousePos, this->_input);
    if (oldLoadedDisplay != *this->_loadedDisplay) {
        this->unloadDisplay(oldLoadedDisplay);
        this->loadDisplay(*this->_loadedDisplay);
    }
    if (updateGame) {
        this->unloadGame(*this->_loadedGame);
        this->loadGame("Main Menu");
    }
}


void Arcade::Core::run() {
    while (this->_running) {
        this->updateLibraries();
        this->handleInputs();
        this->updateLoadedGame();
        this->getDisplay(*this->_loadedDisplay).initObject(this->getGame(*this->_loadedGame).getObjects());
        this->getDisplay(*this->_loadedDisplay).display(this->getGame(*this->_loadedGame).getObjects());
    }
}

void Arcade::Core::loadGame(const std::string &name) const {
    if (this->_gameModules.contains(name)) {
        *this->_loadedGame = name;
    }
}

void Arcade::Core::unloadGame(const std::string &name) {
    const std::string toWrite = name + " " + dynamic_cast<MainMenu&>(this->getGame("Main Menu")).getPlayerName() + " " + std::to_string(this->getGame(*this->_loadedGame).getScore());
    std::ofstream file(".save", std::ios::app);

    if (!file.is_open())
        throw CoreException("Could not open save file.");
    file << toWrite << std::endl;
    file.close();
}

void Arcade::Core::loadDisplay(const std::string &name) {
    if (!this->_displayModules.contains(name)) {
        throw CoreException("Non existant display module");
    }

    if (name != "NCURSES") {
        const char* display = std::getenv("DISPLAY");
        if (!display || !*display || display[0] != ':' || display[1] == '\0' ||
            !isdigit(display[1]) || display[2] != '\0') {
            throw CoreException("Invalid DISPLAY");
        }
    }

    if (!this->_loadedDisplay->empty()) {
        unloadDisplay(*this->_loadedDisplay);
    }

    *this->_loadedDisplay = name;
    this->getDisplay(name).initObject(this->getGame(*this->_loadedGame).getObjects());
    this->getDisplay(name).openWindow();
}

void Arcade::Core::unloadDisplay(const std::string &name) {
    if (this->_displayModules.contains(name)) {
        this->getDisplay(name).closeWindow();
    }
}

void Arcade::Core::goToNextGame() {
    if (this->_gameModules.size() <= 2)
        return;
    auto it = this->_gameModules.begin();
    for (; it != this->_gameModules.end(); ++it) {
        if (it->first == *this->_loadedGame)
            break;
    }
    ++it;
    if (it == this->_gameModules.end())
        it = this->_gameModules.begin();
    if (it->first == "Main Menu") {
        ++it;
        if (it == this->_gameModules.end())
            it = this->_gameModules.begin();
    }
    this->unloadGame(*this->_loadedGame);
    this->loadGame(it->first);
}

void Arcade::Core::goToNextDisplay() {
    if (this->_displayModules.size() <= 1)
        return;
    auto it = this->_displayModules.begin();
    for (; it != this->_displayModules.end(); ++it) {
        if (it->first == *this->_loadedDisplay)
            return;
    }
    ++it;
    if (it == this->_displayModules.end())
        it = this->_displayModules.begin();
    this->unloadDisplay(*this->_loadedDisplay);
    this->loadDisplay(it->first);
}
