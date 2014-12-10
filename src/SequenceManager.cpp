#include <iostream>

#include "SequenceManager.h"
#include "ExecutionManager.h"


SequenceManager::SequenceManager()
{
	_sequences = std::unordered_map<string_t, Sequence*>();
}

void SequenceManager::add_sequence(string_t name, Sequence * sequence)
{
	_sequences[name] = sequence;
}

void SequenceManager::run_sequence(string_t name)
{
	if (_sequences.find(name) != _sequences.end())
	{
		_sequences[name]->run(ExecutionManager::thunk);
	}
	else
	{
		std::cout << "could not find sequence to run(str) - " << ws2s(name) << std::endl;
	}
}

void SequenceManager::run_sequence(string_t name, std::function<void()> done)
{
	if (_sequences.find(name) != _sequences.end())
	{
		auto last_period = name.find_last_of('.');
		if (last_period != string_t::npos)
			_sequences[name]->run(done, name.substr(0, last_period));
		else
			_sequences[name]->run(done);
	}
	else
	{
		std::cout << "could not find sequence to run (str, func) - " << ws2s(name) << std::endl;
		ExecutionManager::RunDeferred(done);
	}
}

void SequenceManager::loop_sequence(string_t name)
{
	if (_sequences.find(name) != _sequences.end())
	{
		auto last_period = name.find_last_of('.');
		if (last_period != string_t::npos)
			_sequences[name]->loop(name.substr(0, last_period));
		else
			_sequences[name]->loop();
	}
	else
	{
		std::cout << "could not find sequence to loop - " << ws2s(name) << std::endl;
	}
}

void SequenceManager::stop_sequence(string_t name)
{
	if (_sequences.find(name) != _sequences.end())
	{
		_sequences[name]->stop();
	}
	else
	{
		std::cout << "could not find sequence to stop - " << ws2s(name) << std::endl;
	}
}
