#include "ResourceManager.h"
#include "ContextManager.h"

ContextManager::ContextManager()
{
	_contexts = std::unordered_map<string_t, Context*>();
	_actions = std::unordered_map<string_t, std::function<ActionVal(ActionVal)>>();

	_actions[TEXT("seti")] = [this](ActionVal val){
		this->seti(val.vals, val.next->vali());
		return ActionVal::EMPTY();
	};
	_actions[TEXT("setf")] = [this](ActionVal val){
		this->seti(val.vals, val.next->valf());
		return ActionVal::EMPTY();
	};
	_actions[TEXT("sets")] = [this](ActionVal val){
		this->sets(val.vals, val.next->vals);
		return ActionVal::EMPTY();
	};
	_actions[TEXT("set_default_font")] = [](ActionVal val){
		ResourceManager::set_default_font(val.vals);
		return ActionVal::EMPTY();
	};
}

void ContextManager::add_context(string_t name, Context * context)
{
	_contexts[name] = context;
}

void ContextManager::add_action(string_t name, std::function<ActionVal(ActionVal)> handler)
{
	_actions[name] = handler;
}

int ContextManager::geti(string_t name)
{
	return _int_vals[name];
}

float ContextManager::getf(string_t name)
{
	return _float_vals[name];
}

string_t ContextManager::gets(string_t name)
{
	return _string_vals[name];
}

void ContextManager::seti(string_t name, int value)
{
	_int_vals[name] = value;
}

void ContextManager::setf(string_t name, float value)
{
	_float_vals[name] = value;
}

void ContextManager::sets(string_t name, string_t value)
{
	_string_vals[name] = value;
}
