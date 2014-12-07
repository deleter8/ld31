#if defined(_WIN32) || defined(WIN32)
#include <codecvt>
#endif

#include "string_t.h"
#include "fstream_t.h"
#include "ScriptDef.h"


ScriptDef * load_script(string_t filename)
{
	ifstream_t infile(filename);

#if defined(_WIN32) || defined(WIN32)
	infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
#endif
	
	stringstream_t ss;
	ss << infile.rdbuf();
	auto script_string =  ss.str();

	return new ScriptDef(TEXT("TODO!!!"));
}

ScriptDef::ScriptDef(string_t thing)
{
	line_def = thing;
}