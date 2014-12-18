#ifndef __GAME_MAP_MANAGER__
#define __GAME_MAP_MANAGER__

#include <unordered_map>

#include "string_t.h"
#include "GameMap.h"


class GameMapManager
{
private:
	std::unordered_map<string_t, GameMap*> _maps;
	static GameMapManager * _inst;

	GameMapManager();
public:
	static GameMap * get_map(string_t map_name);

	static void add_map(string_t map_name, GameMap* map);
};

#endif //__GAME_MAP_MANAGER__