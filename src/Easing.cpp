#include "Easing.h"
#include "ExecutionManager.h"


Easing::Easing(float from, float to, int duration_ticks)
{
	_from = from;
	_val = from;
	_to = to;
	_total_ticks = duration_ticks;
	_current_ticks = 0;
}

void Easing::run(int ticks)
{
	if (_current_ticks >= _total_ticks) return;
	
	_current_ticks += ticks;
	if (_current_ticks > _total_ticks) _current_ticks = _total_ticks;

	_val = tfunc(_from, _to, _val, (float)_current_ticks / (float)_total_ticks);
	on_step(_val);
		

	if (_current_ticks >= _total_ticks)
	{
		ExecutionManager::RunDeferred(on_done);
		return;
	}		
	
}

bool Easing::is_done()
{
	return _current_ticks >= _total_ticks;
}