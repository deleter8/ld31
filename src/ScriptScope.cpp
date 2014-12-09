#include "ScriptScope.h"


ScriptScope::ScriptScope()
{
	actions = std::unordered_map<string_t, std::function<ActionVal(ActionVal*)>>();
	defs = std::unordered_map<string_t, std::function<ScriptScope*(ScriptRaw*)>>();
	async_actions = std::unordered_map<string_t, script_async_action_t>();
	statements = NULL;
	async_statements = NULL;
	scope_target = TEXT("");
}