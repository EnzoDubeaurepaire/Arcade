/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** SdlWrapper
*/

#ifndef SDLWRAPPER_H_
#define SDLWRAPPER_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

class SdlWrapper {
public:
    SdlWrapper();
    ~SdlWrapper();

    // Initialisation et nettoyage
    bool initSDL();
    bool initTTF();
    bool initIMG();
    void cleanup();

    // Création et gestion de fenêtre/renderer
    bool createWindow(const std::string& title, int width, int height);
    bool createRenderer();
    void destroyWindow();
    void destroyRenderer();

    // Accesseurs
    SDL_Window* getWindow() const { return this->_window; }
    SDL_Renderer* getRenderer() const { return this->_renderer; }
    bool isInitialized() const { return this->_isInitialized; }

    // Fonctions de rendu
    void clearRenderer();
    void presentRenderer();
    void setDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    // Création de textures
    SDL_Texture* createTextureFromSurface(SDL_Surface* surface);

private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;
    bool _isInitialized;
    bool _sdlInitialized;
    bool _ttfInitialized;
    bool _imgInitialized;
};

#endif /* !SDLWRAPPER_H_ */
