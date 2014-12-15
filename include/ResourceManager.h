#ifndef __RESOURCE_MANAGER__
#define __RESOURCE_MANAGER__

#include <unordered_map>
#include <list>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "string_t.h"
#include "ScriptRaw.h"
#include "ScriptScope.h"
#include "GameMap.h"

class TextureWrapper
{
public:
	sf::Texture * texture;
	int sprite_width;
	int sprite_height;
	bool smooth;
};

class SpriteDef
{
public:
	string_t texture_name;
	int sheet_index_x;
	int sheet_index_y;
	int sheet_width;
	int sheet_height;
	bool center_origin;
	bool abs_coords;
};

class ResourceManager
{
private:

	static ResourceManager * _inst;

	std::unordered_map<string_t, sf::Font *> _fonts;
	std::unordered_map<string_t, TextureWrapper> _textures;
	std::unordered_map<string_t, SpriteDef> _sprite_defs;
	std::unordered_map<string_t, sf::SoundBuffer *> _soundbuffers;
	std::unordered_map<string_t, ScriptRaw *> _scripts;
	std::unordered_map<string_t, string_t> _text;
	std::unordered_map<string_t, string_t> _music_defs;
    std::unordered_map<string_t, GameMap*> _maps;

	string_t _filepath;
	sf::Vector2f _scaling_factor;
	sf::Vector2i _internal_res;
	sf::Font * _default_font;

	float _global_volume;
	float _sound_volume;
	float _music_volume;


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
	static ScriptRaw * get_script(string_t script_name);
	static sf::Text * get_text(string_t id);
    static GameMap * get_map(string_t map_name);

	static void set_default_font(string_t font_name);
	
	static ScriptScope * build_resource(ScriptRaw *);

	static const sf::Vector2f& scaling_factor();
	static float music_volume();
	static float sound_volume();
	static float& raw_music_volume();
	static float& raw_sound_volume();
	static const float& global_volume();

	static void set_screen_res(int x, int y);
	static void set_global_volume(float v);	

	static void clean();
};


#endif //__RESOURCE_MANAGER__
