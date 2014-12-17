#ifndef __CONTEXT_______
#define __CONTEXT_______

#include <list>
#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "ScriptScope.h"
#include "ScriptRaw.h"
#include "IRenderObject.h"

enum MouseEvent
{
	MOUSE_DOWN,
	MOUSE_CLICK
};

class Context
{
private:

	string_t _name;
    std::list<IRenderObject*> _draw_list;
	sf::Music * _music;

    sf::FloatRect _context_dimensions;

	bool has_lingering_mouseclick_handler;
	std::function<bool(MouseEvent)> lingering_mouseclick_handler;
	bool has_lingering_mousedown_handler;
	std::function<bool(MouseEvent)> lingering_mousedown_handler;
	bool _only_handle_when_top_context;
	string_t _music_thing;

	std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mouseclick_handlers;
	std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mousedown_handlers;
	std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keypress_handlers;
	std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keyheld_handlers;

	std::list<std::pair<string_t, std::function<bool()>>> _named_event_handlers;

	std::list<sf::Keyboard::Key> _keys;

	std::list<Context*> _inner_elements;
	std::unordered_map<string_t, Context*> _inner_element_lookup;

    void add_inner_element(string_t name, Context * element);

	void step_scale(float val);
	void step_scalex(float val);
	void step_scaley(float val);
	void step_opacity(float val);
	void step_volume(float val);

public:

	Context(string_t name);

	void add_mouseclick_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler);
	void add_mousedown_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler);
	void add_keypress_handler(sf::Keyboard::Key key, std::function<bool()> handler);
	void add_keyheld_handler(sf::Keyboard::Key key, std::function<bool()> handler);
	void add_named_event_handler(string_t event_name, std::function<bool()> handler);

	bool handle_mouseclick(int x, int y);
	bool handle_mousedown(int x, int y);
	bool handle_keypress(sf::Keyboard::Key key);
	bool handle_keyheld(sf::Keyboard::Key key);

	bool handle_named_event(string_t event_name);//todo: probably add some optional args?

	const std::list<sf::Keyboard::Key> Keys();

	void render(sf::RenderWindow& window);

	ScriptScope * build_context(ScriptRaw *);
	void prep();
	const bool& only_handle_when_top_context();
	
	std::function<void(float)> get_step(string_t attrib);
	const string_t& get_name();
	Context * get_inner_element(string_t name);
};

#endif //__CONTEXT_______
