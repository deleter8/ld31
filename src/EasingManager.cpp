#include <iostream>

#include "EasingManager.h"


ease_func_t EasingManager::float_thunk = [](float from, float, float, float){return from; };

EasingManager::EasingManager()
{
	_easings = std::list<Easing*>();
	_ease_funcs = std::unordered_map<string_t, ease_func_t>();
}

void EasingManager::add_easing(Easing * easing)
{
	_easings.push_back(easing);
}

void EasingManager::run(int ticks)
{
	auto expired_easings = std::list<Easing*>();

	for (auto easing : _easings)
	{
		easing->run(ticks);
		if (easing->is_done()) expired_easings.push_back(easing);
	}

	for (auto expired_easing : expired_easings)
	{
		_easings.remove(expired_easing);
		delete expired_easing;
	}
}

void EasingManager::add_ease_func(string_t name, ease_func_t func)
{
	_ease_funcs[name] = func;
}

ease_func_t EasingManager::get_ease_func(string_t name)
{
	if (_ease_funcs.find(name) != _ease_funcs.end()) return _ease_funcs[name];
	std::cout << "could not find easing func - " << ws2s(name) << std::endl;
	return float_thunk;
}