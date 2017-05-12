#include "FCFS.hpp"
FCFS::FCFS ()
	:Scheduler()
{
}
		
// Enqueues a process in the ready queue
void FCFS::Enqueue (PCB* pcb)
{
	readyQueue.push_back(pcb);
}
		
// Dequeues a process from the ready queue to run on the processor
PCB* FCFS::Dequeue ()
{
/* TODO: Returns the PCB* that should run on the processor 
* next. Returns NULL when the readyQueue is empty.(5 pts)
*/ 
	if(readyQueue.empty()){
		return NULL;
	}
	else{
		PCB* old = readyQueue.front();
		readyQueue.pop_front();
		return old;
	}
}

// Processes a tick, which updates PCB waiting statistics  
void FCFS::ProcessTick()
{
	/* TODO: Add one wait tick to every PCB in the readyQueue.(5pts)
	*/ 
	for(int i = 0; i < readyQueue.size(); i++){
		readyQueue[i]->_waitTicks += 1;
	}
}
		
// Returns the number of PCBs in the ready queue
int FCFS::PCBCount()
{
	return readyQueue.size();
}

// Returns true if the scheduler is preemptive
bool FCFS::IsPremptive()
{
	// TODO: Return the appropriate value for FCFS. (1 pts)
	return false;
}
		
// Returns true when it is time to interrupt the running process
bool FCFS::PreemptProcess()
{
	// TODO: Returns the appropriate value for FCFS schedulers. (1 pts)
	return false;
}
