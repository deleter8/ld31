#include <algorithm>
#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "string_t.h"
#include "ResourceManager.h"
#include "Context.h"
#include "ContextManager.h"
#include "ScriptRunner.h"


void crawl_script(ScriptRaw * script, string_t prelim)
{
	std::cout << ws2s(prelim + script->line_def) << " ";
	ActionVal * vals = script->vals;
	while (vals->vals != TEXT(""))
	{
		std::cout << ws2s(vals->vals) << ",";
		if (vals->next == NULL) break;
		vals = vals->next.get();
	}
	std::cout << std::endl;

	for (auto line : script->inner_lines)
	{
		crawl_script(line, prelim + TEXT("->"));
	}
}

int main()
{
	auto context_manager = new ContextManager();
	auto script_runner = new ScriptRunner();

	string_t asset_path = TEXT("../assets/");
	ResourceManager::init(asset_path);

	sf::RenderWindow * window = NULL;
	auto window_rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));

	script_runner->add_def(TEXT("def_context"), [&](ScriptRaw* raw){
		auto context = new Context();
		auto local_scope = context->build_context(raw);
		context_manager->add_context(raw->vals->vals, context);
		return local_scope;
	});

	script_runner->add_def(TEXT("def_resource"), [&](ScriptRaw* raw){
		return ResourceManager::build_resource(raw);		
	});

	script_runner->add_action(TEXT("push_context"), [&](ActionVal * val){
		context_manager->push_context(val->vals);
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("pop_context"), [&](ActionVal * val){
		context_manager->pop_context();
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("set_default_font"), [](ActionVal * val){
		ResourceManager::set_default_font(val->vals);
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("open_window"), [&](ActionVal * val){
		int w = val->vali();
		int h = val->next->vali();
		string_t title = val->next->next->vals;
		std::replace(title.begin(), title.end(), '_', ' ');
		window = new sf::RenderWindow(sf::VideoMode(w, h), title);
		window_rect = sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)window->getSize());
		
		return ActionVal::EMPTY();
	});
	
	script_runner->add_action(TEXT("quit_game"), [&](ActionVal * val){
		window->close(); 
		
		return ActionVal::EMPTY();
	});

	auto script = ResourceManager::get_script(TEXT("main"));
	script_runner->run(script);
	
	//crawl_script(script, TEXT(""));

	////hacked in
	//window = new sf::RenderWindow(sf::VideoMode(1024, 576), TEXT("Dare teh thirty-first"));
	//window_rect = sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)window->getSize());

	//while (window == NULL)
	//{
	//	sf::sleep(sf::milliseconds(10));
	//}

	//start going away:
				//auto menu_context = new Context();

				//auto title_screen = ResourceManager::get_texture(TEXT("title"));
				//auto title_options = ResourceManager::get_texture(TEXT("menu_options"));
				//auto options_size = title_options->getLocalBounds();
				//title_options->setOrigin(sf::Vector2f(options_size.width/2.f, options_size.height/2.f));
				//title_options->setScale(sf::Vector2f(.5f, .5f));
				//title_options->setPosition(512, 350);
	
				//menu_context->add_render_object(title_screen);
				//menu_context->add_render_object(title_options);

				//auto menu_start_topleft = sf::Vector2i(450, 227);
				//auto menu_start_bottomright = sf::Vector2i(575, 275);
				//auto menu_start_rect = sf::IntRect(menu_start_topleft, menu_start_bottomright - menu_start_topleft);	
				//menu_context->add_mouseclick_handler(menu_start_rect, [](MouseEvent ev){return true; });

				//auto menu_settings_topleft = sf::Vector2i(411, 316);
				//auto menu_settings_bottomright = sf::Vector2i(615, 373);
				//auto menu_settings_rect = sf::IntRect(menu_settings_topleft, menu_settings_bottomright - menu_settings_topleft);
				//menu_context->add_mouseclick_handler(menu_settings_rect, [](MouseEvent ev){return true; });

				//auto menu_quit_topleft = sf::Vector2i(456, 405);
				//auto menu_quit_bottomright = sf::Vector2i(569, 463);
				//auto menu_quit_rect = sf::IntRect(menu_quit_topleft, menu_quit_bottomright - menu_quit_topleft);
				//menu_context->add_mouseclick_handler(menu_quit_rect, [&window](MouseEvent ev){
				//	window->close();
				//	return true;
				//});

	//end going away
	
    auto test_sound = ResourceManager::get_sound(TEXT("jump2"));

	auto music = ResourceManager::get_music(TEXT("robert_del_naja_HS"));
	bool button_pressed = false;
	music->play();

	auto key_pressed = std::unordered_map<sf::Keyboard::Key, bool, std::hash<int> >();
	for (auto key : context_manager->Keys())
	{
		key_pressed[key] = false;
	}

	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		for (auto key : context_manager->Keys())
		{
			if (sf::Keyboard::isKeyPressed(key))
			{
				context_manager->handle_keyheld(key);
				key_pressed[key] = true;
			}
			else if (key_pressed[key])
			{
				auto handled = context_manager->handle_keypress(key);
				key_pressed[key] = false;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if(test_sound->getStatus() != sf::Sound::Playing )test_sound->play();
		}

		auto coords = sf::Mouse::getPosition(*window);
		bool in_window = window_rect.contains(coords);
		coords.x = coords.x / ResourceManager::scaling_factor().x;
		coords.y = coords.y / ResourceManager::scaling_factor().y;
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (in_window) context_manager->handle_mousedown(coords.x, coords.y);
			button_pressed = true;
		}
		else if (button_pressed)
		{
			button_pressed = false;
			//todo: enhance with coords where button went down for drag and such
			if (in_window) context_manager->handle_mouseclick(coords.x, coords.y);
			if (in_window) std::cout << coords.x << ", " << coords.y << std::endl;
		}
		
		window->clear();
		context_manager->render(*window);
		window->display();
		sf::sleep(sf::milliseconds(1));
	}

	delete test_sound;

	music->stop();

	delete music;
	music = NULL;

	ResourceManager::clean();

	delete window;

	//context:
		//view
		//input
		//resources - text, images, sounds, music
	std::cout << "all done" << std::endl;
	return 0;
}
