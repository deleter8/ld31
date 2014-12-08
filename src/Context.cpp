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

	has_lingering_mouseclick_handler = false;
	has_lingering_mousedown_handler = false;

	_only_handle_when_top_context = false;
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
	_keys.clear();
	for (auto element : _inner_elements)
	{
		for (auto key : element->Keys())
		{
			_keys.push_back(key);
		}
	}	
	for (auto handler : _keypress_handlers)
	{
		_keys.push_back(handler.first);
	}
	for (auto handler : _keyheld_handlers)
	{
		_keys.push_back(handler.first);
	}
	_keys.unique();
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
			auto sprite_thing = SpriteThings();
			sprite_thing.sprite = sprite_name;
			sprite_thing.x = x;
			sprite_thing.y = y;
			_sprites.push_back(sprite_thing);
		} 
		else if (attrib_name == TEXT("value"))
		{
			//TODO 
		}
		else if (attrib_name == TEXT("hide_nonfocused"))
		{
			_only_handle_when_top_context = true;
		}
		
		return (ScriptScope*)NULL;
	};

	scope->defs[TEXT("def_handler")] = [&](ScriptRaw* raw){
		string_t event_name = raw->vals->vals;
		if (event_name.find(TEXT("mouse")) == 0)
		{
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

			sf::IntRect rect;
			if (raw->vals->next == NULL || raw->vals->next->vals == TEXT(""))
			{
				if (event_name == TEXT("mouseclick")) lingering_mouseclick_handler = handler;
				else if (event_name == TEXT("mousedown")) lingering_mousedown_handler = handler;
				if (event_name == TEXT("mouseclick")) has_lingering_mouseclick_handler = true;
				else if (event_name == TEXT("mousedown")) has_lingering_mousedown_handler = true;

				if (_sprites.size() != 1) return (ScriptScope*)NULL; //error bad def
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

void Context::prep()
{
	_draw_list.clear();
	_first_sprite = NULL;

	for (auto thing : _sprites)
	{
		auto sprite = ResourceManager::get_sprite(thing.sprite);
		sprite->setPosition(thing.x * ResourceManager::scaling_factor().x, 
					        thing.y * ResourceManager::scaling_factor().y);
		_draw_list.push_back(sprite);
		_first_sprite = sprite;
	}

	if (has_lingering_mouseclick_handler)
	{
		auto rect = (sf::IntRect)_first_sprite->getGlobalBounds();
		rect.left /= ResourceManager::scaling_factor().x;
		rect.top /= ResourceManager::scaling_factor().y;
		rect.width /= ResourceManager::scaling_factor().x;
		rect.height /= ResourceManager::scaling_factor().y;

		add_mouseclick_handler(rect, lingering_mouseclick_handler);
		has_lingering_mouseclick_handler = false;
	}

	if (has_lingering_mousedown_handler)
	{
		auto rect = (sf::IntRect)_first_sprite->getGlobalBounds();
		add_mousedown_handler(rect, lingering_mousedown_handler);
		has_lingering_mousedown_handler = false;
	}

	for (auto element : _inner_elements)
	{
		element->prep();
	}
}

const bool& Context::only_handle_when_top_context()
{
	return _only_handle_when_top_context;
}