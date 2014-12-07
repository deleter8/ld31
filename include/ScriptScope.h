#ifndef __SCRIPT_SCOPE__
#define __SCRIPT_SCOPE__

#include <functional>
#include <unordered_map>

#include "string_t.h"
#include "ActionVal.h"
#include "ScriptRaw.h"

typedef std::function<ActionVal(ActionVal*)> script_action_t;
typedef std::pair<script_action_t, ActionVal*> script_statement_t;
typedef std::list<script_statement_t> script_statement_list_t;

class ScriptScope
{
public:
	ScriptScope();
	std::unordered_map<string_t, script_action_t> actions;
	std::unordered_map<string_t, std::function<ScriptScope*(ScriptRaw*)>> defs;
	std::list<script_statement_t> * statements;
};

#endif //__SCRIPT_SCOPE__