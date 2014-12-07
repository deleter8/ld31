#include "ScriptRunner.h"
#include "ResourceManager.h"


ScriptRunner::ScriptRunner()
{
	_base_scope = new ScriptScope();
	
	_scopes = std::list<ScriptScope*>();

	_vars = std::unordered_map<string_t, string_t>();
	_base_scope->actions[TEXT("set")] = [&](ActionVal * val){
		_vars[val->vals] = val->next->vals;
		return ActionVal::EMPTY();
	};

	_scopes.push_back(_base_scope);
}

void ScriptRunner::add_action(string_t name, std::function<ActionVal(ActionVal*)> handler)
{
	_base_scope->actions[name] = handler;
}

void ScriptRunner::add_def(string_t name, std::function<ScriptScope*(ScriptRaw*)> handler)
{
	_base_scope->defs[name] = handler;
}

void ScriptRunner::parse(ScriptRaw * script)
{
	string_t cmd = script->line_def;
	if (cmd == TEXT("root"))
	{
		for (auto line : script->inner_lines)
		{
			parse(line);
		}
	}
	else if (cmd == TEXT("import"))
	{
		auto sub_script = ResourceManager::get_script(script->vals->vals);
		parse(sub_script);
	}
	else
	{
		bool handled = false;
		for (auto it = _scopes.rbegin(); it != _scopes.rend(); it++)
		{
			if ((*it)->defs.find(cmd) != (*it)->defs.end())
			{
				auto local_scope = (*it)->defs[cmd](script);
				_scopes.push_back(local_scope);
				for (auto line : script->inner_lines)
				{
					parse(line);
				}
				_scopes.pop_back();
				delete local_scope;
				handled = true;
				break;
			}

			if ((*it)->actions.find(cmd) != (*it)->actions.end())
			{
				_scopes.back()->statements->push_back(script_statement_t((*it)->actions[cmd], script->vals));
				handled = true;
				break;
			}
		}
	}
}

ActionVal * ScriptRunner::eval(ActionVal * vals)
{
	ActionVal * tmp = vals;

	while (tmp != NULL && tmp->vals != TEXT(""))
	{
		while (tmp->vals[0] == '$')
		{
			tmp->vals = _vars[tmp->vals.substr(1)];
		}
		tmp = tmp->next.get();
	}

	return vals;
}

void ScriptRunner::run(ScriptRaw * script)
{
	
	std::list<script_statement_t> lines;
	auto script_scope = new ScriptScope();
	script_scope->statements = &lines;
	_scopes.push_back(script_scope);
	parse(script);
	_scopes.pop_back();
	delete script_scope;

	for (auto line : lines)
	{
		line.first(eval(line.second));
	}
}