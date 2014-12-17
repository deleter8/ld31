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

	easing_manager->add_ease_func(TEXT("linear"), [](float from, float to, float, float percent)->float{
		return from + (to - from) * percent;
	});

	easing_manager->add_ease_func(TEXT("sine"), [](float from, float to, float, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(sin(PI * 2 * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("cosine"), [](float from, float to, float, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(cos(PI * 2 * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("halfsine"), [](float from, float to, float, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(sin(PI * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("halfcosine"), [](float from, float to, float, float percent)->float{
		auto half = (to - from) / 2.f;
		return (float)(cos(PI * percent) * half + half + from);
	});

	easing_manager->add_ease_func(TEXT("squared"), [](float from, float to, float, float percent)->float{
		return from + (to - from) * percent * percent;
	});

	easing_manager->add_ease_func(TEXT("sqrt"), [](float from, float to, float, float percent)->float{
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

	script_runner->add_action(TEXT("pop_context"), [&](ActionVal *){
		context_manager->pop_context();
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("set_default_font"), [](ActionVal * val){
		ResourceManager::set_default_font(val->vals);
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("set_control_if_auto"), [&](ActionVal * val){
		std::cout << "warning: set_control_if_auto not defined yet" << std::endl;
		return ActionVal::EMPTY();
	});


	script_runner->add_action(TEXT("lower"), [](ActionVal * val){
		auto target = val->vals;
		
		if (target == TEXT("music_volume"))
		{
			auto volume = ResourceManager::raw_music_volume();
			volume -= 10.f;
			if (volume < 0) volume = 0;
			ResourceManager::raw_music_volume() = volume;
		}
		else  if (target == TEXT("audio_volume"))
		{
			auto volume = ResourceManager::raw_sound_volume();
			volume -= 10.f;
			if (volume < 0) volume = 0;
			ResourceManager::raw_sound_volume() = volume;
		}
		else if (target == TEXT("global_volume"))
		{
			auto volume = ResourceManager::global_volume();
			volume -= .1f;
			if (volume < 0) volume = 0;
			ResourceManager::set_global_volume(volume);
		}
		
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("raise"), [](ActionVal * val){
		auto target = val->vals;

		if (target == TEXT("music_volume"))
		{
			auto volume = ResourceManager::raw_music_volume();
			volume += 10.f;
			if (volume > 100.f) volume = 100.f;
			ResourceManager::raw_music_volume() = volume;
		}
		else  if (target == TEXT("audio_volume"))
		{
			auto volume = ResourceManager::raw_sound_volume();
			volume += 10.f;
			if (volume > 100.f) volume = 100.f;
			ResourceManager::raw_sound_volume() = volume;
		}
		else if (target == TEXT("global_volume"))
		{
			auto volume = ResourceManager::global_volume();
			volume += .1f;
			if (volume > 1.f) volume = 1.f;
			ResourceManager::set_global_volume(volume);
		}

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
        auto style = sf::Style::Close;
        if(val->next->next->next != NULL && val->next->next->next->vals == TEXT("fullscreen"))
        {
            style = sf::Style::Fullscreen;
        }
        window = new sf::RenderWindow(sf::VideoMode(w, h), title, style);
        window_rect = sf::IntRect(sf::Vector2i(0, 0), (sf::Vector2i)window->getSize());
		ResourceManager::set_screen_res(w, h);

		return ActionVal::EMPTY();
	});
	
	script_runner->add_action(TEXT("quit_game"), [&](ActionVal *){
		window->close();
		sf::sleep(sf::milliseconds(300));
		quit_game = true;
		return ActionVal::EMPTY();
	});

	script_runner->add_action(TEXT("play_sound"), [&](ActionVal * val){
		ResourceManager::get_sound(val->vals)->play();
		
		return ActionVal::EMPTY();
	});


	auto mapped_keys = std::unordered_map<sf::Keyboard::Key, string_t, std::hash<int> >();
	//todo: same for gamepad and mouse stuff, but encapsulate in class first
	script_runner->add_def(TEXT("def_input_mapping"), [&](ScriptRaw * raw){
		string_t mapped_val = raw->vals->vals;
		auto scope = new ScriptScope();
		scope->defs[TEXT("def_in")] = [&, mapped_val](ScriptRaw* in_raw){
			string_t input_type = in_raw->vals->vals;
			if (input_type == TEXT("keypress"))
			{
				mapped_keys[(sf::Keyboard::Key)in_raw->vals->next->vali()] = mapped_val;
			}
			else if (input_type.find(TEXT("gp_")) == 0)
			{
				//gamepad stuffs
			}
			return (ScriptScope*)NULL;
		};
		return scope;
	});



	script_runner->run(ResourceManager::get_script(TEXT("main")));
	
	bool button_pressed = false;

	auto key_pressed = std::unordered_map<sf::Keyboard::Key, bool, std::hash<int> >();
	for (auto key : context_manager->Keys())
	{
		key_pressed[key] = false;
	}

	auto mapped_key_pressed = std::unordered_map<sf::Keyboard::Key, bool, std::hash<int> >();
	for (auto key : mapped_keys)
	{
		mapped_key_pressed[key.first] = false;
	}

    auto video_modes = sf::VideoMode::getFullscreenModes();
    for(auto mode : video_modes)
    {
        if(mode.bitsPerPixel == 32)
        {
            std::cout << mode.width << " x " << mode.height << std::endl;
        }
    }

	sf::Clock clock;
	sf::Time last_time = clock.getElapsedTime();
	int leftover_ticks = 0;
	while (!quit_game)
	{
		sf::Time elapsed = clock.getElapsedTime() - last_time;
		last_time = clock.getElapsedTime();
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

			for (auto key : mapped_keys)
			{
				if (sf::Keyboard::isKeyPressed(key.first))
				{
					mapped_key_pressed[key.first] = true;
				}
				else if (mapped_key_pressed[key.first])
				{
					auto handled = context_manager->handle_named_event(key.second);
					mapped_key_pressed[key.first] = false;
					if (!handled) std::cout << "key was not handled: " << key.first;
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
                    if(!handled) std::cout << "key was not handled: " << key;
				}
			}

			auto coords = sf::Mouse::getPosition(*window);
			bool in_window = window_rect.contains(coords);
			coords.x = (int) ((float) coords.x / ResourceManager::scaling_factor().x);
            coords.y = (int) ((float) coords.y / ResourceManager::scaling_factor().y);

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

			while (ticks >= 5)
			{
				easing_manager->run(5);
				ticks -= 5;
			}
			leftover_ticks = ticks;

			window->clear();
			context_manager->render(*window);
			window->display();
		}
		ExecutionManager::execute();
		sf::sleep(sf::milliseconds(1));
	}

	ResourceManager::clean();

	if(window != NULL) delete window;
	window = NULL;

	std::cout << "all done" << std::endl;
	return 0;
}
