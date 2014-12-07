#ifndef __SCRIPT_RAW__
#define __SCRIPT_RAW__

#include <list>

#include "string_t.h"
#include "ActionVal.h"

class ScriptRaw
{
public:
	ScriptRaw(string_t thing);
	string_t line_def;
	ActionVal * vals;

	std::list<ScriptRaw*> inner_lines;

};

ScriptRaw * load_script(string_t filename);

#endif //__SCRIPT_RAW__