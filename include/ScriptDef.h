#ifndef __SCRIPT_DEF__
#define __SCRIPT_DEF__

#include <list>

#include "string_t.h"
#include "ActionVal.h"

class ScriptDef
{
public:
	ScriptDef(string_t thing);
	string_t line_def;
	ActionVal * vals;

	std::list<ScriptDef*> inner_lines;

};

ScriptDef * load_script(string_t filename);

#endif //__SCRIPT_DEF__