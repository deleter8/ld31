#include "GameMap.h"
#include "ResourceManager.h"

MapColumnBuilder::MapColumnBuilder(MapLayer &layer, int column) : _layer(layer)
{
    _column = column;
}

MapTileBuilder MapColumnBuilder::operator [](int row)
{
    return MapTileBuilder(_layer, _column, row);
}

MapTileBuilder::MapTileBuilder(MapLayer &layer, int column, int row) : _layer(layer)
{
    _column = column;
    _row = row;
}

MapTileBuilder& MapTileBuilder::operator =(int tile)
{
    _layer._sprite_info[_column][_row] = ResourceManager::get_sprite(_layer._sprite_names[tile]);
	_layer._sprite_info[_column][_row]->setPosition((float)_column * 160.f * ResourceManager::scaling_factor().x, (float)_row * 160.f * ResourceManager::scaling_factor().y);
	if (_layer._scale.x > 0)
	{
		_layer._sprite_info[_column][_row]->setScale(_layer._scale.x * ResourceManager::scaling_factor().x, _layer._scale.y * ResourceManager::scaling_factor().y);
	}

    _layer.tiles[_column][_row] = tile;
    return *this;
}

MapLayer::MapLayer(int w, int h)
{
    tiles = std::vector<std::vector<int>>(w);
	_sprite_info = std::vector<std::vector<sf::Sprite*>>(w);
    for(int i = 0; i < w; i++)
    {
        tiles[i] = std::vector<int>(h);
		_sprite_info[i] = std::vector<sf::Sprite*>(h);
    }
    _sprite_names = std::vector<string_t>(256);
    _width = w;
    _height = h;
	_scale = sf::Vector2f(0.f, 0.f);
}

MapColumnBuilder MapLayer::operator [](int column)
{
    return MapColumnBuilder(*this, column);
}

void MapLayer::prep()
{
    for(int x = 0; x < _width; x++)
    {
        for(int y = 0; y < _height; y++)
        {
            _sprite_info[x][y] = ResourceManager::get_sprite(_sprite_names[tiles[x][y]]);
			_sprite_info[x][y]->setPosition((float)x * 160.f * ResourceManager::scaling_factor().x, (float)y * 160.f * ResourceManager::scaling_factor().y);
			auto pos = _sprite_info[x][y]->getPosition();
			_sprite_info[x][y]->setScale(_scale.x, _scale.y);
        }
    }
}

void MapLayer::set_scale(sf::Vector2f scale)
{
    _scale = scale;

    for(int c = 0; c < _width; c++)
    {
        for(int r = 0; r < _height; r++)
        {
            _sprite_info[c][r]->setScale(_scale);
        }
    }
}

GameMap::GameMap()
{
    _width = 0;
    _height = 0;

    _layers = std::vector<MapLayer>();
}

GameMap::GameMap(int w, int h)
{
    _width = w;
    _height = h;
    _size = sf::FloatRect(0, 0, (float)w * 160.f, (float)h * 160.f);

    _layers = std::vector<MapLayer>();
}

GameMap::~GameMap()
{

}

void MapLayer::render(sf::RenderTarget & target)
{
    for(int x = 0; x < _width; x++)
        for(int y = 0; y < _height; y++)
            target.draw(*_sprite_info[x][y]);
}

ScriptScope * MapLayer::build_layer(ScriptRaw *)
{
    auto scope = new ScriptScope();
    scope->defs[TEXT("def_tile")] = [&](ScriptRaw* tile_raw){
        auto idx = tile_raw->vals->vali();
        string_t sprite_name = tile_raw->vals->next->vals;

        _sprite_names[idx] = sprite_name;

        return (ScriptScope*)NULL;
    };

    return scope;
}

MapLayer& GameMap::operator [](int layer)
{
    return _layers[layer];
}

void GameMap::hack_a_map()
{
    for (int bx = 0; bx < 16; bx+=4)
    {
        for(int by = 0; by < 12; by+=4)
        {
            _layers[0][bx + 0][by + 0] = 2;
            _layers[0][bx + 1][by + 0] = 0;
            _layers[0][bx + 2][by + 0] = 0;
            _layers[0][bx + 3][by + 0] = 0;

            _layers[0][bx + 0][by + 1] = 3;
            _layers[0][bx + 1][by + 1] = 5;
            _layers[0][bx + 2][by + 1] = 5;
            _layers[0][bx + 3][by + 1] = 5;

            _layers[0][bx + 0][by + 2] = 3;
            _layers[0][bx + 1][by + 2] = 5;
            _layers[0][bx + 2][by + 2] = 5;
            _layers[0][bx + 3][by + 2] = 5;

            _layers[0][bx + 0][by + 3] = 3;
            _layers[0][bx + 1][by + 3] = 5;
            _layers[0][bx + 2][by + 3] = 5;
            _layers[0][bx + 3][by + 3] = 5;
        }
    }
}

ScriptScope * GameMap::build_map(ScriptRaw *)
{
    auto scope = new ScriptScope();
    scope->defs[TEXT("def_attrib")] = [&](ScriptRaw* attrib_raw){
        string_t attrib_name = attrib_raw->vals->vals;
        if (attrib_name == TEXT("size"))
        {
            auto w = attrib_raw->vals->next->vali();
            auto h = attrib_raw->vals->next->next->vali();
            _width = w;
            _height = h;
            _size = sf::FloatRect(0.f, 0.f, (float)w * 160.f, (float)h * 160.f);
        }
        return (ScriptScope*)NULL;
    };

    scope->defs[TEXT("def_layer")] = [&](ScriptRaw*layer_raw){
        _layers.push_back(MapLayer(_width, _height));
        return _layers.back().build_layer(layer_raw);
    };

    return scope;
}

void GameMap::prep()
{
    for(auto& layer : _layers)
    {
        layer.prep();
	}
	hack_a_map();
}

void GameMap::render(sf::RenderTarget & target)
{
    for(auto& layer : _layers)
    {
        layer.render(target);
    }
}

void GameMap::set_scale(float x, float y)
{
    _scale = sf::Vector2f(x, y);

    for(auto& layer : _layers)
    {
        layer.set_scale(_scale);
    }
}

const sf::Vector2f& GameMap::get_scale() const
{
    return _scale;
}

sf::FloatRect GameMap::get_bounds() const
{
    return _size;
}

void GameMap::set_position(float x, float y)
{
    //todo: move map at some point
    _position = sf::Vector2f(x, y);
}

const sf::Vector2f& GameMap::get_position() const
{
    return _position;
}

void GameMap::set_color(sf::Color c)
{
    _color = c;
}

const sf::Color& GameMap::get_color() const
{
    return _color;
}
