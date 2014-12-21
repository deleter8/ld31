#ifndef __INTERACTIONBASE_H__
#define __INTERACTIONBASE_H__

#include <list>
#include <functional>
#include <SFML/Graphics.hpp>

#include "string_t.h"


enum MouseEvent
{
    MOUSE_DOWN,
    MOUSE_CLICK
};

class InteractionBase
{

private:

    std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mouseclick_handlers;
    std::list<std::pair<sf::IntRect, std::function<bool(MouseEvent)>>> _mousedown_handlers;
    std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keypress_handlers;
    std::list<std::pair<sf::Keyboard::Key, std::function<bool()>>> _keyheld_handlers;

    std::list<std::pair<string_t, std::function<bool()>>> _named_event_handlers;

    std::list<sf::Keyboard::Key> _keys;

protected:

    bool _only_handle_when_top_context;

    InteractionBase();

public:

    virtual ~InteractionBase();

    void add_mouseclick_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler);
    void add_mousedown_handler(sf::IntRect region, std::function<bool(MouseEvent)> handler);
    void add_keypress_handler(sf::Keyboard::Key key, std::function<bool()> handler);
    void add_keyheld_handler(sf::Keyboard::Key key, std::function<bool()> handler);
    void add_named_event_handler(string_t event_name, std::function<bool()> handler);

    virtual bool handle_mouseclick(int x, int y);
    virtual bool handle_mousedown(int x, int y);
    virtual bool handle_keypress(sf::Keyboard::Key key);
    virtual bool handle_keyheld(sf::Keyboard::Key key);

    virtual bool handle_named_event(string_t event_name);//todo: probably add some optional args?

    virtual const std::list<sf::Keyboard::Key> Keys();

    const bool& only_handle_when_top_context();

};

#endif // __INTERACTIONBASE_H__
