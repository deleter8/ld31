#ifndef __TRANSITION_GODZILLA__
#define __TRANSITION_GODZILLA__

#include <functional>

class Transition
{
private:

	float _from;
	float _to;
	float _val;
	int _current_ticks;
	int _total_ticks;

public:

	Transition(float from, float to, int duration_ticks);

	void run(int ticks);

	bool done();

	std::function<float(float, float, float, float)> tfunc;

	std::function<void(float)> on_step;

	std::function<void()> on_done;

};

#endif //__TRANSITION_GODZILLA__