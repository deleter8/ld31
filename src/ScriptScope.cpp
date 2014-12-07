#include "ScriptScope.h"


ScriptScope::ScriptScope()
{
	actions = std::unordered_map<string_t, std::function<ActionVal(ActionVal*)>>();
	defs = std::unordered_map<string_t, std::function<ScriptScope*(ScriptRaw*)>>();
}