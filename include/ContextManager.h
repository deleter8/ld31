#ifndef __CONTEXT_MANAGER__
#define __CONTEXT_MANAGER__

#include <functional>
#include <unordered_map>

#include "string_t.h"
#include "Context.h"
#include "ActionVal.h"

class ContextManager
{
private:

	std::unordered_map<string_t, int> _int_vals;
	std::unordered_map<string_t, float> _float_vals;
	std::unordered_map<string_t, string_t> _string_vals;

	std::unordered_map<string_t, Context*> _contexts;
	std::unordered_map<string_t, std::function<ActionVal(ActionVal)>> _actions;

public:

	ContextManager();

	void add_context(string_t name, Context * context);

	void add_action(string_t name, std::function<ActionVal(ActionVal)> handler);

	int geti(string_t name);
	float getf(string_t name);
	string_t gets(string_t name);

	void seti(string_t name, int value);
	void setf(string_t name, float value);
	void sets(string_t name, string_t value);
};

#endif //__CONTEXT_MANAGER__