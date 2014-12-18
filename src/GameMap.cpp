#include "GameMap.h"
#include "ResourceManager.h"
#include "TileMap.h"


MapLayerInfo::MapLayerInfo()
{
	data_name = TEXT("");
	texture_name = TEXT("");
	offset = sf::Vector2f(0.f, 0.f);
}

GameMap::GameMap()
{
    _width = 0;
    _height = 0;

    _layers = std::vector<TileMap*>();
}

GameMap::GameMap(int w, int h)
{
    _width = w;
    _height = h;
    _size = sf::FloatRect(0, 0, (float)w * 160.f, (float)h * 160.f);

    _layers = std::vector<TileMap*>();
}

GameMap::~GameMap()
{

}

ScriptScope * GameMap::build_map(ScriptRaw *)
{
    auto scope = new ScriptScope();
    scope->defs[TEXT("def_attrib")] = [&](ScriptRaw*){
        return (ScriptScope*)NULL;
    };

	
    scope->defs[TEXT("def_layer")] = [&](ScriptRaw*){
		_layer_info.push_back(MapLayerInfo());

		auto layer_info = &_layer_info.back();
		auto local_scope = new ScriptScope();
		
		local_scope->defs[TEXT("def_texture")] = [layer_info](ScriptRaw*tex_raw){
			layer_info->texture_name = tex_raw->vals->vals;
			return (ScriptScope*)NULL;
		};
		
		local_scope->defs[TEXT("def_data")] = [layer_info](ScriptRaw*data_raw){
			layer_info->data_name = data_raw->vals->vals;
			return (ScriptScope*)NULL;
		};
		
		local_scope->defs[TEXT("def_offset")] = [layer_info](ScriptRaw*off_raw){
			layer_info->offset.x = (float)off_raw->vals->vali();
			layer_info->offset.y = (float)off_raw->vals->next->vali();
			return (ScriptScope*)NULL;
		};

		return local_scope;
    };

    return scope;
}

void GameMap::prep()
{
    for(auto info : _layer_info)
    {
		auto layer = ResourceManager::get_tile_map(info.texture_name, info.data_name);
		layer->setPosition(
			info.offset.x * ResourceManager::scaling_factor().x,
			info.offset.y  * ResourceManager::scaling_factor().y);
		_layers.push_back(layer);
	}	
}

void GameMap::render(sf::RenderTarget & target)
{
    for(auto& layer : _layers)
    {
        target.draw(*layer);
    }
}

void GameMap::set_scale(float x, float y)
{
    _scale = sf::Vector2f(x, y);

    for(auto& layer : _layers)
    {
		layer->setScale(sf::Vector2f(
			_scale.x * ResourceManager::scaling_factor().x,
			_scale.y * ResourceManager::scaling_factor().y));
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
    _position = sf::Vector2f(x, y);

	for (size_t i = 0; i < _layers.size(); i++)
	{
		auto position = _position + _layer_info[i].offset;
		_layers[i]->setPosition(position.x * ResourceManager::scaling_factor().x, position.y  * ResourceManager::scaling_factor().y);
	}
}

const sf::Vector2f& GameMap::get_position() const
{
    return _position;
}

void GameMap::set_color(sf::Color c)
{
    _color = c;//todo
}

const sf::Color& GameMap::get_color() const
{
    return _color;
}
