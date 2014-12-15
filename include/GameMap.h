#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include <vector>

#include <SFML/Graphics.hpp>

#include "string_t.h"
#include "ScriptRaw.h"
#include "ScriptScope.h"
#include "IRenderObject.h"


class MapLayer;

class MapTileBuilder
{
private:
    MapLayer & _layer;
    int _row;
    int _column;

public:
    MapTileBuilder(MapLayer& layer, int column, int row);

    MapTileBuilder& operator=(int tile);
};

class MapColumnBuilder
{
private:
    MapLayer & _layer;
    int _column;

public:
    MapColumnBuilder(MapLayer& layer, int column);

    MapTileBuilder operator[](int row);
};

class MapLayer
{
private:

    std::vector<string_t> _sprite_names;
    std::vector<std::vector<sf::Sprite*>> _sprite_info;
    int _width;
    int _height;
    sf::Vector2f _scale;
	sf::Vector2f _position;

public:
    friend class MapTileBuilder;
    MapLayer(int w, int h);

    std::vector<std::vector<int>> tiles;
    MapColumnBuilder operator [](int);

    void render(sf::RenderTarget&);
    void prep();
    void set_scale(sf::Vector2f scale);

    ScriptScope * build_layer(ScriptRaw *);
};

class GameMap : public IRenderObject
{
private:
    int _width;
    int _height;
    std::vector<MapLayer> _layers;
    sf::Vector2f _scale;
    sf::FloatRect _size;
    sf::Vector2f _position;//note: not used yet!
    sf::Color _color; //note: not used yet!

public:
    GameMap();
    GameMap(int w, int h);
    virtual ~GameMap();

    MapLayer& operator[](int layer);

    void hack_a_map();

    virtual void prep();
    virtual void render(sf::RenderTarget &);
    virtual void set_scale(float, float);
    virtual const sf::Vector2f& get_scale() const;
    virtual void set_position(float, float);
    virtual const sf::Vector2f& get_position() const;
    virtual sf::FloatRect get_bounds() const;
    virtual void set_color(sf::Color c);
    virtual const sf::Color& get_color() const;

    ScriptScope * build_map(ScriptRaw *);
};

#endif // __GAMEMAP_H__
