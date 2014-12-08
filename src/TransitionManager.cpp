#include "TransitionManager.h"


TransitionManager::TransitionManager()
{
	_transitions = std::list<Transition*>();
}

void TransitionManager::add_transition(Transition * transition)
{
	_transitions.push_back(transition);
}

void TransitionManager::run(int ticks)
{
	auto expired_transitions = std::list<Transition*>();

	for (auto transition : _transitions)
	{
		transition->run(ticks);
		if (transition->done()) expired_transitions.push_back(transition);
	}

	for (auto expired_transition : expired_transitions)
	{
		_transitions.remove(expired_transition);
	}
}