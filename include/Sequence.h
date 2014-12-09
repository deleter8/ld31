#ifndef __SEQUENCE_GODZILLA__
#define __SEQUENCE_GODZILLA__

#include <list>
#include <functional>

#include "ScriptScope.h"
#include "ScriptRaw.h"


class SequenceStep
{
private:
	int count;
public:
	SequenceStep();
	script_async_statement_list_t async_statements;

	void run(std::function<void()> done, string_t target = TEXT(""));
};

class Sequence
{
private:
	bool stop_looping;
	std::list<SequenceStep*> _steps;
	void loop_helper();
	bool _already_running;
public:
	Sequence();
	void run(std::function<void()> done, string_t target = TEXT(""));
	void loop();
	void stop();//not instantaneous, has to wait for loop to come to end of _steps

	ScriptScope * build_sequence(ScriptRaw *);
};

#endif //__SEQUENCE_GODZILLA__