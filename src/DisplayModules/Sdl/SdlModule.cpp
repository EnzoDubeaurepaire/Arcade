/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SdlModule
*/

#include "SdlModule.hpp"

#include <iostream>

Arcade::SdlModule::SdlResource::SdlResource() : texture(nullptr), font(nullptr), fontSize(0) {}

Arcade::SdlModule::SdlResource::~SdlResource() {
    this->cleanup();
}

void Arcade::SdlModule::SdlResource::cleanup() {
    if (this->texture) {
        if (SDL_WasInit(SDL_INIT_VIDEO)) {
            SDL_DestroyTexture(this->texture);
        }
        this->texture = nullptr;
    }

    if (this->font) {
        if (TTF_WasInit()) {
            TTF_CloseFont(this->font);
        }
        this->font = nullptr;
    }
    this->text.clear();
}

Arcade::SdlModule::~SdlModule() {
    for (auto& res : _resources) {
        if (res.second) {
            res.second->cleanup();
        }
    }
    _resources.clear();
    if (_sdlWrapper.getRenderer()) {
        SDL_RenderClear(_sdlWrapper.getRenderer());
        SDL_RenderPresent(_sdlWrapper.getRenderer());
    }
    SDL_Delay(100);
    closeWindow();
}

Arcade::SdlModule::SdlModule() = default;

int Arcade::SdlModule::getInput() {
    SDL_Event event{};
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
            event.key.keysym.mod & KMOD_CTRL && event.key.keysym.sym == SDLK_q))
            return CTRL('q');

        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.mod & KMOD_CTRL) {
                if (event.key.keysym.sym == SDLK_d)
                    return CTRL('d');
                if (event.key.keysym.sym == SDLK_g)
                    return CTRL('g');
            }

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
            if (event.key.keysym.sym == SDLK_SPACE)
                return ' ';
            if (event.key.keysym.sym == SDLK_ESCAPE)
                return K_ESC;
            if (event.key.keysym.sym == SDLK_r)
                return 'r';
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            _lastMousePos = {event.button.x, event.button.y};

            if (event.button.button == SDL_BUTTON_LEFT)
                return K_LCLICK;
            if (event.button.button == SDL_BUTTON_RIGHT)
                return K_RCLICK;
            if (event.button.button == SDL_BUTTON_MIDDLE)
                return K_MCLICK;
            }

        if (event.type == SDL_MOUSEMOTION) {
            _lastMousePos = {event.motion.x, event.motion.y};
        }
    }
    return 0;
}

void Arcade::SdlModule::openWindow() {
    if (_isInitialized)
        return;

    if (!_sdlWrapper.initSDL())
        return;
    if (!_sdlWrapper.initTTF()) {
        _sdlWrapper.cleanup();
        return;
    }
    if (!_sdlWrapper.initIMG()) {
        _sdlWrapper.cleanup();
        return;
    }
    if (!_sdlWrapper.createWindow("SDL2 Window", 1920, 1080)) {
        _sdlWrapper.cleanup();
        return;
    }
    if (!_sdlWrapper.createRenderer()) {
        _sdlWrapper.cleanup();
        return;
    }
    _isInitialized = true;
}

void Arcade::SdlModule::closeWindow() {
    if (!_isInitialized)
        return;
    for (auto& res : _resources) {
        if (res.second) {
            res.second->cleanup();
        }
    }
    _resources.clear();
    if (_sdlWrapper.getRenderer()) {
        SDL_RenderClear(_sdlWrapper.getRenderer());
        SDL_RenderPresent(_sdlWrapper.getRenderer());
    }
    SDL_Delay(50);
    _sdlWrapper.cleanup();
    _isInitialized = false;
}

void Arcade::SdlModule::display(std::map<std::string, std::unique_ptr<Arcade::IObject>>& objects) {
    if (!_isInitialized || !_sdlWrapper.getRenderer())
        return;

    _sdlWrapper.setDrawColor(0, 0, 0, 255);
    _sdlWrapper.clearRenderer();
    for (auto& object : objects) {
        try {
            std::string key;
            try {
                key = any_cast<std::string>(object.second->getSprite());
            } catch (const std::bad_any_cast&) {
                continue;
            }
            auto it = _resources.find(key);
            if (it == _resources.end() || !it->second)
                continue;
            auto& resource = it->second;
            if (object.second->getType() == SPRITE && resource->texture) {
                SDL_Rect srcRect;
                Arcade::IObject::SpriteProperties props = std::get<Arcade::IObject::SpriteProperties>(object.second->getProperties());
                srcRect.x = props.offset.first;
                srcRect.y = props.offset.second;
                srcRect.w = props.size.first;
                srcRect.h = props.size.second;

                SDL_Rect dstRect;
                dstRect.x = object.second->getPosition().first;
                dstRect.y = object.second->getPosition().second;

                dstRect.w = static_cast<int>(props.size.first * props.scale.first);
                dstRect.h = static_cast<int>(props.size.second * props.scale.second);

                SDL_RenderCopy(_sdlWrapper.getRenderer(), resource->texture, &srcRect, &dstRect);
            }
            if (object.second->getType() == TEXT && resource->font) {
                Arcade::IObject::TextProperties props = std::get<Arcade::IObject::TextProperties>(object.second->getProperties());
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
                            resource->texture = _sdlWrapper.createTextureFromSurface(textSurface);
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

                    SDL_RenderCopy(_sdlWrapper.getRenderer(), resource->texture, NULL, &dstRect);
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error during display: " << e.what() << std::endl;
        }
    }

    _sdlWrapper.presentRenderer();
}

void Arcade::SdlModule::initObject(std::map<std::string, std::unique_ptr<Arcade::IObject>>& objects) {
    if (!_isInitialized)
        openWindow();

    if (!_isInitialized || !_sdlWrapper.getRenderer()) {
        std::cerr << "SDL not initialized in initObject!" << std::endl;
        return;
    }

    for (auto& object : objects) {
        if (std::string(object.second->getSprite().type().name()) == "NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE")
            continue;
        auto resource = std::make_unique<SdlResource>();
        std::string key = object.first;

        if (object.second->getType() == SPRITE) {
            std::string path = "./assets/" + object.second->getTexturePath() + ".png";
            SDL_Surface* surface = IMG_Load(path.c_str());
            if (surface) {
                resource->texture = _sdlWrapper.createTextureFromSurface(surface);
                SDL_FreeSurface(surface);

                if (!resource->texture) {
                    std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
                }
            } else {
                std::cerr << "Failed to load image: " << path << " - " << IMG_GetError() << std::endl;
            }
        }
        if (object.second->getType() == TEXT) {
            Arcade::IObject::TextProperties props = std::get<Arcade::IObject::TextProperties>(object.second->getProperties());
            std::string fontPath = "./assets/" + object.second->getTexturePath() + ".ttf";
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
                        resource->texture = _sdlWrapper.createTextureFromSurface(textSurface);
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

extern "C" {
std::unique_ptr<Arcade::IDisplayModule> createInstanceIDisplay() {
    return std::make_unique<Arcade::SdlModule>();
}
}
