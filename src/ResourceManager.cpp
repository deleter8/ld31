#include <iostream>
#include "ResourceManager.h"

ResourceManager * ResourceManager::_inst = new ResourceManager("../assets/", 32, 32);

sf::Sprite * ResourceManager::get_texture(std::string texture)
{
	_inst->load_texture_if_needed(texture);

	auto sprite = new sf::Sprite();
	sprite->setTexture(*_inst->_textures[texture]);
	return sprite;
}

sf::Sprite * ResourceManager::get_sprite(std::string texture, int x, int y, int h, int w)
{
	_inst->load_texture_if_needed(texture);

	auto sprite = new sf::Sprite();
	sprite->setTexture(*_inst->_textures[texture]);
	auto start_x = x*_inst->_sprite_width;
	auto start_y = y*_inst->_sprite_height;
	auto width = _inst->_sprite_width * w;
	auto height = _inst->_sprite_height * h;
	sprite->setTextureRect(sf::IntRect(start_x, start_y, width, height));
	return sprite;
}

sf::Sound * ResourceManager::get_sound(std::string sound)
{
	if (_inst->_soundbuffers.find(sound) == _inst->_soundbuffers.end())
	{
		auto soundbuf = new sf::SoundBuffer();
		soundbuf->loadFromFile(_inst->_filepath + sound + ".wav");
		_inst->_soundbuffers[sound] = soundbuf;
	}

	auto snd = new sf::Sound();
	snd->setBuffer(*_inst->_soundbuffers[sound]);
	return snd;
}

sf::Music * ResourceManager::get_music(std::string music)
{
	auto muz = new sf::Music();
	muz->openFromFile(_inst->_filepath + music + ".ogg");
	return muz;
}

ResourceManager::ResourceManager(std::string filepath, int sWidth, int sHeight)
{
	_filepath = filepath;
	_textures = std::unordered_map<std::string, sf::Texture *>();
	_soundbuffers = std::unordered_map<std::string, sf::SoundBuffer*>();
	_sprite_height = sHeight;
	_sprite_width = sWidth;
}

ResourceManager::~ResourceManager()
{
	clean();
}

void ResourceManager::load_texture_if_needed(std::string texture)
{
	if (_textures.find(texture) == _textures.end())
	{
		auto tex = new sf::Texture();
		tex->loadFromFile(_filepath + texture + ".png");
		_textures[texture] = tex;
	}
}

void ResourceManager::clean()
{
	_inst->clean_impl();
}

void ResourceManager::clean_impl()
{
	for (auto it = _textures.begin(); it != _textures.end(); it++)
	{
		delete (*it).second;
	}
	_textures.clear();

	for (auto it = _soundbuffers.begin(); it != _soundbuffers.end(); it++)
	{
		std::cout << "deleting " << it->first << std::endl;
		delete (*it).second;
	}
	_soundbuffers.clear();

}