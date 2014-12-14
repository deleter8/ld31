#if defined(_WIN32) || defined(WIN32)
#include <codecvt>
#endif

#include <list>
#include <algorithm>
#include <iostream>

#include "string_t.h"
#include "fstream_t.h"
#include "ScriptRaw.h"

string_t restore_string_if_needed(string_t from)
{
	if (from.size() == 0) return from;
	if (from[0] != '"') return from;
	
	std::replace(from.begin(), from.end(), '\x1', ' ');
	return from.substr(1, from.size() - 2);
};

ScriptRaw * load_script(string_t filename)
{
	ifstream_t infile(filename);

#if defined(_WIN32) || defined(WIN32)
	infile.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
#endif
	
	stringstream_t ss;
	ss << infile.rdbuf();
	string_t script_string =  ss.str();

	auto lines = split(script_string, '\n');
	int current_scope_count = 0;
	auto scope = std::list<ScriptRaw*>();
	scope.push_back(new ScriptRaw(TEXT("root")));
	scope.back()->vals = (ActionVal*)&ActionVal::EMPTY();

	bool was_error = false;

	for (string_t line : lines)
	{
		bool is_comment = false;
		bool not_just_spaces = false;
		int space_count = 0;
		//just fyi to the future: this *might* not handle really weird cases with unicode
		// with all the char comparison shtuff
		int char_count = 0;
		
		auto start_string = line.end();
		
		for (auto it = line.begin(); it != line.end(); it++)
		{
			auto c = *it;
			if (c == '#')
			{
				if (!not_just_spaces)
				{
					is_comment = true;
				}
				else
				{
					line = line.substr(0, char_count);
					break;
				}
			}
			if (c != ' ' && c != '\t') not_just_spaces = true; 
			else if (!not_just_spaces) space_count++;
			if (!not_just_spaces && c == '\t') space_count++; //tabs == 2 spaces

			if (c == '"')
			{
				//string should now be terminating
				if (start_string != line.end())
				{
					std::replace(start_string, it, ' ', '\x1');
					start_string = line.end();
				}
				else
				{
					start_string = it;
				}
			}

			char_count++;
		}
		if (line.length() > 0 && not_just_spaces && !is_comment)
		{
			if (space_count % 2 != 0)
			{
				was_error = true;
				break;
			}
			if (space_count > current_scope_count + 2)
			{
				was_error = true;
				break;
			}

			auto things = split(line.substr(space_count), ' ');
			
			ScriptRaw * script_line = new ScriptRaw(things[0]);

			ActionVal * previous = NULL;
			if (things.size() > 1)
			{
				previous = new ActionVal();
				previous->vals = restore_string_if_needed(things[1]);
				script_line->vals = previous;
			}
			
			if (things.size() > 2)
			{
				for (auto it = things.begin() + 2; it != things.end(); it++)
				{
					auto action_val = new ActionVal();
					action_val->vals = restore_string_if_needed(*it);
					
					previous->next = std::shared_ptr<ActionVal>(action_val);

					//technically cheating the shared_ptr a little bit here but that
					//is ok because this is a local ref vs the ActionVal chain that
					//is passed on
					previous = action_val;
				}
			}
			
			if (things.size() < 2)
			{
				script_line->vals = (ActionVal*) &ActionVal::EMPTY();
			}
			
			if (space_count < current_scope_count)
			{
				int scopes_to_lose = (current_scope_count - space_count) / 2;
				for (auto i = 0; i < scopes_to_lose; i++) scope.pop_back();
			}
			else if (space_count > current_scope_count)
			{
				scope.push_back(scope.back()->inner_lines.back());
			}
			
			scope.back()->inner_lines.push_back(script_line);

			current_scope_count = space_count;
		}
	}

	if (was_error)
	{
		std::cout << "there was an error while loading script" << std::endl;
	}
	//TODO: detect and indicate error when someone starts a file with a tabbed entry
	//TODO: check error thing
	return scope.front();
}

ScriptRaw::ScriptRaw(string_t thing)
{
	line_def = thing;
}