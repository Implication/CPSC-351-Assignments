/* A simulated CPU
 */ 
#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <unistd.h>
#include <iostream>
#include "PCB.hpp"

using namespace std;

class Processor
{
private:
	// The PCB actively running on this processor
	PCB* _activeProcess;	
	
public:
	Processor ();
	
	// Processor performs a tick of work. 
	// Returns true when the process has finished its cpu burst
	bool ProcessTick();
	
	// Runs a process on the processor
	void ActivateProcess(PCB* pcb);
	
	// Switches a process off the processor and returns it. 
	PCB* DeactivateProcess();
	
	// True when a process is not running on the processor
	bool IsIdle();
};

#endif