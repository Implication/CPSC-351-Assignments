/* A first-come-first-serve scheduler implementation
 */
#ifndef FCFS_H
#define FCFS_H

#include <cstddef>
#include <list>
#include "Scheduler.hpp"
#include <deque>

using namespace std;

class FCFS : public Scheduler
{
protected:
	deque<PCB*> readyQueue;
	 
public:
		FCFS ();
		
		// Adds a process in the ready queue
		virtual void Enqueue (PCB* pcb);
		
		// Dequeues a process from the ready queue to run on the processor
		virtual PCB* Dequeue ();

		// Processes a tick, which updates PCB waiting statistics  
		virtual void ProcessTick();
		
		// Returns the number of PCBs in the ready queue
		virtual int PCBCount();

		// Returns true if the scheduler is preemptive
		virtual bool IsPremptive();
		
		// Returns true when it is time to interrupt the running process
		virtual bool PreemptProcess();
};

#endif //FCFS_H
