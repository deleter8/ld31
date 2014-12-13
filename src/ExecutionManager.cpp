#include "ExecutionManager.h"
#include <iostream>

std::function<void()> ExecutionManager::thunk = [](){};
bool ExecutionManager::_executing = false;

std::list<std::function<void()>> ExecutionManager::_stuff = std::list<std::function<void()>>();
std::list<std::function<void()>> ExecutionManager::_deffered_stuff = std::list<std::function<void()>>();
std::list<std::function<void()>> ExecutionManager::_more_stuff = std::list<std::function<void()>>();

//if something is enqueued as runimmediate and it itself enqueues
//something as runimmediate that something will also run before the
//event loop turns. this could create an infinitely blocking loop
//implementation may keep a running timer and at some point defer
//execution of the rest of hte stack until after the next turn
void ExecutionManager::RunImmediate(std::function<void()> func)
{
	if (_executing)
	{
		_more_stuff.push_back(func);
	}
	else
	{
		_stuff.push_back(func);
	}
}


//calls put to run deferred will run at the start of the next
//run of execution manager
void ExecutionManager::RunDeferred(std::function<void()> func)
{
	if (_executing)
	{
		_deffered_stuff.push_back(func);
	}
	else
	{
		_stuff.push_back(func);
	}
}

//todo: eventually try more robust impl that can time out and stop taking stuff
//from more_stuff
void ExecutionManager::execute()
{
	_executing = true;
	
	do
	{
		for (auto more : _more_stuff) _stuff.push_back(more);
		_more_stuff.clear();

		for (auto stuff : _stuff) stuff();
		_stuff.clear();

	} while (_more_stuff.size() > 0);
	
	for (auto deffered : _deffered_stuff) _stuff.push_back(deffered);
	_deffered_stuff.clear();

	_executing = false;
}
