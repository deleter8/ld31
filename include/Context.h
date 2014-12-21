#ifndef __CONTEXT_______
#define __CONTEXT_______

#include <list>
#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "InteractionBase.h"
#include "ScriptScope.h"
#include "ScriptRaw.h"
#include "IRenderObject.h"


class Context : public InteractionBase
{
private:

    std::list<sf::Keyboard::Key> _keys_combined;

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

    virtual ~Context();

    virtual bool handle_mouseclick(int x, int y);
    virtual bool handle_mousedown(int x, int y);
    virtual bool handle_keypress(sf::Keyboard::Key key);
    virtual bool handle_keyheld(sf::Keyboard::Key key);

    virtual bool handle_named_event(string_t event_name);//todo: probably add some optional args?

    virtual const std::list<sf::Keyboard::Key> Keys();

	void render(sf::RenderWindow& window);

	ScriptScope * build_context(ScriptRaw *);
    void prep();
	
	std::function<void(float)> get_step(string_t attrib);
	const string_t& get_name();
	Context * get_inner_element(string_t name);
};

#endif //__CONTEXT_______
