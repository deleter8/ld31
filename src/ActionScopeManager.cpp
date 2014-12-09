#include "ActionScopeManager.h"


ContextManager * ActionScopeManager::context_manager = NULL;
Context * ActionScopeManager::current_context = NULL;

void ActionScopeManager::init(ContextManager * context_manager)
{
	context_manager = NULL;
}

void ActionScopeManager::set_scope(string_t name)
{
	current_context = context_manager->get_context(name);
}

void ActionScopeManager::set_scope(Context * context)
{
	current_context = context;
}

string_t ActionScopeManager::get_name()
{
	return current_context->get_name();
}

std::function<void(float)> ActionScopeManager::get_step(string_t attrib)
{
	return current_context->get_step(attrib);
}
