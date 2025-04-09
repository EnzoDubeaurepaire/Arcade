/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SdlModule
*/

#include "SdlModule.hpp"

#include <iostream>


SdlResource::SdlResource() : texture(nullptr), font(nullptr), fontSize(0) {
}

SdlResource::~SdlResource() {
    cleanup();
}

void SdlResource::cleanup() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (font) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

SdlModule::SdlModule() = default;

SdlModule::~SdlModule() {
    _resources.clear();

    closeWindow();
}

int SdlModule::getInput() {
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
            event.key.keysym.mod & KMOD_CTRL && event.key.keysym.sym == SDLK_q))
            return CTRL('q');

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT)
                return K_LEFT;
            if (event.key.keysym.sym == SDLK_RIGHT)
                return K_RIGHT;
            if (event.key.keysym.sym == SDLK_UP)
                return K_UP;
            if (event.key.keysym.sym == SDLK_DOWN)
                return K_DOWN;
            if (event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z)
                return static_cast<int>(event.key.keysym.sym);
            if (event.key.keysym.sym == SDLK_BACKSPACE)
                return K_BACKSPACE;
            if (event.key.keysym.sym == SDLK_RETURN)
                return '\n';
        }
    }
    return 0;
}

void SdlModule::openWindow() {
    if (_isInitialized)
        return;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return;
    }
    _window = SDL_CreateWindow("SDL2 Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        SDL_WINDOW_SHOWN);
    if (!_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return;
    }
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(_window);
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return;
    }
    _isInitialized = true;
}

void SdlModule::closeWindow() {
    if (!_isInitialized) return;

    if (_renderer) {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    _isInitialized = false;
}

void SdlModule::display(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    if (!_isInitialized || !_renderer) return;

    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    for (auto& object : objects) {
        try {
            std::string key;
            try {
                key = any_cast<std::string>(object.second->getSprite());
            } catch (const std::bad_any_cast& e) {
                continue;
            }
            auto it = _resources.find(key);
            if (it == _resources.end() || !it->second) continue;
            auto& resource = it->second;
            if (object.second->getType() == SPRITE && resource->texture) {
                SDL_Rect srcRect;
                IObject::SpriteProperties props = std::get<IObject::SpriteProperties>(object.second->getProperties());
                srcRect.x = props.offset.first;
                srcRect.y = props.offset.second;
                srcRect.w = props.size.first;
                srcRect.h = props.size.second;
                SDL_Rect dstRect;
                dstRect.x = object.second->getPosition().first;
                dstRect.y = object.second->getPosition().second;
                dstRect.w = props.size.first;
                dstRect.h = props.size.second;

                SDL_RenderCopy(_renderer, resource->texture, &srcRect, &dstRect);
            }
            if (object.second->getType() == TEXT && resource->font) {
                IObject::TextProperties props = std::get<IObject::TextProperties>(object.second->getProperties());
                SDL_Color textColor = {
                    static_cast<Uint8>(GET_RED(props.color)),
                    static_cast<Uint8>(GET_GREEN(props.color)),
                    static_cast<Uint8>(GET_BLUE(props.color)),
                    static_cast<Uint8>(GET_ALPHA(props.color))
                };
                if (resource->text != props.text || (long unsigned int)resource->fontSize != props.characterSize) {
                    resource->text = props.text;
                    resource->fontSize = props.characterSize;

                    if (resource->texture) {
                        SDL_DestroyTexture(resource->texture);
                        resource->texture = nullptr;
                    }
                    if (!props.text.empty()) {
                        SDL_Surface* textSurface = TTF_RenderText_Solid(resource->font, props.text.c_str(), textColor);
                        if (textSurface) {
                            resource->texture = SDL_CreateTextureFromSurface(_renderer, textSurface);
                            SDL_FreeSurface(textSurface);
                        }
                    }
                }
                if (resource->texture) {
                    int width, height;
                    SDL_QueryTexture(resource->texture, NULL, NULL, &width, &height);

                    SDL_Rect dstRect;
                    dstRect.x = object.second->getPosition().first;
                    dstRect.y = object.second->getPosition().second;
                    dstRect.w = width;
                    dstRect.h = height;

                    SDL_RenderCopy(_renderer, resource->texture, NULL, &dstRect);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error during display: " << e.what() << std::endl;
        }
    }
    SDL_RenderPresent(_renderer);
}

void SdlModule::initObject(std::map<std::string, std::unique_ptr<IObject>>& objects) {
    if (!_isInitialized) {
        openWindow();
    }
    if (!_isInitialized || !_renderer) {
        std::cerr << "SDL not initialized in initObject!" << std::endl;
        return;
    }
    for (auto& object : objects) {
        auto resource = std::make_unique<SdlResource>();
        std::string key = object.first;
        if (object.second->getType() == SPRITE) {
            std::string path = object.second->getTexturePath() + "/graphical.png";
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (surface) {
                resource->texture = SDL_CreateTextureFromSurface(_renderer, surface);
                SDL_FreeSurface(surface);

                if (!resource->texture) {
                    std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
                }
            } else {
                std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
            }
        }
        if (object.second->getType() == TEXT) {
            IObject::TextProperties props = std::get<IObject::TextProperties>(object.second->getProperties());
            std::string fontPath = object.second->getTexturePath() + "/font.ttf";
            resource->font = TTF_OpenFont(fontPath.c_str(), props.characterSize);
            if (resource->font) {
                resource->fontSize = props.characterSize;
                resource->text = props.text;

                if (!props.text.empty()) {
                    SDL_Color textColor = {
                        static_cast<Uint8>(GET_RED(props.color)),
                        static_cast<Uint8>(GET_GREEN(props.color)),
                        static_cast<Uint8>(GET_BLUE(props.color)),
                        static_cast<Uint8>(GET_ALPHA(props.color))
                    };
                    SDL_Surface* textSurface = TTF_RenderText_Solid(resource->font, props.text.c_str(), textColor);
                    if (textSurface) {
                        resource->texture = SDL_CreateTextureFromSurface(_renderer, textSurface);
                        SDL_FreeSurface(textSurface);
                    }
                }
            } else {
                std::cerr << "Failed to load font: " << fontPath << std::endl;
            }
        }
        _resources[key] = std::move(resource);
        object.second->setSprite(key);
        object.second->setTexture(key);
    }
}