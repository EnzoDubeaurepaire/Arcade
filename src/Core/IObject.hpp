/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IObject
*/

#ifndef IOBJECT_H
#define IOBJECT_H

// std includes
#include <any>
#include <string>
#include <utility>

// Object types (add more if needed)
#define SPRITE "SPRITE"
#define TEXT "TEXT"

// Definition of the IObject interface
class IObject
{
public:
    // Default virtual destructor
    virtual ~IObject() = default;

    // Position setter and getter
    virtual void setPosition(std::pair<int, int>) = 0;
    virtual std::pair<int, int> getPosition() const = 0;

    // Sprite getter and setter
    // !! getter returns a reference so the sprite properties can be modified by the display module
    virtual void setSprite(std::any) = 0;
    virtual std::any& getSprite() = 0;

    // Texture path getter and setter
    // !! The path represent the folder in which are stored the sprite sheets (graphical.png and text.txt)
    virtual void setTexturePath(std::string) = 0;
    virtual std::string getTexturePath() = 0;

    // Gets the type of the object
    virtual std::string getType() const = 0;

    // Size getter and setter
    // Size of the Sprite in the sprite sheet
    virtual std::pair<int, int> getSize() const = 0;
    virtual void setSize(std::pair<int, int>) = 0;

    // Offset getter and setter
    // This is the offset in the sprite sheet, aka the point where the sprite starts
    virtual std::pair<int, int> getOffset() const = 0;
    virtual void setOffset(std::pair<int, int>) = 0;

    // Texture getter and setter
    // !! getter returns a reference so the texture properties can be modified by the display module
    virtual std::any& getTexture() = 0;
    virtual void setTexture(std::any) = 0;

    // Text getter and setter
    virtual std::string getText() = 0;
    virtual void setText(std::string) = 0;
};

#endif
