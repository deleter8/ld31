#include "include/InteractionBase.h"


InteractionBase::InteractionBase()
{
    _mouseclick_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
    _mousedown_handlers = std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>>();
    _keypress_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
    _keyheld_handlers = std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>>();
    _keys = std::list<sf::Keyboard::Key>();
    _named_event_handlers = std::list<std::pair<string_t, std::function<bool()>>>();

    _only_handle_when_top_context = false;
}

InteractionBase::~InteractionBase()
{

}

void InteractionBase::add_mouseclick_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler)
{
    _mouseclick_handlers.push_back(std::pair<sf::IntRect, std::function<bool(MouseEvent)>>(region, handler));
}

void InteractionBase::add_mousedown_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler)
{
    _mousedown_handlers.push_back(std::pair<sf::IntRect, std::function<bool(MouseEvent)>>(region, handler));
}

void InteractionBase::add_keypress_handler(sf::Keyboard::Key key, std::function<bool()> handler)
{
    _keypress_handlers.push_back(std::pair<sf::Keyboard::Key, std::function<bool()>>(key, handler));
    _keys.push_back(key);
}

void InteractionBase::add_keyheld_handler(sf::Keyboard::Key key, std::function<bool()> handler)
{
    _keyheld_handlers.push_back(std::pair<sf::Keyboard::Key, std::function<bool()>>(key, handler));
    _keys.push_back(key);
}

void InteractionBase::add_named_event_handler(string_t event_name, std::function<bool()> handler)
{
    _named_event_handlers.push_back(std::pair<string_t, std::function<bool()>>(event_name, handler));
}

bool InteractionBase::handle_mouseclick(int x, int y)
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

bool InteractionBase::handle_mousedown(int x, int y)
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

bool InteractionBase::handle_keypress(sf::Keyboard::Key key)
{
    for (auto it : _keypress_handlers)
    {
        if (it.first == key && it.second()) return true;
    }

    return false;
}

bool InteractionBase::handle_keyheld(sf::Keyboard::Key key)
{
    for (auto it : _keyheld_handlers)
    {
        if (it.first == key && it.second()) return true;
    }

    return false;
}

bool InteractionBase::handle_named_event(string_t event_name)
{
    for (auto handler : _named_event_handlers)
    {
        if (handler.first == event_name && handler.second()) return true;
    }

    return false;
}

const std::list<sf::Keyboard::Key> InteractionBase::Keys()
{
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

const bool& InteractionBase::only_handle_when_top_context()
{
    return _only_handle_when_top_context;
}
