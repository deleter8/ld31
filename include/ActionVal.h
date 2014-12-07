#ifndef __ACTION_VAL__
#define __ACTION_VAL__

#include <memory>

#include "string_t.h"

class ActionVal
{
private:
	static ActionVal empty;
public:
	ActionVal();
	ActionVal(std::shared_ptr<ActionVal>&);
	int vali();
	float valf();
	string_t vals;
	std::shared_ptr<ActionVal> next;

	static const ActionVal& EMPTY();
};

#endif //__ACTION_VAL__