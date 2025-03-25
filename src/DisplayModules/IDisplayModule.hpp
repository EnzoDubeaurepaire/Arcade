/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IDisplayModule
*/

#ifndef IDISPLAYMODULE_H
#define IDISPLAYMODULE_H

// std includes
#include <memory>
#include <map>

// project includes
#include "IObject.hpp"

// Names for the display modules :
#define SFML "SFML"
#define CURSES "CURSES"
#define SDL "SDL"
#define CACA "CACA"
#define AA "AA"
#define ALLEGRO "ALLEGRO"
#define XLIB "XLIB"
#define GTK "GTK"
#define IRRLICHT "IRRLICHT"
#define OPENGL "OPENGL"
#define VULKAN "VULKAN"
#define QT "QT"

// Definition of the IDisplayModule interface
class IDisplayModule
{
public:
    // Enumeration for mouse click states
    enum click {
        NOTHING = 0, // No Click
        RIGHT,       // Right Click
        LEFT         // Left Click
    };

    // Default virtual destructor
    virtual ~IDisplayModule() = default;

    // Initializes objects to be displays
    // takes a reference to the map of objects
    virtual void initObject(std::map<std::string, std::unique_ptr<IObject>>&) = 0;

    // Retrieves keyboard inputs
    virtual int getInput() = 0;
    // Retrieves mouse position
    virtual std::pair<int, int> getMousePos() const = 0;
    // Retrieves mouse state
    virtual click getMouseState() const = 0;

    // Open the window
    virtual void openWindow() = 0;
    // Close the window
    virtual void closeWindow() = 0;

    // Displays the objects on screen
    // Takes a reference to the map of objects
    virtual void display(std::map<std::string, std::unique_ptr<IObject>>&) = 0;

    // Gets the name of the Display Module for example SFML
    virtual std::string getName() const = 0;
};

#endif
