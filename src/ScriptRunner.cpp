#include <iostream>

#include "ScriptRunner.h"
#include "ResourceManager.h"
#include "ExecutionManager.h"


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

void ScriptRunner::add_async_action(string_t name, std::function<void(ActionVal*, std::function<void()>)> handler)
{
	_base_scope->async_actions[name] = handler;
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
		string_t def_ns = TEXT("");
		for (auto it = _scopes.rbegin(); it != _scopes.rend(); it++)
		{
			if ((*it)->defs.find(cmd) != (*it)->defs.end())
			{
				if (def_ns.size() > 0) script->vals->vals = def_ns + script->vals->vals;
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
				if (_scopes.back()->statements != NULL)
				{
					_scopes.back()->statements->push_back(script_statement_t((*it)->actions[cmd], script->vals));
				}
				else if (_scopes.back()->async_statements != NULL)
				{
					//massage sync -> async
					auto action = (*it)->actions[cmd];
					_scopes.back()->async_statements->push_back(script_async_statement_t(
						[action](ActionVal* vals, std::function<void()> done){
							action(vals);
							ExecutionManager::RunDeferred(done);
						}						
						, script->vals));
				}
				handled = true;
				break;
			}

			if ((*it)->async_actions.find(cmd) != (*it)->async_actions.end())
			{
				if (_scopes.back()->statements != NULL)
				{
					//massage async -> sync
					auto async_action = (*it)->async_actions[cmd];
					_scopes.back()->statements->push_back(script_statement_t(
						[async_action](ActionVal * vals){
							async_action(vals, ExecutionManager::thunk);
							return ActionVal::EMPTY();
						}
						, script->vals));
				}
				else if (_scopes.back()->async_statements != NULL)
				{
					_scopes.back()->async_statements->push_back(script_async_statement_t((*it)->async_actions[cmd], script->vals));
				}
				handled = true;
				break;
			}

			if ((*it)->scope_target.size() > 0)
			{
				def_ns = (*it)->scope_target + TEXT(".") + def_ns;
			}
		}
		if (!handled)
		{
			std::cout << "warning: command '" << ws2s(cmd) << "' could not be found" << std::endl;
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