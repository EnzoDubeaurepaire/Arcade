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
#include <variant>

// Object types (add more if needed)
#define SPRITE "SPRITE"
#define TEXT "TEXT"

#define COLOR(A, R, G, B) ((A << 24) | (R << 16) | (G << 8) | B)
#define GET_ALPHA(color) ((color >> 24) & 0xFF)
#define GET_RED(color) ((color >> 16) & 0xFF)
#define GET_GREEN(color) ((color >> 8) & 0xFF)
#define GET_BLUE(color) (color & 0xFF)

#define WHITE 0xffffffff
#define BLACK 0xff000000

// Definition of the IObject interface
namespace Arcade {
    class IObject
    {
    public:
        struct TextProperties {
            u_int32_t color;
            size_t characterSize;
            std::string text;
        };

        struct SpriteProperties {
            std::pair<int, int> size;
            std::pair<int, int> offset;
            std::pair<int, int> textSize;
            std::pair<int, int> textOffset;
            std::pair<float, float> scale;
            u_int32_t textColor;
        };

        using Properties = std::variant<TextProperties, SpriteProperties>;

        // Default virtual destructor
        virtual ~IObject() = default;

        // Position setter and getter
        virtual void setPosition(std::pair<int, int>) = 0;
        virtual std::pair<int, int> getPosition() const = 0;

        // Sprite getter and setter
        // !! getter returns a reference so the sprite properties can be modified by the display module
        virtual void setSprite(std::any) = 0;
        virtual std::any& getSprite() = 0;

        // Texture getter and setter
        // !! getter returns a reference so the texture properties can be modified by the display module
        virtual std::any& getTexture() = 0;
        virtual void setTexture(std::any) = 0;

        // Texture path getter and setter
        // !! The path represent the folder in which are stored the sprite sheets (MainMenu.png and MainMenu.txt)
        virtual void setTexturePath(std::string) = 0;
        virtual std::string getTexturePath() = 0;

        // Properties getter and setter
        virtual void setProperties(Properties properties) = 0;
        virtual Properties getProperties() = 0;

        // Gets the type of the object
        virtual std::string getType() const = 0;

    };
}
#endif
