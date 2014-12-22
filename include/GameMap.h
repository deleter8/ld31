#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include <vector>

#include <SFML/Graphics.hpp>

#include "string_t.h"
#include "ScriptRaw.h"
#include "ScriptScope.h"
#include "IRenderObject.h"
#include "TileMap.h"
#include "InteractionBase.h"


class GameMap;

class GameMapLayerBase
{
protected:
    string_t _tex_name;
    string_t _data_name;
    sf::Vector2f _offset;

public:
    friend class GameMap;

    GameMapLayerBase();

    virtual ~GameMapLayerBase();

    virtual void render(sf::RenderTarget &)=0;
    virtual void prep()=0;
};

class TiledGameMapLayer : public GameMapLayerBase
{
private:

    TileMap * _tile_map;

public:

    TiledGameMapLayer();
    virtual ~TiledGameMapLayer();

    virtual void render(sf::RenderTarget &);
    virtual void prep();

};

class SparseGameMapLayer : public GameMapLayerBase
{
private:

public:

    SparseGameMapLayer();
    virtual ~SparseGameMapLayer();

    virtual void render(sf::RenderTarget &);
    virtual void prep();

};

class GameMap : public IRenderObject, public InteractionBase
{
private:
    int _width;
    int _height;
    std::vector<GameMapLayerBase*> _layers;
    sf::Vector2f _scale;
    sf::FloatRect _size;
    sf::Vector2f _position;//note: not used yet!
    sf::Color _color; //note: not used yet!

public:
    GameMap();
    GameMap(int w, int h);
    virtual ~GameMap();

    //MapLayer& operator[](int layer);

    //void hack_a_map();

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
