#include "Context.h"

Context::Context()
{
	_mouseclick_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
	_mousedown_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
	_keypress_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
	_keyheld_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
}

void Context::add_render_object(sf::Drawable * object)
{
	_draw_list.push_back(object);
}

void Context::add_mouseclick_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler)
{
	_mouseclick_handlers.push_back(std::pair<sf::IntRect, std::function<bool(MouseEvent)>>(region, handler));
}

void Context::add_mousedown_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler)
{
	_mousedown_handlers.push_back(std::pair<sf::IntRect, std::function<bool(MouseEvent)>>(region, handler));
}

void Context::add_keypress_handler(sf::Keyboard::Key key, std::function<bool()> handler)
{
	_keypress_handlers.push_back(std::pair<sf::Keyboard::Key, std::function<bool()>>(key, handler));
}

void Context::add_keyheld_handler(sf::Keyboard::Key key, std::function<bool()> handler)
{
	_keyheld_handlers.push_back(std::pair<sf::Keyboard::Key, std::function<bool()>>(key, handler));
}


bool Context::handle_mouseclick(int x, int y)
{
	for (auto it : _mouseclick_handlers)
	{
		if (it.first.contains(sf::Vector2<int>(x, y)))
		{
			if (it.second(MOUSE_CLICK)) return true;
		}
	}

	return false;
}

bool Context::handle_mousedown(int x, int y)
{
	for (auto it : _mousedown_handlers)
	{
		if (it.first.contains(sf::Vector2<int>(x, y)))
		{
			if (it.second(MOUSE_DOWN)) return true;
		}
	}

	return false;
}

bool Context::handle_keypress(sf::Keyboard::Key key)
{
	for (auto it : _keypress_handlers)
	{
		if (it.first == key && it.second()) return true;
	}
	return false;
}

bool Context::handle_keyheld(sf::Keyboard::Key key)
{
	for (auto it : _keyheld_handlers)
	{
		if (it.first == key && it.second()) return true;
	}
	return false;
}

void Context::render(sf::RenderWindow& window)
{
	for (auto draw : _draw_list)
	{
		window.draw(*draw);
	}
}