/* A RR scheduler implementation
 */ 
#ifndef RR_H
#define RR_H
#include "FCFS.hpp"


class RR : public FCFS
{
private:
	
	// This implements the RR scheduler's quantum. 
	// The active process is preempted when the timer expires
	class QuantumTimer
	{
	private:
		int _timer;   // Number of ticks left in the quantum
		int _quantum; // The time quantum (number of ticks) the timer is reset
		
	public:

		QuantumTimer (int quantum);
		
		// Resets the timer.
		void Reset ();
		
		// True when there are no ticks left in the quantum
		bool IsExpired ();
	
		// Decrement the quantum 
		void ProcessTick ();
	};

	QuantumTimer _timer;

public:
	RR();

	RR(int quantum);
	
	// Dequeues a process from the ready queue to run on the processor
	virtual PCB* Dequeue ();
	
	// Processes a tick, which updates PCB waiting statistics  
	virtual void ProcessTick();

	// Returns true if the scheduler is preemptive
	virtual bool IsPremptive();
	
	virtual bool PreemptProcess();
};

#endif
