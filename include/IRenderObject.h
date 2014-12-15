#ifndef __IRENDEROBJECT_H__
#define __IRENDEROBJECT_H__

#include <SFML/Graphics.hpp>


class IRenderObject
{
public:
    virtual ~IRenderObject();

    virtual void prep()=0;
    virtual void render(sf::RenderTarget &)=0;
    virtual void set_scale(float, float)=0;
    virtual const sf::Vector2f& get_scale() const=0;
    virtual void set_position(float, float)=0;
    virtual const sf::Vector2f& get_position() const=0;
    virtual sf::FloatRect get_bounds() const=0;
    virtual void set_color(sf::Color c)=0;
    virtual const sf::Color& get_color() const=0;
};

#endif // __IRENDEROBJECT_H__
