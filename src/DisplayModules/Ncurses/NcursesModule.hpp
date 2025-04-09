#ifndef NCURSESMODULE_H_
#define NCURSESMODULE_H_

#include <map>
#include <memory>
#include <string>
#include "KeyCodes.hpp"
#include "IDisplayModule.hpp"
#include "NcursesWrapper.hpp"

class NcursesModule final : public IDisplayModule {
public:
    NcursesModule();
    ~NcursesModule();

    void initObject(std::map<std::string, std::unique_ptr<IObject>>&) override;
    int getInput() override;
    std::pair<int, int> getMousePos() const override {return {0, 0};};
    std::string getName() const override {return "NCURSES";};

    void openWindow() override;
    void closeWindow() override;
    void display(std::map<std::string, std::unique_ptr<IObject>>&) override;

private:
    std::pair<int, int> scaleCoords(int x, int y);
    short getNearestColor(u_int32_t color);

    NcursesWrapper _ncurses;
    bool _isOpen;
    int _termWidth;
    int _termHeight;
    int _gameWidth;
    int _gameHeight;
};

extern "C" {
std::unique_ptr<IDisplayModule> createInstanceIDisplay() {
    return std::make_unique<NcursesModule>();
}
}

#endif