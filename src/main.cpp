#include <iostream>
#include <SFML/Graphics.hpp>

#include "ResourceManager.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 600), "Dare the Thirty-First");
	
	auto title_screen = ResourceManager::get_texture("title");

	auto test_sound = ResourceManager::get_sound("jump2");

	auto music = ResourceManager::get_music("robert_del_naja_HS");

	music->play();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			if(test_sound->getStatus() != sf::Sound::Playing )test_sound->play();
		}

		window.clear();
		window.draw(*title_screen);
		window.display();
		sf::sleep(sf::milliseconds(1));
	}

	delete test_sound;

	music->stop();

	delete music;
	music = NULL;

	ResourceManager::clean();

	//context:
		//view
		//input
		//resources - text, images, sounds, music
	std::cout << "all done" << std::endl;
	return 0;
}