/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SdlModule
*/

#ifndef SDLMODULE_H_
#define SDLMODULE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "KeyCodes.hpp"
#include "SdlWrapper.hpp"

#include "IDisplayModule.hpp"
#include <memory>
#include <map>
#include <string>
#include <utility>

namespace Arcade {
    class SdlModule final : public IDisplayModule {
    public:
        SdlModule();
        ~SdlModule() override;

        void initObject(
            std::map<std::string, std::unique_ptr<IObject>> &
        ) override;

        int getInput() override;

        std::pair<int, int> getMousePos() const override
        {
            return {0, 0};
        };

        std::string getName() const override
        {
            return "SDL";
        };

        void openWindow() override;
        void closeWindow() override;

        void display(
            std::map<std::string, std::unique_ptr<IObject>> &
        ) override;

    private:
        class SdlResource {
        public:
            SDL_Texture *texture;
            TTF_Font *font;
            std::string text;
            int fontSize;

            SdlResource();
            ~SdlResource();

            void cleanup();
        };

        SdlWrapper _sdlWrapper;
        bool _isInitialized = false;

        std::map<std::string, std::unique_ptr<SdlResource>> _resources;
    };
}

#endif
