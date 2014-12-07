#include "ActionVal.h"

ActionVal ActionVal::empty = ActionVal();

ActionVal::ActionVal()
{
	//next = std::shared_ptr<ActionVal>();
	vals = TEXT("");
}

ActionVal::ActionVal(std::shared_ptr<ActionVal>& action) : ActionVal()
{
	//next = std::shared_ptr<ActionVal>(action);
}

int ActionVal::vali()
{
	return stoi(vals);
}

float ActionVal::valf()
{
	return stof(vals);
}

const ActionVal& ActionVal::EMPTY()
{
	return empty;
}