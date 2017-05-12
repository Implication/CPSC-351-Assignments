#include <iostream>
#include <signal.h>
#include <sys/time.h>
#include <vector>
#include <iomanip>

#include "Processor.hpp"
#include "FCFS.hpp"
#include "RR.hpp"
#include <deque>

using namespace std;

/* 
 * TODO: Implement a list of pointers to PCB structures called
 * done_processes. Processes are stored in this list when they complete 
 * their final cpu burst.The PCB structures contain 
 * statistics that will be updated by the simulated schedulers and processor.
 * You may implement the list using a data structure from the standard
 * library or by building your own. The list should be able to 
 * hold an arbitrary number of processes. (5 pts) 
 */  


int context_switches = 0;
deque<PCB*> done_processes;
// Adds a completed PCB to done_processes
void AddDonePCB (PCB* pcb)
{
	done_processes.push_front(pcb);
}

// Removes all PCBs from done_processes
void ClearDonePCBs()
{
		while(!done_processes.empty()){
			done_processes.pop_back();	
		}
}

// Calculates the turnaround time for a process
float ProcessTurnaroundTime(PCB* pcb)
{
	/*
	 * TODO: Turnaround time is the total time a process is in the system.
	 * This is measured from the time it enters the ready queue to the
	 * the time it finishes its final CPU burst. It is calculated
	 * by adding the total ticks of work to the total ticks it spent 
	 * waiting. (5 pts)
	 */
	float ptat = 0; //Value for the ProcessTurnAroundTime
	ptat = pcb->_workTicks + pcb->_waitTicks;
	return ptat;
}

// Returns the average turnaround time for all processes 
float AverageTurnaroundTime()
{
	/*
	 * TODO: Returns the average of all process turnaround times
	 * by looping through done_processes and calling ProcessTurnAroundTime
	 * on each PCB. This total is divided by the number of PCBs.(5 pts)
	 */
	 float atat = 0; //Value for the Average turn around time
	 for(int i = 0; i < done_processes.size(); i++){
		atat += ProcessTurnaroundTime(done_processes[i]);
	}
	atat /= done_processes.size();
	 return atat;
}

float AverageResponseTime ()
{
	/*
	 * TODO: Returns the average of all done_processes response times
	 * by looping through each PCB to total up 
	 * all response times and dividing it by the number of PCBs.(5 pts)
	 */
	float art = 0; //Value for the average response times
	for(int i = 0; i < done_processes.size(); i++){
		art += done_processes[i]->_responseTicks;
	}
	art /= done_processes.size();
	 return art;
}

// Returns the average wait time for all processes
float AverageWaitTime()
{
	/*
	 * TODO: Calculates the average wait time
	 * by looping through done_processes and adding up the wait ticks for each
	 * PCB. This total wait time is divided by the total number of PCBs.(5 pts)
	 */
	float awt = 0; //Average wait time total
	for(int i = 0; i < done_processes.size(); i++){
		awt += done_processes[i]->_waitTicks;	
	}
	awt /= done_processes.size();
	 return awt;
}

// Returns the number of context switches that occured during the trial
int ContextSwitches()
{
	return context_switches;
}

// Returns the total number of ticks that were required to run all of the processes to completion.
float TotalWorkTime()
{
	/*
	 * TODO:Calculates the total time by looping through
	 * done_processes and adding up PCB work ticks. (5 pts)
	 * 
	 */
	float twT = 0; //Value of the total work time
	for(int i = 0; i < done_processes.size(); i++){
			twT += done_processes[i]->_workTicks;
	}
	return twT;
}

// Prints the end statistics for each simulation trial
void PrintStatistcs()
{
	cout << "Total Work Time: " << TotalWorkTime() << endl;
	cout	 << "Avg. Turnaround Time: " << AverageTurnaroundTime() << endl;
	cout	 << "Avg. Wait Time: " << AverageWaitTime() << endl;
	cout	 << "Avg. Response Time: " << AverageResponseTime() << endl;
	cout	 << "Context Switches: " << ContextSwitches() << endl;
	cout	 << endl;
}

// Adds PCBs with short cpu bursts to the simulation
void AddShortBurstProcesses(Scheduler* sched)
{
	sched->Enqueue(new PCB (8));
	sched->Enqueue(new PCB (3));
	sched->Enqueue(new PCB (5));
	sched->Enqueue(new PCB (4));
	sched->Enqueue(new PCB (1));
	sched->Enqueue(new PCB (2));
	sched->Enqueue(new PCB (4));
	sched->Enqueue(new PCB (6));
	sched->Enqueue(new PCB (3));
	sched->Enqueue(new PCB (7));
}

// Adds PCBs with long cpu bursts to the simulation 
void AddLongBurstProcesses(Scheduler* sched)
{
	sched->Enqueue(new PCB (14));
	sched->Enqueue(new PCB (24));
	sched->Enqueue(new PCB (32));
	sched->Enqueue(new PCB (64));
	sched->Enqueue(new PCB (80));
	sched->Enqueue(new PCB (26));
	sched->Enqueue(new PCB (45));
	sched->Enqueue(new PCB (36));
	sched->Enqueue(new PCB (23));
	sched->Enqueue(new PCB (28));
}

// Adds PCBs from the slide examples about calculating average wait time for FCFS and RR
void AddTestProcesses (Scheduler* sched)
{
	sched->Enqueue(new PCB (24));
	sched->Enqueue(new PCB (3));
	sched->Enqueue(new PCB (3));
}

// Performs a context switch in the simulated environment
void ContextSwitch(Scheduler* sched, Processor & cpu)
{
	PCB* oldPCB = cpu.DeactivateProcess ();
					
	if (oldPCB != NULL)
	{
		if (false == oldPCB->IsProcessDone())
			sched->Enqueue(oldPCB);
		else
			AddDonePCB(oldPCB);	
	}
	
	PCB* newPCB = sched->Dequeue();
	
	if (newPCB != NULL)
	{
		context_switches++;
		cpu.ActivateProcess(newPCB);
	}
}

// Executes the simulation
void RunSimulation (Scheduler* sched)
{
	// The simulated processor
	Processor cpu;
					
	bool go = true;
	context_switches = 0;
	
	// go is true as long as there are processes with work to do.
	while (go)
	{
		// Need to context switch when either the processor is idle or the scheduler decides its necessary 
		if (cpu.IsIdle() || sched->PreemptProcess())
		{
			ContextSwitch(sched, cpu);
		}
		
		// Tick the clock for the environment
		cpu.ProcessTick(); 
		sched->ProcessTick();
		// Assume the simulation is over when the scheduler's ready queue is empty and cpu is idle 
		if (sched->PCBCount() <= 0 && cpu.IsIdle())
		{
			AddDonePCB(cpu.DeactivateProcess ());
			go = false;
		}
	}
}

void fcfs_test()
{
	ClearDonePCBs();
	
	cout << "FCFS Test Run" << endl;
	
	FCFS fcfs_scheduler;
	
	AddTestProcesses(&fcfs_scheduler);
	RunSimulation(&fcfs_scheduler);
	PrintStatistcs();
}

void rr_test()
{
	ClearDonePCBs();
	
	cout << "RR Test Run: quantum = 4" << endl;
	
	RR rr_scheduler(4);
	
	AddTestProcesses(&rr_scheduler);
		
	RunSimulation(&rr_scheduler);
	
	PrintStatistcs();
}

void fcfs_trial1 ()
{	
	ClearDonePCBs();
	
	cout << "FCFS Trial 1: short burst processes"  << endl;
	
	FCFS fcfs_scheduler;
	
	AddShortBurstProcesses(&fcfs_scheduler);
		
	RunSimulation(&fcfs_scheduler);
	
	PrintStatistcs();
}

void fcfs_trial2 ()
{
	ClearDonePCBs();
	
	cout << "FCFS Trial 2: long burst processes" << endl;
	
	FCFS fcfs_scheduler;
	
	AddLongBurstProcesses(&fcfs_scheduler);
		
	RunSimulation(&fcfs_scheduler);
	
	PrintStatistcs();
}

void fcfs_trial3 ()
{
	ClearDonePCBs();
	
	cout << "FCFS Trial 3: short burst processes followed by long burst processes" << endl;
	
	FCFS fcfs_scheduler;
	
	AddShortBurstProcesses(&fcfs_scheduler);
	AddLongBurstProcesses(&fcfs_scheduler);
	
	RunSimulation(&fcfs_scheduler);
	
	PrintStatistcs();
}

void fcfs_trial4 ()
{
	ClearDonePCBs();
	
	cout << "FCFS Trial 4: long burst processes followed by short burst processes" << endl;
	
	FCFS fcfs_scheduler;
	
	AddLongBurstProcesses(&fcfs_scheduler);
	AddShortBurstProcesses(&fcfs_scheduler);
		
	RunSimulation(&fcfs_scheduler);
	
	PrintStatistcs();
}

void rr_trial1()
{	
	ClearDonePCBs();
	
	cout << "RR Trial 1: quantum = 10, short burst processes" << endl;
	
	RR rr_scheduler(10);
	
	AddShortBurstProcesses(&rr_scheduler);
		
	RunSimulation(&rr_scheduler);
	
	PrintStatistcs();
}

void rr_trial2()
{	
	ClearDonePCBs();
	
	cout << "RR Trial 2: quantum = 10, long burst processes" << endl;
	
	RR rr_scheduler(10);
	
	AddLongBurstProcesses(&rr_scheduler);
		
	RunSimulation(&rr_scheduler);
	
	PrintStatistcs();
}

void rr_trial3()
{	
	ClearDonePCBs();
	
	cout << "RR Trial 3: quantum = 4, short burst processes" << endl;
	
	RR rr_scheduler(4);
	
	AddShortBurstProcesses(&rr_scheduler);
		
	RunSimulation(&rr_scheduler);
	
	PrintStatistcs();
}

void rr_trial4()
{	
	ClearDonePCBs();
	
	cout << "RR Trial 4: quantum = 4, short burst processes followed by long burst processes" << endl;
	
	RR rr_scheduler(4);
	
	AddShortBurstProcesses(&rr_scheduler);
	AddLongBurstProcesses(&rr_scheduler);
		
	RunSimulation(&rr_scheduler);
	
	PrintStatistcs();
}

void rr_trial5()
{
	ClearDonePCBs();
	
	cout << "RR Trial 5: quantum = 4, long burst processes followed by short burst processes" << endl;
	
	RR rr_scheduler(4);
	
	AddLongBurstProcesses(&rr_scheduler);
	AddShortBurstProcesses(&rr_scheduler);
		
	RunSimulation(&rr_scheduler);
	
	PrintStatistcs();
}


int main()
{	
	
	fcfs_test();   // This is the small set of processes from the slides. Used as a basic test.
	rr_test();     // This is the small set of processes from the slides. Used as a basic test. 
	fcfs_trial1(); // FCFS: short burst processes 
	fcfs_trial2(); // FCFS: long burst processes
	fcfs_trial3(); // FCFS: short burst processes followed by long burst processes 
	fcfs_trial4(); // FCFS: long burst processes followed by short burst processes
	rr_trial1();   // RR: quantum = 10, short burst processes 
	rr_trial2();   // RR: quantum = 10, long burst processes
	rr_trial3();   // RR: quantum = 4 , short burst processes
	rr_trial4();   // RR: quantum = 4 short burst processes followed by long burst processes 
	rr_trial5();   // RR: quantum = 4, long burst processes followed by short burst processes

	cout << "Press enter to exit." << endl;
	cin.get();

	return 0;
}
