/*
 * Scheduler is an abstract base class used to define an interface for 
 * all schedulers.
 */ 

#ifndef Scheduler_H
#define Scheduler_H

#include "PCB.hpp"

using namespace std;

class Scheduler
{
	
public:
		// Adds a new PCB into the ready queue
		virtual void Enqueue (PCB* pcb) = 0;

		// Remove a PCB from the ready queue to run on the processor
		virtual PCB* Dequeue () = 0;

		// Updates PCB statistics evey tick 
		virtual void ProcessTick() = 0; 
		
		// Returns the number of PCBs in the ready queue
		virtual int PCBCount() = 0;
		
		// Returns true if scheduler is preemptive 
		virtual bool IsPremptive() = 0;

		// Returns true when it is time to preempt an active process
		virtual bool PreemptProcess() = 0;
};

#endif