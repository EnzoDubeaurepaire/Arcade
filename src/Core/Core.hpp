/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Core
*/

#ifndef CORE_H
#define CORE_H
#include <map>
#include <memory>
#include <string>

#include "IDisplayModule.hpp"
#include "IGameModule.hpp"
#include "Loader.hpp"
#include "KeyCodes.hpp"
#include <vector>

class Core
{
public:
    class CoreException final : public std::exception
    {
    public:
        explicit CoreException(const std::string &what) : message("Core error : " + what) {};
        const char* what() const noexcept override {return this->message.c_str();};
    private:
        std::string message;
    };

    explicit Core(const std::string& baseDisplay);
    ~Core() = default;
    IGameModule& getGame(const std::string& name);
    IDisplayModule& getDisplay(const std::string& name);
    bool isLibLoaded(const std::string& name);
    void updateLibraries();
    void run();


private:
    bool _running = true;
    std::shared_ptr<std::string> _loadedGame;
    std::shared_ptr<std::string> _loadedDisplay;
    std::map<std::string, std::pair<std::unique_ptr<DynamicLibrary>, std::unique_ptr<IGameModule>>> _gameModules;
    std::map<std::string, std::pair<std::unique_ptr<DynamicLibrary>, std::unique_ptr<IDisplayModule>>> _displayModules;
    std::pair<int, int> _mousePos;
    int _input{};

    void loadGame(const std::string& name);
    void unloadGame(const std::string& name);
    void loadDisplay(const std::string& name);
    void unloadDisplay(const std::string& name);
    void getDisplayFallback();
    void getGameFallback();
    void handleInputs();
    void goToNextDisplay();
    void goToNextGame();
    void loadFirstLib(const std::string& name);
    void updateLoadedGame();
    std::vector<std::string> getDisplayList() const;
    std::vector<std::string> getGamesList() const;
};

#endif
