/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SdlWrapper
*/

#include "SdlWrapper.hpp"

SdlWrapper::SdlWrapper() : _window(nullptr), _renderer(nullptr),
                           _isInitialized(false), _sdlInitialized(false),
                           _ttfInitialized(false), _imgInitialized(false)
{
}

SdlWrapper::~SdlWrapper()
{
    this->cleanup();
}

bool SdlWrapper::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    this->_sdlInitialized = true;
    return true;
}

bool SdlWrapper::initTTF()
{
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }
    this->_ttfInitialized = true;
    return true;
}

bool SdlWrapper::initIMG()
{
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }
    this->_imgInitialized = true;
    return true;
}

bool SdlWrapper::createWindow(const std::string& title, int width, int height)
{
    if (!this->_sdlInitialized) {
        std::cerr << "SDL must be initialized before creating a window!" << std::endl;
        return false;
    }

    this->_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_SHOWN
    );

    if (!this->_window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool SdlWrapper::createRenderer()
{
    if (!this->_window) {
        std::cerr << "Window must exist before creating a renderer!" << std::endl;
        return false;
    }

    this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);

    if (!this->_renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    this->_isInitialized = true;
    return true;
}

void SdlWrapper::destroyRenderer()
{
    if (this->_renderer) {
        SDL_DestroyRenderer(this->_renderer);
        this->_renderer = nullptr;
    }
}

void SdlWrapper::destroyWindow()
{
    if (this->_window) {
        SDL_DestroyWindow(this->_window);
        this->_window = nullptr;
    }
}

void SdlWrapper::cleanup()
{
    this->destroyRenderer();
    this->destroyWindow();

    if (this->_imgInitialized) {
        IMG_Quit();
        this->_imgInitialized = false;
    }

    if (this->_ttfInitialized) {
        TTF_Quit();
        this->_ttfInitialized = false;
    }

    if (this->_sdlInitialized) {
        SDL_Quit();
        this->_sdlInitialized = false;
    }

    this->_isInitialized = false;
}

void SdlWrapper::clearRenderer()
{
    if (this->_renderer) {
        SDL_RenderClear(this->_renderer);
    }
}

void SdlWrapper::presentRenderer()
{
    if (this->_renderer) {
        SDL_RenderPresent(this->_renderer);
    }
}

void SdlWrapper::setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if (this->_renderer) {
        SDL_SetRenderDrawColor(this->_renderer, r, g, b, a);
    }
}

SDL_Texture* SdlWrapper::createTextureFromSurface(SDL_Surface* surface)
{
    if (!this->_renderer || !surface) {
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(this->_renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
    }

    return texture;
}
