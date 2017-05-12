#include "Processor.hpp"

	
Processor::Processor ()
{
	// Processor is idle when _activeProcess is NULL.
	_activeProcess = NULL;
}

bool Processor::IsIdle()
{
	bool result = false;
	
	// Processor is idle when no process is active or the active process 
	// has completed its cpu burst.
	if (_activeProcess == NULL || _activeProcess->IsProcessDone())
		result = true;
		
	return result;
}

bool Processor::ProcessTick()
{
	bool isProcessDone = false;
	
	// Return true when processor is idle.
	if (_activeProcess == NULL)
	{
		isProcessDone = true;
		return isProcessDone;
	}
	
	// Perform a tick of work. 
	int burstLeft = _activeProcess->ProcessCPUTick();
		
	// The process is done with the processor when there are no burst ticks left
	if (burstLeft <= 0)
	{
		isProcessDone = true;
	}
	
	return isProcessDone;
}

void Processor::ActivateProcess(PCB* pcb)
{	

	// Make the pcb the active process
	_activeProcess = pcb;
	_activeProcess->ActivateProcess();
}

PCB* Processor::DeactivateProcess()
{
	// Deactivate the active process
	PCB* tempProcess = _activeProcess;
	_activeProcess = NULL;
	return tempProcess;
}