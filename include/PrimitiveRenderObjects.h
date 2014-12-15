#ifndef __PRIMITIVERENDEROBJECTS_H__
#define __PRIMITIVERENDEROBJECTS_H__

#include <SFML/Graphics.hpp>

#include "string_t.h"
#include "IRenderObject.h"


class TextRenderObject : public IRenderObject
{
private:
    sf::Vector2f _position;
    string_t _text_name;
    sf::Text * _text;
    int _font_size;
public:
    TextRenderObject(string_t text_name);
    virtual ~TextRenderObject();

    virtual void render(sf::RenderTarget &);
    virtual void prep();
    virtual void set_scale(float, float);
    virtual const sf::Vector2f& get_scale() const;
    virtual void set_position(float, float);
    virtual const sf::Vector2f& get_position() const;
    void set_font_size(int size);
    virtual sf::FloatRect get_bounds() const;
    virtual void set_color(sf::Color c);
    virtual const sf::Color& get_color() const;
};

class SpriteRenderObject : public IRenderObject
{
private:
    sf::Vector2f _position;
    string_t _sprite_name;
    sf::Sprite * _sprite;
public:
    SpriteRenderObject(string_t sprite_name);
    virtual ~SpriteRenderObject();

    virtual void render(sf::RenderTarget &);
    virtual void prep();
    virtual void set_scale(float, float);
    virtual const sf::Vector2f& get_scale() const;
    virtual void set_position(float, float);
    virtual const sf::Vector2f& get_position() const;
    void set_font_size(int size);
    virtual sf::FloatRect get_bounds() const;
    virtual void set_color(sf::Color c);
    virtual const sf::Color& get_color() const;
};

#endif // __PRIMITIVERENDEROBJECTS_H__
