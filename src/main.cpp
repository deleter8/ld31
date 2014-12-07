#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "string_t.h"
#include "ResourceManager.h"
#include "Context.h"
#include "ContextManager.h"

int main()
{
	auto context_manager = new ContextManager();

	string_t asset_path = TEXT("../assets/");
	ResourceManager::init(asset_path);

	sf::RenderWindow * window = NULL;
	auto window_rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
	
	context_manager->add_action(TEXT("open_window"), [&](ActionVal val){
		int w = val.vali();
		int h = val.next->vali();
		string_t title = val.next->next->vals;
		std::replace(title.begin(), title.end(), '_', ' ');
		window = new sf::RenderWindow(sf::VideoMode(w, h), title);
		window_rect = sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)window->getSize());
		
		return ActionVal::EMPTY();
	});
	
	context_manager->add_action(TEXT("quit_game"), [&](ActionVal val){
		window->close(); 
		
		return ActionVal::EMPTY();
	});


	//start going away:
				auto menu_context = new Context();

				auto title_screen = ResourceManager::get_texture(TEXT("title"));
				auto title_options = ResourceManager::get_texture(TEXT("menu_options"));
				auto options_size = title_options->getLocalBounds();
				title_options->setOrigin(sf::Vector2f(options_size.width/2.f, options_size.height/2.f));
				title_options->setScale(sf::Vector2f(.5f, .5f));
				title_options->setPosition(512, 350);
	
				menu_context->add_render_object(title_screen);
				menu_context->add_render_object(title_options);

				auto menu_start_topleft = sf::Vector2i(450, 227);
				auto menu_start_bottomright = sf::Vector2i(575, 275);
				auto menu_start_rect = sf::IntRect(menu_start_topleft, menu_start_bottomright - menu_start_topleft);	
				menu_context->add_mouseclick_handler(menu_start_rect, [](MouseEvent ev){return true; });

				auto menu_settings_topleft = sf::Vector2i(411, 316);
				auto menu_settings_bottomright = sf::Vector2i(615, 373);
				auto menu_settings_rect = sf::IntRect(menu_settings_topleft, menu_settings_bottomright - menu_settings_topleft);
				menu_context->add_mouseclick_handler(menu_settings_rect, [](MouseEvent ev){return true; });

				auto menu_quit_topleft = sf::Vector2i(456, 405);
				auto menu_quit_bottomright = sf::Vector2i(569, 463);
				auto menu_quit_rect = sf::IntRect(menu_quit_topleft, menu_quit_bottomright - menu_quit_topleft);
				menu_context->add_mouseclick_handler(menu_quit_rect, [&window](MouseEvent ev){
					window->close();
					return true;
				});

	//end going away
	
    auto test_sound = ResourceManager::get_sound(TEXT("jump2"));

	auto music = ResourceManager::get_music(TEXT("robert_del_naja_HS"));
	bool button_pressed = false;
	music->play();

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window->close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if(test_sound->getStatus() != sf::Sound::Playing )test_sound->play();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			button_pressed = true;
		}
		else if (button_pressed)
		{
			button_pressed = false;
			
			auto coords = sf::Mouse::getPosition(*window);
			if (window_rect.contains(coords))
			{				
				std::cout << coords.x << ", " << coords.y << std::endl;
				menu_context->handle_mouseclick(coords.x, coords.y);
			}
		}
		
		window->clear();
		menu_context->render(*window);		
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