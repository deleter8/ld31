#ifndef __EASING_MANAGER__
#define __EASING_MANAGER__

#include <list>

#include "Easing.h"
#include "ScriptRaw.h"
#include "ScriptScope.h"


class EasingManager
{
private:

	std::list<Easing*> _easings;

public:
	
	EasingManager();
	
	void add_easing(Easing * easing);

	void run(int ticks);
};

#endif //__EASING_MANAGER__