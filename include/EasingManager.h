#ifndef __EASING_MANAGER__
#define __EASING_MANAGER__

#include <list>
#include <unordered_map>

#include "string_t.h"
#include "Easing.h"
#include "ScriptRaw.h"
#include "ScriptScope.h"


class EasingManager
{
private:

	std::list<Easing*> _easings;

	std::unordered_map<string_t, ease_func_t> _ease_funcs;

	static ease_func_t float_thunk;

public:
	
	EasingManager();
	
	void add_easing(Easing * easing);

	void add_ease_func(string_t name, ease_func_t func);
	ease_func_t get_ease_func(string_t name);

	void run(int ticks);
};

#endif //__EASING_MANAGER__