#ifndef __SCRIPT_RUNNER__
#define __SCRIPT_RUNNER__

#include <list>
#include <unordered_map>

#include "string_t.h"
#include "ScriptScope.h"
#include "ContextManager.h"


class ScriptRunner
{
private:
	
	ScriptScope *_base_scope;
	std::list<ScriptScope*> _scopes;
	std::unordered_map<string_t, string_t> _vars;

	void parse(ScriptRaw * script);
	ActionVal * eval(ActionVal * vals);

public:

	ScriptRunner();

	void add_action(string_t name, std::function<ActionVal(ActionVal*)> handler);
	void add_def(string_t name, std::function<ScriptScope*(ScriptRaw*)> handler);

	void run(ScriptRaw * script);

};

#endif //__SCRIPT_RUNNER__