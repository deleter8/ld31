#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <string>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class ResourceManager
{
private:

	static ResourceManager * _inst;

	std::unordered_map<std::string, sf::Texture *> _textures;
	std::unordered_map<std::string, sf::SoundBuffer *> _soundbuffers;
	std::string _filepath;
	int _sprite_width;
	int _sprite_height;	
	
	ResourceManager(std::string filepath, int sWidth, int sHeight);
	
	~ResourceManager();

	void load_texture_if_needed(std::string texture);

	void clean_impl();

public:

	static sf::Sprite * get_texture(std::string texture);

	static sf::Sprite * get_sprite(std::string texture, int x, int y, int h = 1, int w = 1);

	static sf::Sound * get_sound(std::string sound);

	static sf::Music * get_music(std::string music);

	static void clean();
};


#endif
