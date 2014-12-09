#include "ResourceManager.h"
#include "ContextManager.h"

ContextManager::ContextManager()
{
	_contexts = std::unordered_map<string_t, Context*>();
	_context_stack = std::list<Context*>();
	_keys = std::list<sf::Keyboard::Key>();
}

void ContextManager::add_context(string_t name, Context * context)
{
	_contexts[name] = context;
	for (auto key : context->Keys())
	{
		_keys.push_back(key);
	}

	_keys.unique();
}

void ContextManager::push_context(string_t name)
{
	_contexts[name]->prep();
	_context_stack.push_back(_contexts[name]);
}

void ContextManager::pop_context()
{
	_context_stack.pop_back();
}

bool ContextManager::handle_mouseclick(int x, int y)
{
	auto top_context = _context_stack.rbegin();
	for (auto it = top_context; it != _context_stack.rend(); it++)
	{
		if (!(*it)->only_handle_when_top_context() || it == top_context)
		{
			if ((*it)->handle_mouseclick(x, y)) return true;
		}
	}

	return false;
}

bool ContextManager::handle_mousedown(int x, int y)
{
	auto top_context = _context_stack.rbegin();
	for (auto it = top_context; it != _context_stack.rend(); it++)
	{
		if (!(*it)->only_handle_when_top_context() || it == top_context)
		{
			if ((*it)->handle_mousedown(x, y)) return true;
		}
	}

	return false;
}

bool ContextManager::handle_keypress(sf::Keyboard::Key key)
{
	auto top_context = _context_stack.rbegin();
	for (auto it = top_context; it != _context_stack.rend(); it++)
	{
		if (!(*it)->only_handle_when_top_context() || it == top_context)
		{
			if ((*it)->handle_keypress(key)) return true;
		}
	}

	return false;
}

bool ContextManager::handle_keyheld(sf::Keyboard::Key key)
{
	auto top_context = _context_stack.rbegin();
	for (auto it = top_context; it != _context_stack.rend(); it++)
	{
		if (!(*it)->only_handle_when_top_context() || it == top_context)
		{
			if ((*it)->handle_keyheld(key)) return true;
		}
	}

	return false;
}


void ContextManager::render(sf::RenderWindow& window)
{
	Context * top_context = NULL;
	if (_context_stack.size() > 0) top_context = _context_stack.back();
	for (auto context : _context_stack)
	{
		if (!context->only_handle_when_top_context() || context == top_context)
		{
			context->render(window);
		}
	}
}

const std::list<sf::Keyboard::Key> ContextManager::Keys()
{
	_keys.clear();
	for (auto context : _context_stack)
	{
		for (auto key : context->Keys())
		{
			_keys.push_back(key);
		}
	}
	_keys.unique();
	return _keys;
}

Context * ContextManager::get_context(string_t name)
{
	if (_contexts.find(name) != _contexts.end())
	{
		return _contexts[name];
	}
	else
	{
		for (auto context : _contexts)
		{
			if (name.find(context.first) == 0)
			{
				return _contexts[context.first]->get_inner_element(name);
			}
		}
	}

	return NULL;
}