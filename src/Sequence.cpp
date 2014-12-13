#include <iostream>

#include "Sequence.h"
#include "ExecutionManager.h"
#include "ActionScopeManager.h"


SequenceStep::SequenceStep()
{
	async_statements = script_async_statement_list_t();
}

void SequenceStep::run(std::function<void()> done, string_t target)
{
	count = async_statements.size();
	if (count == 0)
	{
		ExecutionManager::RunDeferred(done);
		return;
	}

	auto done_wrapper = [&, done](){
		count--;
		if (count == 0) ExecutionManager::RunDeferred(done);
	};

	if(target.size() > 0) ActionScopeManager::set_scope(target);
	for (auto statement : async_statements)
	{
		statement.first(statement.second, done_wrapper);
	}
}

Sequence::Sequence()
{
	_steps = std::list<SequenceStep*>();
	stop_looping = false;
	_already_running = false;
}

void Sequence::run(std::function<void()> done, string_t target)
{
	if (_already_running)
	{
		std::cout << "tried to run sequence that was already running" << std::endl;
		ExecutionManager::RunDeferred(done);
		return;
	}
	_already_running = true;
	std::function<void()> next_done = [&, done](){
		_already_running = false;
		done();
	};
	for (auto it = _steps.rbegin(); it != _steps.rend(); it++)
	{
		auto step = *it;
		next_done = [=](){step->run(next_done, target); };
	}

	ExecutionManager::RunImmediate(next_done);
}

void Sequence::loop_helper(string_t target)
{
	if (!stop_looping)
	{
		run([&, target](){loop_helper(target); }, target);
	}
}

void Sequence::loop(string_t target)
{
	if (_already_running) return;

	stop_looping = false;
	
	loop_helper(target);
}

void Sequence::stop()
{
	stop_looping = true;
}

ScriptScope * Sequence::build_sequence(ScriptRaw *)
{
	auto scope = new ScriptScope();

	scope->defs[TEXT("def_step")] = [&](ScriptRaw* raw){
		
		auto local_scope = new ScriptScope();

		auto step = new SequenceStep();
		_steps.push_back(step);
		local_scope->async_statements = &step->async_statements;

		return local_scope;
	};

	return scope;
}
