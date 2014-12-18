#include "GameMapManager.h"


GameMapManager * GameMapManager::_inst = new GameMapManager();

GameMapManager::GameMapManager()
{

}

GameMap * GameMapManager::get_map(string_t map_name)
{
	if (_inst->_maps.find(map_name) != _inst->_maps.end())
	{
		return _inst->_maps[map_name];
	}

	return NULL;
}

void GameMapManager::add_map(string_t map_name, GameMap * map)
{
	_inst->_maps[map_name] = map;
}