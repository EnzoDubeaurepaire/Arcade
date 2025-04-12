/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Text
*/

#ifndef TEXT_H
#define TEXT_H
#include <iostream>
#include <ostream>

#include "IObject.hpp"

namespace Arcade {
    class Text final : public IObject {
    public:
        Text(const std::string& path);
        ~Text() override {
            _font = nullptr;
            _sprite = nullptr;
        };

        std::pair<int, int> getPosition() const override {return _pos;};
        std::any &getSprite() override {return _sprite;};
        std::any &getTexture() override {return _font;};
        std::string getType() const override {return TEXT;};
        std::string getTexturePath() override {return _path;};
        Properties getProperties() override {return _properties;};

        void setSprite(const std::any sprite) override {_sprite = sprite;};
        void setPosition(const std::pair<int, int> pos) override {_pos = pos;};
        void setTexture(const std::any font) override {_font = font;};
        void setTexturePath(const std::string path) override {_path = path;};
        void setProperties(const Properties properties) override {this->_properties = properties;};

    private:
        Properties _properties;
        std::string _path;
        std::pair<int, int> _pos;
        std::any _sprite;
        std::any _font;
    };
}

#endif
