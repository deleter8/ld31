#include "Context.h"
#include "ResourceManager.h"

Context::Context()
{
	_mouseclick_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
	_mousedown_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
	_keypress_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
	_keyheld_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
	_keys = std::list<sf::Keyboard::Key>();

	_inner_elements = std::list<Context*>();
	_inner_element_lookup = std::unordered_map<string_t, Context*>();

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
	_keys.push_back(key);
}

void Context::add_keyheld_handler(sf::Keyboard::Key key, std::function<bool()> handler)
{
	_keyheld_handlers.push_back(std::pair<sf::Keyboard::Key, std::function<bool()>>(key, handler));
	_keys.push_back(key);
}

void Context::add_inner_element(string_t name, Context * element)
{
	_inner_element_lookup[name] = element;
	_inner_elements.push_back(element);

	for (auto key : element->Keys())
	{
		_keys.push_back(key);
	}

	_keys.unique();
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

	for (auto element : _inner_elements)
	{
		if (element->handle_mouseclick(x, y)) return true;
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

	for (auto element : _inner_elements)
	{
		if (element->handle_mousedown(x, y)) return true;
	}

	return false;
}

bool Context::handle_keypress(sf::Keyboard::Key key)
{
	for (auto it : _keypress_handlers)
	{
		if (it.first == key && it.second()) return true;
	}

	for (auto element : _inner_elements)
	{
		if (element->handle_keypress(key)) return true;
	}
	return false;
}

bool Context::handle_keyheld(sf::Keyboard::Key key)
{
	for (auto it : _keyheld_handlers)
	{
		if (it.first == key && it.second()) return true;
	}

	for (auto element : _inner_elements)
	{
		if (element->handle_keyheld(key)) return true;
	}
	return false;
}

void Context::render(sf::RenderWindow& window)
{
	for (auto draw : _draw_list)
	{
		window.draw(*draw);
	}
	for (auto element : _inner_elements)
	{
		element->render(window);
	}
}

const std::list<sf::Keyboard::Key> Context::Keys()
{
	return _keys;
}

ScriptScope * Context::build_context(ScriptRaw *)
{
	auto scope = new ScriptScope();

	scope->statements = NULL; //error b/c this doesn't make sense, there shouldn't be actions in the def-scope of context
	
	scope->defs[TEXT("def_attrib")] = [&](ScriptRaw* raw){
		string_t attrib_name = raw->vals->vals;
		if (attrib_name == TEXT("sprite"))
		{
			string_t sprite_name = raw->vals->next->vals;
			auto x = raw->vals->next->next->vali();
			auto y = raw->vals->next->next->next->vali();
			auto sprite = ResourceManager::get_sprite(sprite_name);
			sprite->setPosition(x, y);
			_draw_list.push_back(sprite);
			_first_sprite = sprite;
		} 
		else if (attrib_name == TEXT("value"))
		{
			//TODO 
		}
		
		return (ScriptScope*)NULL;
	};

	scope->defs[TEXT("def_handler")] = [&](ScriptRaw* raw){
		string_t event_name = raw->vals->vals;
		if (event_name.find(TEXT("mouse")) == 0)
		{
			sf::IntRect rect;
			if (raw->vals->next == NULL || raw->vals->next->vals == TEXT(""))
			{
				if (_draw_list.size() != 1) return (ScriptScope*)NULL; //error bad def
				rect = (sf::IntRect)_first_sprite->getGlobalBounds();
			}
			else
			{
				auto x1 = raw->vals->next->vali();
				auto y1 = raw->vals->next->next->vali();
				auto x2 = raw->vals->next->next->next->vali();
				auto y2 = raw->vals->next->next->next->next->vali();

				auto topleft = sf::Vector2i(x1, y1);
				auto bottomright = sf::Vector2i(x2, y2);
				rect = sf::IntRect(topleft, bottomright - topleft);
			}
			
			auto local_scope = new ScriptScope();
			
			auto handler_statements = std::shared_ptr<script_statement_list_t>(new script_statement_list_t());

			local_scope->statements = handler_statements.get();
			
			auto handler = [handler_statements](MouseEvent ev){
				for (auto stmt : *handler_statements)
				{
					stmt.first(stmt.second);
				}
				return true;
			};

			if (event_name == TEXT("mouseclick"))
			{ 
				add_mouseclick_handler(rect, handler);
			}
			else if (event_name == TEXT("mousedown"))
			{
				add_mousedown_handler(rect, handler);
			}

			return local_scope;
		}
		else if (event_name.find(TEXT("key")) == 0)
		{
			auto key = (sf::Keyboard::Key)raw->vals->next->vali();

			auto local_scope = new ScriptScope();

			auto handler_statements = std::shared_ptr<script_statement_list_t>(new script_statement_list_t());

			local_scope->statements = handler_statements.get();

			auto handler = [handler_statements](){
				for (auto stmt : *handler_statements)
				{
					stmt.first(stmt.second);
				}
				return true;
			};

			if (event_name == TEXT("keypress"))
			{
				add_keypress_handler(key, handler);
			}
			else if (event_name == TEXT("keyheld"))
			{
				add_keyheld_handler(key, handler);
			}

			return local_scope;
		}

		return (ScriptScope*)NULL;
	};

	scope->defs[TEXT("def_element")] = [&](ScriptRaw * raw){
		auto inner_element = new Context();
		auto build_thing = inner_element->build_context(raw);
		add_inner_element(raw->vals->vals, inner_element);
		return build_thing;
	};

	return scope;
}