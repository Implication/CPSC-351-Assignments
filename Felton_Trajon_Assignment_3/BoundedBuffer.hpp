#ifndef _BOUNDEDBUFFER_H
#define _BOUNDEDBUFFER_H

#include <pthread.h>
#include <semaphore.h>
#include "BufferItem.hpp"

#define BUFFER_SIZE 10

struct BoundedBuffer 
{
private:
	//Index where the next item is produced
	int in;
	
	//Index of the next item to be consumed
	int out;
	
	//The number of items in the buffer
	int counter;
	
	//The buffer
	BufferItem buffer[BUFFER_SIZE];
	
	//Semaphores used to implement thread safety 
	sem_t full, empty, mutex;

public:
    BoundedBuffer();
	
	//Adds an item to the buffer. When buffer is full, this call blocks
	//until a slot frees up for the item.
	void Produce (BufferItem& item);

	//Removes and returns an item from the buffer. When buffer is empty
	//this call blocks until an item is placed into the buffer.
	BufferItem Consume();
	
	~BoundedBuffer();
};

#endif