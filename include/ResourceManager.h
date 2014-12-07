#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <unordered_map>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "string_t.h"
#include "ScriptDef.h"

class TextureWrapper
{
public:
	sf::Texture * texture;
	int sprite_width;
	int sprite_height;
};

class SpriteDef
{
public:
	string_t texture_name;
	int sheet_index_x;
	int sheet_index_y;
};

class ResourceManager
{
private:

	static ResourceManager * _inst;

	std::unordered_map<string_t, sf::Font *> _fonts;
	std::unordered_map<string_t, TextureWrapper> _textures;
	std::unordered_map<string_t, SpriteDef> _sprite_defs;
	std::unordered_map<string_t, sf::SoundBuffer *> _soundbuffers;
	std::unordered_map<string_t, ScriptDef *> _scripts;
	string_t _filepath;
	sf::Vector2f _scaling_factor;
	sf::Font * _default_font;

	ResourceManager();
	
	~ResourceManager();

	void load_texture_if_needed(string_t texture_name);

	void clean_impl();

public:

	static void init(string_t filepath);

	static sf::Sprite * get_texture(string_t texture);
	static sf::Sprite * get_sprite(string_t sprite_name);
	static sf::Sound * get_sound(string_t sound);
	static sf::Music * get_music(string_t music);
	static sf::Font * get_font(string_t font_name);
	static ScriptDef * get_script(string_t script_name);

	static void set_default_font(string_t font_name);
	

	static void clean();
};


#endif //__RESOURCE_MANAGER__
