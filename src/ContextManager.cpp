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
	_context_stack.push_back(_contexts[name]);
}

void ContextManager::pop_context()
{
	_context_stack.pop_back();
}

bool ContextManager::handle_mouseclick(int x, int y)
{
	for (auto it = _context_stack.rbegin(); it != _context_stack.rend(); it++)
	{
		if ((*it)->handle_mouseclick(x, y)) return true;
	}

	return false;
}

bool ContextManager::handle_mousedown(int x, int y)
{
	for (auto it = _context_stack.rbegin(); it != _context_stack.rend(); it++)
	{
		if ((*it)->handle_mousedown(x, y)) return true;
	}

	return false;
}

bool ContextManager::handle_keypress(sf::Keyboard::Key key)
{
	for (auto it = _context_stack.rbegin(); it != _context_stack.rend(); it++)
	{
		if ((*it)->handle_keypress(key)) return true;
	}

	return false;
}

bool ContextManager::handle_keyheld(sf::Keyboard::Key key)
{
	for (auto it = _context_stack.rbegin(); it != _context_stack.rend(); it++)
	{
		if ((*it)->handle_keypress(key)) return true;
	}

	return false;
}


void ContextManager::render(sf::RenderWindow& window)
{
	for (auto context : _context_stack)
	{
		context->render(window);
	}
}

const std::list<sf::Keyboard::Key> ContextManager::Keys()
{
	return _keys;
}


//int ContextManager::geti(string_t name)
//{
//	return _int_vals[name];
//}
//
//float ContextManager::getf(string_t name)
//{
//	return _float_vals[name];
//}
//
//string_t ContextManager::gets(string_t name)
//{
//	return _string_vals[name];
//}
//
//void ContextManager::seti(string_t name, int value)
//{
//	_int_vals[name] = value;
//}
//
//void ContextManager::setf(string_t name, float value)
//{
//	_float_vals[name] = value;
//}
//
//void ContextManager::sets(string_t name, string_t value)
//{
//	_string_vals[name] = value;
//}
