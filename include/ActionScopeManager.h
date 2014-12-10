#ifndef __ACTION_SCOPE_MANAGER__
#define __ACTION_SCOPE_MANAGER__

#include <functional>

#include "string_t.h"
#include "Context.h"
#include "ContextManager.h"

class ActionScopeManager
{
private:
	static ContextManager * _context_manager;
	static Context * _current_context;
	
public:
	static void init(ContextManager * context_manager);

	static void set_scope(string_t name);
	static void set_scope(Context * context);

	static string_t get_name();

	static std::function<void(float)> get_step(string_t attrib);
};

#endif //__ACTION_SCOPE_MANAGER__