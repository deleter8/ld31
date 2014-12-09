#ifndef __HEADS_SHALL_ROLLLLLL__
#define __HEADS_SHALL_ROLLLLLL__

#include <list>
#include <functional>


class ExecutionManager
{
private:
	static bool _executing;
	static std::list<std::function<void()>> _stuff;
	static std::list<std::function<void()>> _deffered_stuff;
	static std::list<std::function<void()>> _more_stuff;

public:

	//if something is enqueued as runimmediate and it itself enqueues
	//something as runimmediate that something will also run before the
	//event loop turns. this could create an infinitely blocking loop
	//implementation may keep a running timer and at some point defer
	//execution of the rest of hte stack until after the next turn
	static void RunImmediate(std::function<void()> func);

	//calls put to run deferred will run at the start of the next
	//run of execution manager
	static void RunDeferred(std::function<void()> func);

	static void execute();

	static std::function<void()> thunk;

};

#endif //__HEADS_SHALL_ROLLLLLL__