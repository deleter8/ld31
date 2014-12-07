#ifndef __CONTEXT_______
#define __CONTEXT_______

#include <list>
#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "ScriptScope.h"
#include "ScriptRaw.h"

enum MouseEvent
{
	MOUSE_DOWN,
	MOUSE_CLICK
};

class Context
{
private:

	std::list<sf::Drawable*> _draw_list;
	sf::Sprite * _first_sprite;
	

	std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mouseclick_handlers;
	std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mousedown_handlers;
	std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keypress_handlers;
	std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keyheld_handlers;

	std::list<sf::Keyboard::Key> _keys;

	std::list<Context*> _inner_elements;
	std::unordered_map<string_t, Context*> _inner_element_lookup;

	void add_inner_element(string_t name, Context * element);

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

	const std::list<sf::Keyboard::Key> Keys();

	void render(sf::RenderWindow& window);

	ScriptScope * build_context(ScriptRaw *);
};

#endif //__CONTEXT_______