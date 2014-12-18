#include <fstream>

#include "TileMap.h"


bool TileMap::computeGeometry(sf::Vector2u tile_size, const int* tiles)
{
	_vertices.clear();

	int count = 0;
	for (unsigned int i = 0; i < _width; ++i)	
		for (unsigned int j = 0; j < _height; ++j)		
			if (tiles[i + j * _width] != -1) count++;

	// resize the vertex array to fit the level size
	_vertices.setPrimitiveType(sf::Quads);
	_vertices.resize(count * 4);

	count = 0;
	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < _width; ++i)
	{
		for (unsigned int j = 0; j < _height; ++j)
		{
			// get the current tile number
			int tileNumber = tiles[i + j * _width];
			if (tileNumber == -1) continue;

			// find its position in the tileset texture
			int tu = tileNumber % (_tileset->getSize().x / tile_size.x);
			int tv = tileNumber / (_tileset->getSize().x / tile_size.x);

			// get a pointer to the current tile's quad
			sf::Vertex* quad = &_vertices[count * 4];
			count++;

			// define its 4 corners
			quad[0].position = sf::Vector2f((float)i * tile_size.x, (float)j * tile_size.y);
			quad[1].position = sf::Vector2f((float)(i + 1) * tile_size.x, (float)j * tile_size.y);
			quad[2].position = sf::Vector2f((float)(i + 1) * tile_size.x, (float)(j + 1) * tile_size.y);
			quad[3].position = sf::Vector2f((float)i * tile_size.x, (float)(j + 1) * tile_size.y);

			// define its 4 texture coordinates
			quad[0].texCoords = sf::Vector2f((float)tu * tile_size.x, (float)tv * tile_size.y);
			quad[1].texCoords = sf::Vector2f((float)(tu + 1) * tile_size.x, (float)tv * tile_size.y);
			quad[2].texCoords = sf::Vector2f((float)(tu + 1) * tile_size.x, (float)(tv + 1) * tile_size.y);
			quad[3].texCoords = sf::Vector2f((float)tu * tile_size.x, (float)(tv + 1) * tile_size.y);
		}
	}

	return true;
}

TileMap::TileMap()
{
	_tileset = NULL;
	_width = 0;
	_height = 0;
}

TileMap::~TileMap()
{
	_vertices.clear();
	_tileset = NULL;
}

bool TileMap::loadFromMemory(sf::Texture * tileset, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height)
{
	_tileset = tileset;
	_width = width;
	_height = height;
	return computeGeometry(tile_size, tiles);
}

bool TileMap::loadFromFile(sf::Texture * tileset, sf::Vector2u tile_size, string_t filename)
{	
	int * map_data = NULL;
	{
		std::ifstream infile(filename, std::ios_base::binary);
		if (infile)
		{
			infile.read((char*)&_width, sizeof(int));
			infile.read((char*)&_height, sizeof(int));
			auto length = _width * _height * sizeof(int);
			map_data = (int*)malloc(length);
			infile.read((char*)map_data, length);
			infile.close();
		}
		else
		{
			std::cout << "could not find data file " << ws2s(filename) << std::endl;
			return false;
		}
	}

	return loadFromMemory(tileset, tile_size, map_data, _width, _height);

}

void TileMap::saveToFile(string_t filename, const int* tiles, unsigned int width, unsigned int height)
{
	std::ofstream outfile(filename, std::ios_base::binary);

	outfile.write((char*)&width, sizeof(int));
	outfile.write((char*)&height, sizeof(int));

	auto length = width * height * sizeof(int);
	
	outfile.write((char*)tiles, length);
	outfile.close();
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// apply the tileset texture
	states.texture = _tileset;

	// draw the vertex array
	target.draw(_vertices, states);
}
