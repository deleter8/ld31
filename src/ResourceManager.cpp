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
	
	int width = tex.texture->getSize().x;
	int height = tex.texture->getSize().y;

	if (tex.sprite_width != 0 && tex.sprite_height != 0 && !sprite_def.abs_coords)
	{
		auto start_x = sprite_def.sheet_index_x * tex.sprite_width;
		auto start_y = sprite_def.sheet_index_y * tex.sprite_height;
		width = tex.sprite_width * sprite_def.sheet_width;
		height = tex.sprite_height * sprite_def.sheet_height;
		sprite->setTextureRect(sf::IntRect(start_x, start_y, width, height));
	}
	else if (sprite_def.abs_coords)
	{
		width = sprite_def.sheet_width - sprite_def.sheet_index_x;
		height = sprite_def.sheet_height - sprite_def.sheet_index_y;
		sprite->setTextureRect(sf::IntRect(sprite_def.sheet_index_x, sprite_def.sheet_index_y, width, height));
	}
	
	if (sprite_def.center_origin)
	{
		sprite->setOrigin(sf::Vector2f(width / 2.f, height / 2.f));
	}

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

sf::Text * ResourceManager::get_text(string_t id)
{
	return new sf::Text(_inst->_text[id], *_inst->_default_font, (int)(30.f/_inst->_scaling_factor.x));
}

ScriptRaw * ResourceManager::get_script(string_t script_name)
{
	if (_inst->_scripts.find(script_name) == _inst->_scripts.end())
	{
		_inst->_scripts[script_name] = load_script(_inst->_filepath + script_name + TEXT(".script"));
	}
	
	return _inst->_scripts[script_name];
}

sf::Font * ResourceManager::get_font(string_t font_name)
{
	if (_inst->_fonts.find(font_name) == _inst->_fonts.end())
	{
		auto font = new sf::Font();
		font->loadFromFile(ws2s(_inst->_filepath + font_name + TEXT(".ttf")));
		_inst->_fonts[font_name] = font;
	}
	
	return _inst->_fonts[font_name];
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
	_scripts = std::unordered_map<string_t, ScriptRaw *>();

	_internal_res = sf::Vector2i(2560, 1440);
	_scaling_factor = sf::Vector2f(1, 1);
}

void ResourceManager::set_screen_res(int x, int y)
{
	_inst->_scaling_factor = sf::Vector2f(float(x) / (float)_inst->_internal_res.x,
		                                  (float)y / (float)_inst->_internal_res.y);
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
		tex->setSmooth(_textures[texture_name].smooth);
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

ScriptScope * ResourceManager::build_resource(ScriptRaw * raw)
{
	auto scope = new ScriptScope();

	scope->statements = NULL; //error b/c this doesn't make sense, there shouldn't be actions in the def-scope of context
	string_t resource_type = raw->vals->vals;

	if (resource_type == TEXT("font"))
	{
		get_font(raw->vals->next->vals);
	}
	else if (resource_type == TEXT("text"))
	{
		_inst->_text[raw->vals->next->vals] = raw->vals->next->next->vals;
	}
	else if (resource_type == TEXT("sound"))
	{
		get_sound(raw->vals->next->vals);
	}
	else if (resource_type == TEXT("texture"))
	{
		TextureWrapper * texture_wrapper = &_inst->_textures[raw->vals->next->vals];
		texture_wrapper->sprite_width = 0;
		texture_wrapper->sprite_height = 0;
		texture_wrapper->smooth = true;
		_inst->load_texture_if_needed(raw->vals->next->vals);

		scope->defs[TEXT("def_attrib")] = [texture_wrapper](ScriptRaw* raw){
			string_t attrib_name = raw->vals->vals;
			if (attrib_name == TEXT("sprite_dim"))
			{
				auto w = raw->vals->next->vali();
				auto h = raw->vals->next->next->vali();
				
				texture_wrapper->sprite_width = w;
				texture_wrapper->sprite_height = h;
			}
			else if (attrib_name == TEXT("smooth"))
			{
				string_t sval = raw->vals->next->vals;
				if (sval == TEXT("true"))
				{
					texture_wrapper->smooth = true;
				}
				else if (sval == TEXT("false"))
				{
					texture_wrapper->smooth = false;
				}
				else
				{
					texture_wrapper->smooth = raw->vals->next->vali()!=0;
				}

				if (texture_wrapper->texture != NULL){
					texture_wrapper->texture->setSmooth(texture_wrapper->smooth);
				}
			}
			return (ScriptScope*)NULL;
		};
	}
	else if (resource_type == TEXT("sprite"))
	{
		_inst->_sprite_defs[raw->vals->next->vals] = SpriteDef();
		SpriteDef * sprite_def = &_inst->_sprite_defs[raw->vals->next->vals];
		sprite_def->center_origin = false;
		sprite_def->sheet_width = 1;
		sprite_def->sheet_height = 1;
		sprite_def->sheet_index_x = 0;
		sprite_def->sheet_index_y = 0;
		sprite_def->abs_coords = false;

		scope->defs[TEXT("def_attrib")] = [sprite_def](ScriptRaw* raw){
			string_t attrib_name = raw->vals->vals;
			if (attrib_name == TEXT("texture"))
			{
				auto texture_name = raw->vals->next->vals;
				sprite_def->texture_name = texture_name;

				if (raw->vals->next->next != NULL && raw->vals->next->next->vals != TEXT(""))
				{
					auto idx = raw->vals->next->next->vali();
					auto idy = raw->vals->next->next->next->vali();

					sprite_def->sheet_index_x = idx;
					sprite_def->sheet_index_y = idy;

					if (raw->vals->next->next->next->next != NULL && raw->vals->next->next->next->next->vals != TEXT(""))
					{
						sprite_def->sheet_width = raw->vals->next->next->next->next->vali();
						sprite_def->sheet_height = raw->vals->next->next->next->next->next->vali();
					}
				}
			}
			else if (attrib_name == TEXT("center_origin"))
			{
				sprite_def->center_origin = true;
			}
			else if (attrib_name == TEXT("raw_coords"))
			{
				sprite_def->abs_coords = true;
				
				// *trollface*, yes, I'm reusing variables against their names
				sprite_def->sheet_index_x = raw->vals->next->vali();
				sprite_def->sheet_index_y = raw->vals->next->next->vali();
				sprite_def->sheet_width =   raw->vals->next->next->next->vali();
				sprite_def->sheet_height =  raw->vals->next->next->next->next->vali();
			}
			return (ScriptScope*)NULL;
		};
	}
	return scope;
}

const sf::Vector2f& ResourceManager::scaling_factor()
{
	return _inst->_scaling_factor;
}