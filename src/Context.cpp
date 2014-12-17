#include "Context.h"
#include "ResourceManager.h"
#include "ActionScopeManager.h"
#include "PrimitiveRenderObjects.h"
#include <iostream>


Context::Context(string_t name)
{
	_name = name;
	_mouseclick_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
	_mousedown_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
	_keypress_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
	_keyheld_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
	_keys = std::list<sf::Keyboard::Key>();
	_music = NULL;
	_inner_elements = std::list<Context*>();
	_inner_element_lookup = std::unordered_map<string_t, Context*>();

	has_lingering_mouseclick_handler = false;
	has_lingering_mousedown_handler = false;

	_only_handle_when_top_context = false;
	_music_thing = TEXT("");
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

void Context::add_named_event_handler(string_t event_name, std::function<bool()> handler)
{
	_named_event_handlers.push_back(std::pair<string_t, std::function<bool()>>(event_name, handler));
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
	ActionScopeManager::set_scope(this);
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
	ActionScopeManager::set_scope(this);
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
	ActionScopeManager::set_scope(this);
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
	ActionScopeManager::set_scope(this);
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

bool Context::handle_named_event(string_t event_name)
{
	for (auto handler : _named_event_handlers)
	{
		if (handler.first == event_name && handler.second()) return true;
	}

	for (auto element : _inner_elements)
	{
		if (element->handle_named_event(event_name)) return true;
	}
	return false;
}

void Context::render(sf::RenderWindow& window)
{
	ActionScopeManager::set_scope(this);
	for (auto draw : _draw_list)
	{
        draw->render(window);
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

ScriptScope * Context::build_context(ScriptRaw * raw)
{
	auto scope = new ScriptScope();
	scope->scope_target = raw->vals->vals;
	
	scope->statements = NULL; //error b/c this doesn't make sense, there shouldn't be actions in the def-scope of context
	
	scope->defs[TEXT("def_attrib")] = [&](ScriptRaw* attrib_raw){
		string_t attrib_name = attrib_raw->vals->vals;
		if (attrib_name == TEXT("sprite"))
		{
			string_t sprite_name = attrib_raw->vals->next->vals;
			auto x = attrib_raw->vals->next->next->vali();
			auto y = attrib_raw->vals->next->next->next->vali();
            auto draw_thing = new SpriteRenderObject(sprite_name);
            draw_thing->set_position((float)x, (float)y);
            _draw_list.push_back(draw_thing);
		}
		else if (attrib_name == TEXT("text"))
		{
			string_t text_name = attrib_raw->vals->next->vals;
			auto x = attrib_raw->vals->next->next->vali();
			auto y = attrib_raw->vals->next->next->next->vali();
            auto draw_thing = new TextRenderObject(text_name);
            draw_thing->set_position((float)x, (float)y);

			if (attrib_raw->vals->next->next->next->next != NULL && attrib_raw->vals->next->next->next->next->vals != TEXT(""))
			{
                draw_thing->set_font_size(attrib_raw->vals->next->next->next->next->vali());
            }
            _draw_list.push_back(draw_thing);
		}
        else if(attrib_name == TEXT("map"))
        {
            string_t map_name = attrib_raw->vals->next->vals;
            _draw_list.push_back(ResourceManager::get_map(map_name));
        }
		else if (attrib_name == TEXT("music"))
		{
			_music_thing = attrib_raw->vals->next->vals;
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

    scope->defs[TEXT("def_handler")] = [&](ScriptRaw* handler_raw){
        string_t event_name = handler_raw->vals->vals;
		if (event_name.find(TEXT("mouse")) == 0)
		{
			auto local_scope = new ScriptScope();
			
			auto handler_statements = std::shared_ptr<script_statement_list_t>(new script_statement_list_t());

			local_scope->statements = handler_statements.get();
			
			auto handler = [handler_statements](MouseEvent){
				for (auto stmt : *handler_statements)
				{
					stmt.first(stmt.second);
				}
				return true;
			};

			
            if (handler_raw->vals->next == NULL || handler_raw->vals->next->vals == TEXT(""))
			{
				if (event_name == TEXT("mouseclick")) lingering_mouseclick_handler = handler;
				else if (event_name == TEXT("mousedown")) lingering_mousedown_handler = handler;
				if (event_name == TEXT("mouseclick")) has_lingering_mouseclick_handler = true;
				else if (event_name == TEXT("mousedown")) has_lingering_mousedown_handler = true;

                if (_draw_list.size() != 1) return (ScriptScope*)NULL; //error bad def
			}
			else
			{
				sf::IntRect rect;
                auto x1 = handler_raw->vals->next->vali();
                auto y1 = handler_raw->vals->next->next->vali();
                auto x2 = handler_raw->vals->next->next->next->vali();
                auto y2 = handler_raw->vals->next->next->next->next->vali();

				auto topleft = sf::Vector2i(x1, y1);
				auto bottomright = sf::Vector2i(x2, y2);
				rect = sf::IntRect(topleft, bottomright - topleft);

				if (event_name == TEXT("mouseclick"))
				{
					add_mouseclick_handler(rect, handler);
				}
				else if (event_name == TEXT("mousedown"))
				{
					add_mousedown_handler(rect, handler);
				}
			}

			return local_scope;
		}
		else if (event_name.find(TEXT("key")) == 0)
		{
            auto key = (sf::Keyboard::Key)handler_raw->vals->next->vali();

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
		else
		{
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

			add_named_event_handler(event_name, handler);

			return local_scope;
		}
	};

    scope->defs[TEXT("def_element")] = [&](ScriptRaw * element_raw){
        auto inner_name = _name + TEXT(".") + element_raw->vals->vals;
		auto inner_element = new Context(inner_name);
        auto build_thing = inner_element->build_context(element_raw);
		add_inner_element(inner_name, inner_element);
		return build_thing;
	};

	return scope;
}

void Context::prep()
{
    _context_dimensions = sf::FloatRect(0,0,0,0);

    for (auto thing : _draw_list)
    {
        thing->prep();
        _context_dimensions = thing->get_bounds();
	}

	if (has_lingering_mouseclick_handler)
	{
		auto rect = _context_dimensions;
		rect.left /= ResourceManager::scaling_factor().x;
		rect.top /= ResourceManager::scaling_factor().y;
		rect.width /= ResourceManager::scaling_factor().x;
		rect.height /= ResourceManager::scaling_factor().y;

		add_mouseclick_handler((sf::IntRect)rect, lingering_mouseclick_handler);
		has_lingering_mouseclick_handler = false;
	}

	if (has_lingering_mousedown_handler)
	{
		auto rect = _context_dimensions;
		rect.left /= ResourceManager::scaling_factor().x;
		rect.top /= ResourceManager::scaling_factor().y;
		rect.width /= ResourceManager::scaling_factor().x;
		rect.height /= ResourceManager::scaling_factor().y;
		add_mousedown_handler((sf::IntRect)rect, lingering_mousedown_handler);
		has_lingering_mousedown_handler = false;
	}

	for (auto element : _inner_elements)
	{
		element->prep();
	}

	if (_music_thing != TEXT(""))
	{
		if (_music != NULL)
		{
			_music->stop();
			delete _music;
		}
		_music = ResourceManager::get_music(_music_thing);
		_music->setLoop(true);
		_music->setVolume(0);
		_music->play();
	}
}

const bool& Context::only_handle_when_top_context()
{
	return _only_handle_when_top_context;
}


void Context::step_scale(float val)
{
    for (auto thing : _draw_list) thing->set_scale(val, val);
	for (auto inner : _inner_elements) inner->step_scale(val);
}

void Context::step_scalex(float val)
{
    for (auto thing : _draw_list) thing->set_scale(val, thing->get_scale().y);
	for (auto inner : _inner_elements) inner->step_scalex(val);
}

void Context::step_scaley(float val)
{
    for (auto thing : _draw_list)thing->set_scale(thing->get_scale().x, val);
	for (auto inner : _inner_elements) inner->step_scaley(val);
}

void Context::step_opacity(float val)
{
    for (auto thing : _draw_list)
	{
        auto cc = thing->get_color();
        thing->set_color(sf::Color(cc.r, cc.g, cc.b, (sf::Uint8)(255 * val)));
	}
	for (auto inner : _inner_elements) inner->step_opacity(val);
}

void Context::step_volume(float val)
{
	_music->setVolume(val * ResourceManager::music_volume());
}

std::function<void(float)> Context::get_step(string_t attrib)
{
	if (attrib == TEXT("scale"))
	{
		return[&](float val){ step_scale(val); };
	}
	else if (attrib == TEXT("scale_x"))
	{
		return[&](float val){ step_scalex(val); };
	}
	else if (attrib == TEXT("scale_y"))
	{
		return[&](float val){ step_scaley(val); };
	}
	else if (attrib == TEXT("opacity"))
	{
		return[&](float val){step_opacity(val); };
	}
	else if (attrib == TEXT("volume"))
	{
		return[&](float val){step_volume(val); };
	}
	return[](float){};
}

const string_t& Context::get_name()
{
	return _name;
}

Context * Context::get_inner_element(string_t name)
{
	if (_inner_element_lookup.find(name) != _inner_element_lookup.end())
	{
		return _inner_element_lookup[name];
	}
	else
	{
		for (auto context : _inner_elements)
		{
			if (name.find(context->_name) == 0)
			{
				return _inner_element_lookup[context->_name];
			}
		}
	}	

	return NULL;
}
