#include <algorithm>
#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>

#include "string_t.h"
#include "ResourceManager.h"
#include "Context.h"
#include "ContextManager.h"
#include "SequenceManager.h"
#include "ScriptRunner.h"
#include "ExecutionManager.h"
#include "ActionScopeManager.h"

int main()
{
	auto context_manager = new ContextManager();
	auto seq_manager = new SequenceManager();
	auto script_runner = new ScriptRunner();
	
	ActionScopeManager::init(context_manager);

	string_t asset_path = TEXT("../assets/");
	ResourceManager::init(asset_path);

	sf::RenderWindow * window = NULL;
	auto window_rect = sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(0, 0));
	bool quit_game = false;

	script_runner->add_def(TEXT("def_context"), [&](ScriptRaw* raw){
		auto context = new Context(raw->vals->vals);
		auto local_scope = context->build_context(raw);
		context_manager->add_context(raw->vals->vals, context);
		return local_scope;
	});

	script_runner->add_def(TEXT("def_resource"), [&](ScriptRaw* raw){
		return ResourceManager::build_resource(raw);
	});

	script_runner->add_def(TEXT("def_seq"), [&](ScriptRaw* raw){
		auto seq = new Sequence();
		auto local_scope = seq->build_sequence(raw);
		seq_manager->add_sequence(raw->vals->vals, seq);
		return local_scope;
	});

	script_runner->add_async_action(TEXT("run_seq"), [&](ActionVal * val, std::function<void()> done){
		seq_manager->run_sequence(val->vals, done);
	});

	script_runner->add_def(TEXT("def_anim"), [&](ScriptRaw* raw){
		auto seq = new Sequence();
		auto local_scope = seq->build_sequence(raw);
		seq_manager->add_sequence(raw->vals->vals, seq);
		return local_scope;
	});

	script_runner->add_async_action(TEXT("run_anim"), [&](ActionVal * val, std::function<void()> done){
		
		//if the anim requested is lacking a context, it must be an embedded handler
		//thus we can assume the action scope is setup correctly, we just need to 
		//grab the name so we can find the right animation
		if (val->vals.find('.') == string_t::npos)
		{
			val->vals = ActionScopeManager::get_name() + TEXT(".") + val->vals;
		}
		//on the flip side, if it contains a context name, we need to target said context
		else
		{
			int last_period = val->vals.find_last_of('.');
			string_t context_name = val->vals.substr(0, last_period);
			ActionScopeManager::set_scope(context_name);
		}
		
		seq_manager->run_sequence(val->vals, done);
	});

	script_runner->add_action(TEXT("push_context"), [&](ActionVal * val){
		context_manager->push_context(val->vals);
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("pop_context"), [&](ActionVal * val){
		context_manager->pop_context();
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("set_default_font"), [](ActionVal * val){
		ResourceManager::set_default_font(val->vals);
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("open_window"), [&](ActionVal * val){
		if (window != NULL)
		{
			if (window->isOpen())
			{
				window->close();
			}
			delete window;
		}
		int w = val->vali();
		int h = val->next->vali();
		string_t title = val->next->next->vals;
		window = new sf::RenderWindow(sf::VideoMode(w, h), title);
		window_rect = sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)window->getSize());
		ResourceManager::set_screen_res(w, h);

		return ActionVal::EMPTY();
	});
	
	script_runner->add_action(TEXT("quit_game"), [&](ActionVal * val){
		window->close();
		sf::sleep(sf::milliseconds(300));
		quit_game = true;
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("play_sound"), [&](ActionVal * val){
		ResourceManager::get_sound(val->vals)->play();
		
		return ActionVal::EMPTY();
	});

	script_runner->run(ResourceManager::get_script(TEXT("main")));
	
    auto music = ResourceManager::get_music(TEXT("robert_del_naja_HS"));
	bool button_pressed = false;
	music->play();

	auto key_pressed = std::unordered_map<sf::Keyboard::Key, bool, std::hash<int> >();
	for (auto key : context_manager->Keys())
	{
		key_pressed[key] = false;
	}

	while (!quit_game)
	{
		if (window != NULL && window->isOpen())
		{
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window->close();
				}
			}

			for (auto key : context_manager->Keys())
			{
				if (sf::Keyboard::isKeyPressed(key))
				{
					context_manager->handle_keyheld(key);
					key_pressed[key] = true;
				}
				else if (key_pressed[key])
				{
					auto handled = context_manager->handle_keypress(key);
					key_pressed[key] = false;
				}
			}

			auto coords = sf::Mouse::getPosition(*window);
			bool in_window = window_rect.contains(coords);
			coords.x = (int) ((float) coords.x / ResourceManager::scaling_factor().x);
			coords.y = (int)((float) coords.y / ResourceManager::scaling_factor().y);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				if (in_window) context_manager->handle_mousedown(coords.x, coords.y);
				button_pressed = true;
			}
			else if (button_pressed)
			{
				button_pressed = false;
				//todo: enhance with coords where button went down for drag and such
				if (in_window) context_manager->handle_mouseclick(coords.x, coords.y);
				if (in_window) std::cout << coords.x << ", " << coords.y << std::endl;
			}

			window->clear();
			context_manager->render(*window);
			window->display();
		}
		ExecutionManager::execute();
		sf::sleep(sf::milliseconds(1));
	}

	music->stop();

	delete music;
	music = NULL;

	ResourceManager::clean();

	delete window;

	//context:
		//view
		//input
		//resources - text, images, sounds, music
	std::cout << "all done" << std::endl;
	return 0;
}
