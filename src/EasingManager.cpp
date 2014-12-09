#include "EasingManager.h"


EasingManager::EasingManager()
{
	_easings = std::list<Easing*>();
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
		if (easing->done()) expired_easings.push_back(easing);
	}

	for (auto expired_easing : expired_easings)
	{
		_easings.remove(expired_easing);
		delete expired_easing;
	}
}