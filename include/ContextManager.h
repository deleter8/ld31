#ifndef __CONTEXT_MANAGER__
#define __CONTEXT_MANAGER__

#include <functional>
#include <unordered_map>
#include <list>

#include "string_t.h"
#include "Context.h"
#include "ActionVal.h"

class ContextManager
{
private:

	std::unordered_map<string_t, Context*> _contexts;
	std::list<Context*> _context_stack;

	std::list<sf::Keyboard::Key> _keys;

public:

	ContextManager();

	void add_context(string_t name, Context * context);

	void push_context(string_t name);
	void pop_context();

	bool handle_mouseclick(int x, int y);
	bool handle_mousedown(int x, int y);
	bool handle_keypress(sf::Keyboard::Key key);
	bool handle_keyheld(sf::Keyboard::Key key);

	void render(sf::RenderWindow& window);

	const std::list<sf::Keyboard::Key> Keys();
};

#endif //__CONTEXT_MANAGER__