#include <algorithm>
#include <iostream>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include <math.h>
#ifndef PI 
#define PI 3.14159265
#endif

#include "string_t.h"
#include "ResourceManager.h"
#include "Context.h"
#include "ContextManager.h"
#include "SequenceManager.h"
#include "ScriptRunner.h"
#include "ExecutionManager.h"
#include "ActionScopeManager.h"
#include "Easing.h"
#include "EasingManager.h"

int main()
{
	auto context_manager = new ContextManager();
	auto seq_manager = new SequenceManager();
	auto script_runner = new ScriptRunner();
	auto easing_manager = new EasingManager();
	
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
		std::cout << "running sequence " << ws2s(val->vals) << std::endl;
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
		std::cout << "running animation - " << ws2s(val->vals) << std::endl;
		seq_manager->run_sequence(val->vals, done);
	});

	script_runner->add_action(TEXT("loop_anim"), [&](ActionVal * val){
		if (val->vals.find('.') == string_t::npos)
		{
			val->vals = ActionScopeManager::get_name() + TEXT(".") + val->vals;
		}
		else
		{
			int last_period = val->vals.find_last_of('.');
			string_t context_name = val->vals.substr(0, last_period);
			ActionScopeManager::set_scope(context_name);
		}
		std::cout << "looping animation - " << ws2s(val->vals) << std::endl;
		seq_manager->loop_sequence(val->vals);
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("stop_anim"), [&](ActionVal * val){
		if (val->vals.find('.') == string_t::npos)
		{
			val->vals = ActionScopeManager::get_name() + TEXT(".") + val->vals;
		}
		else
		{
			int last_period = val->vals.find_last_of('.');
			string_t context_name = val->vals.substr(0, last_period);
			ActionScopeManager::set_scope(context_name);
		}
		std::cout << "stopping animation - " << ws2s(val->vals) << std::endl;
		seq_manager->stop_sequence(val->vals);
		return ActionVal::EMPTY();
	});

	easing_manager->add_ease_func(TEXT("linear"), [](float from, float to, float current, float percent)->float{
		return from + (to - from) * percent;
	});

	easing_manager->add_ease_func(TEXT("sine"), [](float from, float to, float current, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(sin(PI * 2 * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("cosine"), [](float from, float to, float current, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(cos(PI * 2 * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("halfsine"), [](float from, float to, float current, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(sin(PI * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("halfcosine"), [](float from, float to, float current, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(cos(PI * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("squared"), [](float from, float to, float current, float percent)->float{
		return from + (to - from) * percent * percent;
	});

	easing_manager->add_ease_func(TEXT("sqrt"), [](float from, float to, float current, float percent)->float{
		return from + (to - from) * sqrt(percent);
	});

	script_runner->add_async_action(TEXT("ease"), [&](ActionVal * val, std::function<void()> done){
		auto ease_attrib = val->vals;
		auto ease_name = val->next->vals;
		
		auto from_val = val->next->next->valf();
		auto to_val = val->next->next->next->valf();
		auto duration = val->next->next->next->next->vali();
		
		auto easing = new Easing(from_val, to_val, duration);
		easing->tfunc = easing_manager->get_ease_func(ease_name);
		easing->on_step = ActionScopeManager::get_step(ease_attrib);
		easing->on_done = done;
		easing->on_step(from_val);
		easing_manager->add_easing(easing);
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
	
    auto music = ResourceManager::get_music(TEXT("b423b42"));
	bool button_pressed = false;
	music->setLoop(true);
	music->play();

	auto key_pressed = std::unordered_map<sf::Keyboard::Key, bool, std::hash<int> >();
	for (auto key : context_manager->Keys())
	{
		key_pressed[key] = false;
	}

	sf::Clock clock;
	int leftover_ticks = 0;
	while (!quit_game)
	{
		sf::Time elapsed = clock.restart();
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

			auto ticks = elapsed.asMilliseconds() + leftover_ticks;
			while (ticks >= 16)
			{
				easing_manager->run(16);
				ticks -= 16;
			}
			leftover_ticks = ticks;

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
