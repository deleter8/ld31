#ifndef __TRANSITION_MANAGER__
#define __TRANSITION_MANAGER__

#include <list>

#include "Transition.h"
#include "ScriptRaw.h"
#include "ScriptScope.h"


class TransitionManager
{
private:

	std::list<Transition*> _transitions;

public:
	
	TransitionManager();
	
	void add_transition(Transition * transition);

	void run(int ticks);
};

#endif //__TRANSITION_MANAGER__