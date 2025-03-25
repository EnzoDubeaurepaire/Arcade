/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Sprite
*/

#ifndef SPRITE_H
#define SPRITE_H

#include "IObject.hpp"

class Sprite final : public IObject {
public:
    Sprite(const std::string& path);
    ~Sprite() override = default;

    std::string getType() const override {return SPRITE;};
    std::pair<int, int> getPosition() const override {return this->_position;};
    std::any& getSprite() override {return this->_sprite;};
    std::string getTexturePath() override {return this->_path;};
    std::pair<int, int> getOffset() const override {return this->_offset;};
    std::pair<int, int> getSize() const override {return this->_size;};
    std::any& getTexture() override {return this->_texture;};
    std::string getText() override {return "";};

    void setPosition(const std::pair<int, int> pos) override {this->_position = pos;};
    void setSprite(const std::any sprite) override {this->_sprite = sprite;};
    void setTexturePath(const std::string path) override {this->_path = path;};
    void setOffset(const std::pair<int, int> offset) override {this->_offset = offset;};
    void setSize(const std::pair<int, int> size) override {this->_size = size;};
    void setTexture(const std::any texture) override {this->_texture = texture;};
    void setText(const std::string text) override {(void)text;};

private:
    std::string _path;
    std::any _sprite;
    std::any _texture;
    std::pair<int, int> _position;
    std::pair<int, int> _offset;
    std::pair<int, int> _size;
};

#endif
