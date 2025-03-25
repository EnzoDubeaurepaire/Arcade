/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#include "Core.hpp"
#include <algorithm>
#include <filesystem>

#include "MainMenu.hpp"

Core::Core(const std::string& baseDisplay) {
    this->_loadedGame = "Main Menu";
    this->_gameModules["Main Menu"] = std::pair(std::make_unique<DynamicLibrary>(), std::make_unique<MainMenu>());
    this->_loadedDisplay = baseDisplay;
    this->updateLibraries();
    if (!this->isLibLoaded(baseDisplay)) {
        throw CoreException("Could not find or open " + baseDisplay + " library");
    }
    this->loadDisplay(baseDisplay);
}

IGameModule& Core::getGame(const std::string& name) {
    return *(this->_gameModules[name].second);
}

IDisplayModule& Core::getDisplay(const std::string& name) {
    return *(this->_displayModules[name].second);
}

bool Core::isLibLoaded(const std::string &name) {
    if (name == "Main Menu")
        return true;
    if (this->_gameModules.contains(name)) {
        return this->_gameModules[name].first->isLoaded();
    }
    if (this->_displayModules.contains(name)) {
        return this->_displayModules[name].first->isLoaded();
    }
    return false;
}

void Core::getDisplayFallback() {
    if (this->_displayModules.empty())
        throw CoreException("No display modules found");
    this->loadDisplay(this->_displayModules.begin()->first);
}

void Core::getGameFallback() {
    if (this->_gameModules.empty()) {
        this->_loadedGame = "Main Menu";
    } else
        this->loadGame(this->_gameModules.begin()->first);
}

void Core::updateLibraries() {
    if (!std::filesystem::exists("./lib") || !std::filesystem::is_directory("./lib")) {
        throw CoreException("Libraries not found");
    }
    for (const auto& entry : std::filesystem::directory_iterator("./lib")) {
        if (entry.is_regular_file() && entry.path().extension() == ".so") {
            try {
                DynamicLibrary lib(entry.path().string());
                void *sym = lib.getSymbol("createInstanceIGame");
                if (sym) {
                    auto *gameCreator = reinterpret_cast<std::unique_ptr<IGameModule>(*)()>(sym);
                    auto game = gameCreator();
                    std::string str = game->getName();
                    if (!this->_gameModules.contains(str))
                        this->_gameModules[str] = std::pair(std::make_unique<DynamicLibrary>(lib), std::move(game));
                }
                sym = lib.getSymbol("createInstanceIDisplay");
                if (sym) {
                    auto *displayCreator = reinterpret_cast<std::unique_ptr<IDisplayModule>(*)()>(sym);
                    auto display = displayCreator();
                    std::string str = display->getName();
                    if (!this->_displayModules.contains(str))
                        this->_displayModules[str] = std::pair(std::make_unique<DynamicLibrary>(lib), std::move(display));
                }
            } catch (DynamicLibrary::DynamicLibraryException& e) {
                std::cout << e.what() << std::endl;
            }
        }
    }
    for (const auto& lib : this->_gameModules)
        if (!this->isLibLoaded(lib.first))
            this->_gameModules.erase(lib.first);
    for (const auto& lib : this->_displayModules)
        if (!this->isLibLoaded(lib.first))
            this->_displayModules.erase(lib.first);
    if (!this->isLibLoaded(this->_loadedDisplay))
        this->getDisplayFallback();
    if (!this->isLibLoaded(this->_loadedGame))
        this->getGameFallback();
}

void Core::handleInputs() {
    const int input = this->getDisplay(this->_loadedDisplay).getInput();
    const std::pair<int, int> mousePos = this->getDisplay(this->_loadedDisplay).getMousePos();
    const IDisplayModule::click mouseState = this->getDisplay(this->_loadedDisplay).getMouseState();

    switch (input) {
        case CTRL('q'):
            this->_running = false;
            break;
        case KEY_ESC:
            this->unloadGame(this->_loadedDisplay);
            this->loadGame("Main Menu");
            break;
        case CTRL('d'):
            this->goToNextDisplay();
            break;
        case CTRL('g'):
            this->goToNextGame();
            break;
        default:
            this->getGame(this->_loadedGame).update(mousePos, mouseState, input);
    }
}

void Core::run() {
    while (this->_running) {
        this->updateLibraries();
        this->handleInputs();
        this->getDisplay(this->_loadedDisplay).display(this->getGame(this->_loadedGame).getObjects());
    }
}

void Core::loadGame(const std::string &name) {
    if (this->_gameModules.contains(name)) {
        this->_loadedGame = name;
    }
}

void Core::unloadGame(const std::string &name) {
    (void)name;
}

void Core::loadDisplay(const std::string &name) {
    if (this->_displayModules.contains(name)) {
        this->_loadedDisplay = name;
        this->getDisplay(name).initObject(this->getGame(this->_loadedGame).getObjects());
        this->getDisplay(name).openWindow();
    }
}

void Core::unloadDisplay(const std::string &name) {
    if (this->_displayModules.contains(name)) {
        this->getDisplay(name).closeWindow();
    }
}

void Core::goToNextGame() {
    if (this->_gameModules.size() <= 2)
        return;
    auto it = this->_gameModules.begin();
    for (; it != this->_gameModules.end(); ++it) {
        if (it->first == this->_loadedGame)
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
    this->unloadGame(this->_loadedGame);
    this->loadGame(it->first);
}

void Core::goToNextDisplay() {
    if (this->_displayModules.size() <= 1)
        return;
    auto it = this->_displayModules.begin();
    for (; it != this->_displayModules.end(); ++it) {
        if (it->first == this->_loadedDisplay)
            return;
    }
    ++it;
    if (it == this->_displayModules.end())
        it = this->_displayModules.begin();
    this->unloadDisplay(this->_loadedDisplay);
    this->loadDisplay(it->first);
}
