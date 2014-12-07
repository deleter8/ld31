#ifndef __CONTEXT_______
#define __CONTEXT_______

#include <list>
#include <functional>

#include <SFML/Graphics.hpp>

enum MouseEvent
{
	MOUSE_DOWN,
	MOUSE_CLICK
};

class Context
{
private:

	std::list<sf::Drawable*> _draw_list;

	std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mouseclick_handlers;
	std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mousedown_handlers;
	std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keypress_handlers;
	std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keyheld_handlers;

public:

	Context();

	void add_render_object(sf::Drawable * object);

	void add_mouseclick_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler);
	void add_mousedown_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler);
	void add_keypress_handler(sf::Keyboard::Key key, std::function<bool()> handler);
	void add_keyheld_handler(sf::Keyboard::Key key, std::function<bool()> handler);

	bool handle_mouseclick(int x, int y);
	bool handle_mousedown(int x, int y);
	bool handle_keypress(sf::Keyboard::Key key);
	bool handle_keyheld(sf::Keyboard::Key key);

	void render(sf::RenderWindow& window);
};

#endif __CONTEXT_______