#include "PrimitiveRenderObjects.h"
#include "ResourceManager.h"

TextRenderObject::TextRenderObject(string_t text_name)
{
    _text_name = text_name;
    _text = NULL;
    _font_size = -1;
}

TextRenderObject::~TextRenderObject()
{
    delete _text;
    _text_name = TEXT("");
}

void TextRenderObject::render(sf::RenderTarget & target)
{
    target.draw(*_text);
}

void TextRenderObject::prep()
{
    _text = ResourceManager::get_text(_text_name);
    _text->setPosition(_position.x * ResourceManager::scaling_factor().x, _position.y  * ResourceManager::scaling_factor().y);
    if(_font_size > 0) _text->setCharacterSize((unsigned int)(_font_size * ResourceManager::scaling_factor().x));

}

void TextRenderObject::set_scale(float x, float y)
{
    _text->setScale(x, y);
}

const sf::Vector2f& TextRenderObject::get_scale() const
{
    return _text->getScale();
}

void TextRenderObject::set_position(float x, float y)
{
    _position = sf::Vector2f(x, y);
    if(_text != NULL)
    {
        _text->setPosition(_position);
    }
}

const sf::Vector2f& TextRenderObject::get_position() const
{
    return _text->getPosition();
}

void TextRenderObject::set_font_size(int size)
{
    _font_size = size;
}

sf::FloatRect TextRenderObject::get_bounds() const
{
    return _text->getGlobalBounds();
}

void TextRenderObject::set_color(sf::Color c)
{
    _text->setColor(c);
}

const sf::Color& TextRenderObject::get_color() const
{
    return _text->getColor();
}


SpriteRenderObject::SpriteRenderObject(string_t sprite_name)
{
    _sprite_name = sprite_name;
    _sprite = NULL;
}

SpriteRenderObject::~SpriteRenderObject()
{
    delete _sprite;
    _sprite_name = TEXT("");
}

void SpriteRenderObject::render(sf::RenderTarget & target)
{
    target.draw(*_sprite);
}

void SpriteRenderObject::prep()
{
    _sprite = ResourceManager::get_sprite(_sprite_name);
    _sprite->setPosition(_position.x * ResourceManager::scaling_factor().x, _position.y  * ResourceManager::scaling_factor().y);
}

void SpriteRenderObject::set_scale(float x, float y)
{
    _sprite->setScale(x, y);
}

const sf::Vector2f& SpriteRenderObject::get_scale() const
{
    return _sprite->getScale();
}

void SpriteRenderObject::set_position(float x, float y)
{
    _position = sf::Vector2f(x, y);
    if(_sprite != NULL)
    {
        _sprite->setPosition(_position.x * ResourceManager::scaling_factor().x, _position.y  * ResourceManager::scaling_factor().y);
    }
}

const sf::Vector2f& SpriteRenderObject::get_position() const
{
    return _sprite->getPosition();
}

sf::FloatRect SpriteRenderObject::get_bounds() const
{
    return _sprite->getGlobalBounds();
}

void SpriteRenderObject::set_color(sf::Color c)
{
    _sprite->setColor(c);
}

const sf::Color& SpriteRenderObject::get_color() const
{
    return _sprite->getColor();
}
