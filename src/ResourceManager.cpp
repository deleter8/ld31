#include <iostream>
#include "ResourceManager.h"

ResourceManager * ResourceManager::_inst = new ResourceManager();

void ResourceManager::init(string_t filepath)
{
	_inst->_filepath = filepath;
}

sf::Sprite * ResourceManager::get_texture(string_t texture)
{
	_inst->load_texture_if_needed(texture);

	auto sprite = new sf::Sprite();
	sprite->setTexture(*_inst->_textures[texture].texture);
	sprite->setScale(_inst->_scaling_factor);
	return sprite;
}

sf::Sprite * ResourceManager::get_sprite(string_t sprite_name)
{
	
	auto sprite_def = _inst->_sprite_defs[sprite_name];
	_inst->load_texture_if_needed(sprite_def.texture_name);
	auto tex = _inst->_textures[sprite_def.texture_name];

	auto sprite = new sf::Sprite();
	sprite->setTexture(*tex.texture);
	auto start_x = sprite_def.sheet_index_x * tex.sprite_width;
	auto start_y = sprite_def.sheet_index_y * tex.sprite_height;
	auto width = tex.sprite_width * tex.sprite_width;
	auto height = tex.sprite_height * tex.sprite_height;
	sprite->setTextureRect(sf::IntRect(start_x, start_y, width, height));
	sprite->setScale(_inst->_scaling_factor);
	return sprite;
}

sf::Sound * ResourceManager::get_sound(string_t sound)
{
	if (_inst->_soundbuffers.find(sound) == _inst->_soundbuffers.end())
	{
		auto soundbuf = new sf::SoundBuffer();
		soundbuf->loadFromFile(ws2s(_inst->_filepath + sound + TEXT(".wav")));
		_inst->_soundbuffers[sound] = soundbuf;
	}

	auto snd = new sf::Sound();
	snd->setBuffer(*_inst->_soundbuffers[sound]);
	return snd;
}

sf::Music * ResourceManager::get_music(string_t music)
{
	auto muz = new sf::Music();
	muz->openFromFile(ws2s(_inst->_filepath + music + TEXT(".ogg")));
	return muz;
}

ScriptDef * ResourceManager::get_script(string_t script_name)
{
	return new ScriptDef(TEXT("TODO!!!"));
}

void ResourceManager::set_default_font(string_t font_name)
{
	_inst->_default_font = _inst->_fonts[font_name];
}

ResourceManager::ResourceManager()
{
	_filepath = TEXT("");
	
	_fonts = std::unordered_map<string_t, sf::Font *>();
	_textures = std::unordered_map<string_t, TextureWrapper> ();
	_sprite_defs = std::unordered_map<string_t, SpriteDef>();
	_soundbuffers = std::unordered_map<string_t, sf::SoundBuffer *>();
	_scripts = std::unordered_map<string_t, ScriptDef *>();

	_scaling_factor = sf::Vector2f(.4f, .4f);
}

ResourceManager::~ResourceManager()
{
	clean();
}

void ResourceManager::load_texture_if_needed(string_t texture_name)
{
	if (_textures[texture_name].texture == NULL)
	{
		auto tex = new sf::Texture();
		tex->loadFromFile(ws2s(_filepath + texture_name + TEXT(".png")));
		tex->setSmooth(true);
		_textures[texture_name].texture = tex;
	}
}

void ResourceManager::clean()
{
	_inst->clean_impl();
}

void ResourceManager::clean_impl()
{
	for (auto font : _fonts)
	{
		delete font.second;
	}
	_fonts.clear();

	for (auto texture : _textures)
	{
		delete texture.second.texture;
	}
	_textures.clear();
	
	_sprite_defs.clear();

	for (auto soundbuffer : _soundbuffers)
	{
		std::cout << "deleting " << ws2s(soundbuffer.first) << std::endl;
		delete soundbuffer.second;
	}
	_soundbuffers.clear();

	for (auto script : _scripts)
	{
		delete script.second;
	}
	_scripts.clear();

}