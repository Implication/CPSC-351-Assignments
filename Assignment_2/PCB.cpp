#include "PCB.hpp"

int PCB::_idcounter = 0;

PCB::PCB()
{
    _id = _idcounter++;
	_burstsRemaining = 10;
    _CPUBurstTicks = 0;
	_CPUTicksRemaining = 0;
	_waitTicks = 0;
	_workTicks = 0;
	_responseTicks = -1;
}

PCB::PCB(int numBursts, int burstTicks)
	:_burstsRemaining(numBursts), _CPUBurstTicks(burstTicks)
{
	_id = _idcounter++;
	_CPUTicksRemaining = _CPUBurstTicks;
	_waitTicks = 0;
	_workTicks = 0;
	_responseTicks = -1;
}

PCB::PCB(int burstTicks)
	:_burstsRemaining(1), _CPUBurstTicks(burstTicks)
{
	_id = _idcounter++;
	_CPUTicksRemaining = _CPUBurstTicks;
	_waitTicks = 0;
	_workTicks = 0;
	_responseTicks = -1;
}

int PCB::ProcessCPUTick()
{
	// A tick of work was completed
	_workTicks++;
    return _CPUTicksRemaining -= 1;
}

int PCB::NextCPUBurst ()
{
	// When no bursts remain there is no work left to do
	if (_burstsRemaining <= 0)
		return 0;
	
	// Reduce the number of bursts left in this process
	// and set the number of CPUTicks remaining to the 
	// number of ticks in the cpu burst
	_burstsRemaining--;
	_CPUTicksRemaining = _CPUBurstTicks;
	return _CPUTicksRemaining;
}

bool PCB::IsProcessDone()
{
	// A process has completed its work when there are no bursts
	// remaining and there is no work left in the current burst. 
	return _burstsRemaining <= 0 && _CPUTicksRemaining <= 0;
}

void PCB::ActivateProcess ()
{
	// _waitTicks is equal to the response time when the process
	// is activated for the first time.
	if (_responseTicks < 0)
		_responseTicks = _waitTicks;
	
	// Sets up the next burst provided the prior burst
	// was completed and there are more cpu bursts.
	if (!IsProcessDone())
		NextCPUBurst();
}

int PCB::ProcessID()
{
	return _id;
}
