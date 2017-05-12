/*
 * Implements a Process Control Block for the simulated system
 * It contains runtime stats for processes and simulates the 
 * behavior of a process when running on a processor. 
 */ 

#ifndef PCB_H
#define PCB_H

class PCB
{
private:
		
		// Used to assign each process a unique ID. 
		static int _idcounter;

        // Simulated statistics: the ticks remaining in the cpu burst
		int _CPUTicksRemaining;
		
		// Simulated statistics: number of cpu bursts remaining before process completes its work  
		int _burstsRemaining;
		
		// Simulated statistics: total number of ticks for each cpu burst
		int _CPUBurstTicks;
		
		// Returns the number of ticks for the next cpu burst
		int NextCPUBurst ();
public:
		// The pid
		int _id;
		
		// Number of ticks the process was waiting in the ready queue
		int _waitTicks;
		
		// Number of ticks the process was active on the processor
		int _workTicks;
		
		// The time between entering the ready queue and becoming active on the processor 
		// for the first time
		int _responseTicks;
		
		// Constructors
		PCB();
		PCB(int numBursts, int burstTicks);
		PCB(int burstTicks);
		
		// Called every tick a process is active on a processor
		int ProcessCPUTick();
		
		// Prepares the process to run on a processor
		void ActivateProcess ();
		
		// Returns the pid for the process
		int ProcessID();
		
		// Returns true when the process is no longer able to remain active
		bool IsProcessDone();
};

#endif // PCB_H
