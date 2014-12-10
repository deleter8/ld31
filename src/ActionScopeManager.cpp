#include "ActionScopeManager.h"


ContextManager * ActionScopeManager::_context_manager = NULL;
Context * ActionScopeManager::_current_context = NULL;

void ActionScopeManager::init(ContextManager * context_manager)
{
	_context_manager = context_manager;
}

void ActionScopeManager::set_scope(string_t name)
{
	_current_context = _context_manager->get_context(name);
}

void ActionScopeManager::set_scope(Context * context)
{
	_current_context = context;
}

string_t ActionScopeManager::get_name()
{
	return _current_context->get_name();
}

std::function<void(float)> ActionScopeManager::get_step(string_t attrib)
{
	return _current_context->get_step(attrib);
}
