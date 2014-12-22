#include "GameMap.h"
#include "ResourceManager.h"
#include "TileMap.h"


GameMapLayerBase::GameMapLayerBase()
{
    _tex_name = TEXT("");
    _data_name = TEXT("");
    _offset = sf::Vector2f(0.f, 0.f);
}

GameMapLayerBase::~GameMapLayerBase()
{

}

TiledGameMapLayer::TiledGameMapLayer()
{

}

TiledGameMapLayer::~TiledGameMapLayer()
{

}

void TiledGameMapLayer::prep()
{
    _tile_map = ResourceManager::get_tile_map(_tex_name, _data_name);
    _tile_map->setPosition(
        _offset.x * ResourceManager::scaling_factor().x,
        _offset.y  * ResourceManager::scaling_factor().y);

}

void TiledGameMapLayer::render(sf::RenderTarget & render_target)
{
    render_target.draw(*_tile_map);
}

SparseGameMapLayer::SparseGameMapLayer()
{

}

SparseGameMapLayer::~SparseGameMapLayer()
{

}

void SparseGameMapLayer::prep()
{
   //TODO
}

void SparseGameMapLayer::render(sf::RenderTarget & /*render_target*/)
{
    //TODO
}

GameMap::GameMap()
{
    _width = 0;
    _height = 0;

    _layers = std::vector<GameMapLayerBase*>();
}

GameMap::GameMap(int w, int h)
{
    _width = w;
    _height = h;
    _size = sf::FloatRect(0, 0, (float)w * 160.f, (float)h * 160.f);

    _layers = std::vector<GameMapLayerBase*>();
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

	
    scope->defs[TEXT("def_layer")] = [&](ScriptRaw* layer_raw){
        GameMapLayerBase * layer;
        if(layer_raw->vals != NULL && layer_raw->vals->vals == TEXT("sparse")){
            layer = new SparseGameMapLayer();
        }else{
            layer = new TiledGameMapLayer();
        }
        _layers.push_back(layer);

		auto local_scope = new ScriptScope();
		
        local_scope->defs[TEXT("def_texture")] = [layer](ScriptRaw*tex_raw){
            layer->_tex_name = tex_raw->vals->vals;
			return (ScriptScope*)NULL;
		};
		
        local_scope->defs[TEXT("def_data")] = [layer](ScriptRaw*data_raw){
            layer->_data_name = data_raw->vals->vals;
			return (ScriptScope*)NULL;
		};
		
        local_scope->defs[TEXT("def_offset")] = [layer](ScriptRaw*off_raw){
            layer->_offset.x = (float)off_raw->vals->vali();
            layer->_offset.y = (float)off_raw->vals->next->vali();
			return (ScriptScope*)NULL;
		};

		return local_scope;
    };

    return scope;
}

void GameMap::prep()
{
    for(auto layer : _layers)
    {
        layer->prep();
	}	
}

void GameMap::render(sf::RenderTarget & target)
{
    for(auto layer : _layers)
    {
        layer->render(target);
    }
}

void GameMap::set_scale(float x, float y)
{
    _scale = sf::Vector2f(x, y);
//TODO:
//    for(auto& layer : _layers)
//    {
//		layer->setScale(sf::Vector2f(
//			_scale.x * ResourceManager::scaling_factor().x,
//			_scale.y * ResourceManager::scaling_factor().y));
//    }
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
//TODO:
//	for (size_t i = 0; i < _layers.size(); i++)
//	{
//		auto position = _position + _layer_info[i].offset;
//		_layers[i]->setPosition(position.x * ResourceManager::scaling_factor().x, position.y  * ResourceManager::scaling_factor().y);
//	}
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
