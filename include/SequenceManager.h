#ifndef __SEQUENCE_MANAGER__
#define __SEQUENCE_MANAGER__

#include <functional>

#include "Sequence.h"


class SequenceManager
{
private:
	std::unordered_map<string_t, Sequence*> _sequences;

public:

	SequenceManager();

	void add_sequence(string_t name, Sequence * sequence);

	void run_sequence(string_t name);
	void run_sequence(string_t name, std::function<void()> done);
	void loop_sequence(string_t name);
	void stop_sequence(string_t name);
};


#endif //__SEQUENCE_MANAGER__