#include "Transition.h"

Transition::Transition(float from, float to, int duration_ticks)
{
	_from = from;
	_val = from;
	_to = to;
	_total_ticks = duration_ticks;
}

void Transition::run(int ticks)
{
	if (_current_ticks >= _total_ticks) return;

	for (int i = 0; i < ticks; i++)
	{
		_current_ticks++;
		_val = tfunc(_from, _to, _val, (float)_current_ticks / (float)_total_ticks);

		if (_current_ticks == _total_ticks)
		{
			on_done();
			return;
		}		
	}
}

bool Transition::done()
{
	return _current_ticks >= _total_ticks;
}