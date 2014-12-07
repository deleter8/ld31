#include <sstream>
#include <fstream>
#include <codecvt>

#include "ScriptDef.h"


ScriptDef * load_script(string_t filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	auto script_string =  wss.str();

	return new ScriptDef(TEXT("TODO!!!"));
}