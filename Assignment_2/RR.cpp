#include "RR.hpp"

RR::QuantumTimer::QuantumTimer (int quantum)
	:_quantum(quantum)
{
	Reset ();
}

void RR::QuantumTimer::Reset ()
{
	_timer = _quantum;
}
	
bool RR::QuantumTimer::IsExpired ()
{
	if (_timer <= 0)
		return true;
			
	return false;
}
	
void RR::QuantumTimer::ProcessTick ()
{
	_timer--;
}

RR::RR()
	:FCFS(), _timer(4)
{
}

RR::RR(int quantum)
	:FCFS(), _timer(quantum)
{
}
	
PCB* RR::Dequeue ()
{
	/* TODO: Reset the quantum timer and return the next process
	 * to run on the processor. (5 pts)
	 */ 
	_timer.Reset();
	if(readyQueue.empty())
		return NULL;
	else{
		PCB* old = readyQueue.front();
		readyQueue.pop_front();
		return old;
	}
}
	
void RR::ProcessTick()
{
	/* TODO: apply a tick to the quantum timer
	 * and the processes in the ready queue. (5 pts)
	 */
	_timer.ProcessTick();
	for(int i = 0; i < readyQueue.size(); i++)
		readyQueue[i]->_waitTicks += 1;
}

bool RR::IsPremptive()
{
	// TODO: Return the correct value for RR. (5 pts)
	return true;
}
	
bool RR::PreemptProcess()
{
	// TODO: Return true when the scheduler is premptive and the
	// quantum timer has expired. (5 pts)
	if(IsPremptive() && _timer.IsExpired())
		return true;
	else
		return false;
}
