/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Sprite
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "IObject.hpp"

namespace Arcade {
    class Sprite final : public IObject {
    public:
        Sprite(const std::string& path);
        ~Sprite() override {
            _sprite = nullptr;
            _texture = nullptr;
        };

        std::string getType() const override {return SPRITE;};
        std::pair<int, int> getPosition() const override {return this->_position;};
        std::any& getSprite() override {return this->_sprite;};
        std::string getTexturePath() override {return this->_path;};
        std::any& getTexture() override {return this->_texture;};
        Properties getProperties() override {return this->_properties;};

        void setPosition(const std::pair<int, int> pos) override {this->_position = pos;};
        void setSprite(const std::any sprite) override {this->_sprite = sprite;};
        void setTexturePath(const std::string path) override {this->_path = path;};
        void setTexture(const std::any texture) override {this->_texture = texture;};
        void setProperties(const Properties properties) override {this->_properties = properties;};

    private:
        Properties _properties;
        std::string _path;
        std::any _sprite;
        std::any _texture;
        std::pair<int, int> _position;
    };
}
#endif
