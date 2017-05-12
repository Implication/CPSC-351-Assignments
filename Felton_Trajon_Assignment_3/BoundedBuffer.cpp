#include "BoundedBuffer.hpp"

BoundedBuffer::BoundedBuffer()
	:in(0), out(0), counter(0)
{
	//TODO: Initialize the full, empty, and mutex
	//semaphores using sem_init. Initialize full to 0,
	//empty to BUFFER_SIZE, and mutex to 1. (5 pts)
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	sem_init(&mutex, 0, 1);
}
	
void BoundedBuffer::Produce (BufferItem& item)
{
	//TODO: Implement a bounded buffer producer that uses a counter
	//to maximize buffer utilization and implements a critical section
	//using the empty, mutex, and full semaphores declared in the .hpp.
	//Review the slides on Process Synchronization for guidance on the
	//producer implementation and the semaphore arrangement. 
	//Use sem_wait and sem_post to manipulate the semaphores. (10 pts) 
		sem_wait(&empty);
		sem_wait(&mutex);
			//This assignment should be called before the item is placed into the buffer
			item.sender_thread = pthread_self();
			buffer[in] = item;
			in = (in + 1) % BUFFER_SIZE;
		sem_post(&mutex);
		sem_post(&full);
}

BufferItem BoundedBuffer::Consume ()
{
	//TODO: Implement a bounded buffer consumer that uses a counter
	//to maximize buffer utilization and implements a critical section
	//using the empty, mutex, and full semaphores declared in the .hpp.
	//Review the slides on Process Synchronization for guidance on the
	//consumer implementation and the semaphore arrangement. 
	//Use sem_wait and sem_post to manipulate the semaphores. (10 pts) 
	
	BufferItem result;
		sem_wait(&full);
		sem_wait(&mutex);
			result = buffer[out];
			out = (out + 1) % BUFFER_SIZE;
			//This assignments should be made after an item is consumed. 
			result.receiver_thread = pthread_self();
		sem_post(&mutex);
		sem_post(&empty);
	return result;
}
	
BoundedBuffer::~BoundedBuffer()
{
	//TODO: Properly destroy the semaphores using sem_destroy. (5 pts)
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
}
