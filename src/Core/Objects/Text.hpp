/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Text
*/

#ifndef TEXT_H
#define TEXT_H
#include "IObject.hpp"

class Text final : public IObject {
public:
    Text(const std::string& path);
    ~Text() override = default;

    std::pair<int, int> getOffset() const override {return {0, 0};};
    std::pair<int, int> getPosition() const override {return _pos;};
    std::pair<int, int> getSize() const override {return _size;};
    std::any &getSprite() override {return _sprite;};
    std::any &getTexture() override {return _font;};
    std::string getType() const override {return TEXT;};
    std::string getTexturePath() override {return _path;};
    std::string getText() override {return _text;};

    void setOffset(const std::pair<int, int> offset) override {(void)offset;};
    void setSize(const std::pair<int, int> size) override {_size = size;};
    void setSprite(const std::any sprite) override {_sprite = sprite;};
    void setPosition(const std::pair<int, int> pos) override {_pos = pos;};
    void setTexture(const std::any font) override {_font = font;};
    void setTexturePath(const std::string path) override {_path = path;};
    void setText(const std::string text) override {_text = text;};

private:
    std::string _path;
    std::pair<int, int> _pos;
    std::pair<int, int> _size;
    std::any _sprite;
    std::any _font;
    std::string _text;
};

#endif
