#ifndef __TILE_MAP__
#define __TILE_MAP__

#include <SFML/Graphics.hpp>

#include "string_t.h"


class TileMap : public sf::Drawable, public sf::Transformable
{
private:

	sf::VertexArray _vertices;
	sf::Texture * _tileset;
	unsigned int _width;
	unsigned int _height;

	bool computeGeometry(sf::Vector2u tile_size, const int* tiles);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:

	TileMap();
	~TileMap();
	
	bool loadFromFile(sf::Texture * tileset, sf::Vector2u tile_size, string_t filename);
	
	bool loadFromMemory(sf::Texture * tileset, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height);
	
	static void saveToFile(string_t filename, const int* tiles, unsigned int width, unsigned int height);
};

#endif //__TILE_MAP__