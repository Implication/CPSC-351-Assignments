#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <list>
#include <semaphore.h>

#include "BufferItem.hpp"
#include "BoundedBuffer.hpp"
#include "BufferItemCollection.hpp"

using namespace std;
using namespace std::chrono;

//Thread-safe bounded buffer to facilitate the item transfers 
BoundedBuffer boundedBuffer;

//Thread-safe collection that will contain all of the items produced and consumed
BufferItemCollection itemCollection;

//The number of items to be produced and consumed 
const int ITEM_COUNT = 100000;

bool go = true;

//Contains all of the consumer threads
list<pthread_t> consumer_threads;
	
//Contains all of the producer threads
list<pthread_t> producer_threads;
		
//Creates the items the producer will pass to the consumer via the bounded buffer.
list<BufferItem>* CreateItems (int numOfItems)
{
	list<BufferItem>* items = new list<BufferItem>; 
	BufferItem item;
	
	//Basically creates a global that makes it convienient to generate unique ids.
	static int count = 0;
	
	for (int i = 0; i < numOfItems; i++)
	{
		item._id = count++;
		items->push_back (item);
	}
		
	return items;
}

//The code executed by each producer thread
void* ProducerThread(void* ptr)
{
	cout << "Started ProducerThread: " << pthread_self() << endl;
	
	list<BufferItem> items = *((list<BufferItem>*)ptr);

	for (BufferItem item: items)
	{
		boundedBuffer.Produce(item);
	}

	delete (list<BufferItem>*)ptr;

	return NULL;
}

//The code executed by each consumer thread.
void* ConsumerThread(void* ptr)
{
	cout << "Started ConsumerThread: " << pthread_self() << endl;
	
	//TODO: Make this thread cancellable at any time by using pthread_setcancelstate to
	//set PTHREAD_CANCEL_ENABLE and by using pthread_setcanceltype to PTHREAD_CANCEL_ASYNCHRONOUS. (2 pts)
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
	while (go)
	{
		itemCollection.InsertItem(boundedBuffer.Consume());
	}

	
	return NULL;
}


//Creates the quanity of producer threads specified by the parameter
void StartProducerThreads (int numThreads)
{
	//TODO: Create the number of p_threads specified by numThreads using pthread_create. Each p_thread_t
	//will have a pointer to a list<BufferItem*> passed to it containing ITEM_COUNT/numThreads items.
	//Use the provided CreateItems function to generate the list. Each thread will begin their execution in
	//the ProducerThread function. Each p_thread_t should be added to producer_threads
	//so that the threads can be manipulated later. (10 pts)
	pthread_t thread[numThreads];
	for(int i = 0; i < numThreads; i++){
		list<BufferItem>* items = CreateItems(ITEM_COUNT/numThreads);
		if (pthread_create(&thread[i], NULL, ProducerThread, items)){
			fprintf(stderr, "Error on produce create");
		}
		producer_threads.push_back(thread[i]);
	}	
}

//Creates the quanity of consumer threads specified by the parameter
void StartConsumerThreads (int numThreads)
{
	//TODO: Create the number of p_threads specified by numThreads using pthread_create. 
	//Each thread will begin their execution in the ConsumerThread function.
	//Each p_thread_t should be added to consumer_threads
	//so that the threads can be manipulated later. (10 pts)
	pthread_t thread[numThreads];
	for(int i = 0; i < numThreads; i++){
		if(pthread_create(&thread[i], NULL, ConsumerThread, NULL)){
			fprintf(stderr, "Error on consume create");
		}
		consumer_threads.push_back(thread[i]);
	}
}

//Joins all threads in the passed in list
void JoinThreads (list<pthread_t> threads)
{
	//TODO: Iterate through the list of threads and join them
	//using pthread_join. (5pts)
	for(list<pthread_t>::iterator i = threads.begin(), end = threads.end(); i != end; ++i){
		if(pthread_join(*i,NULL)) {
			cout << "error" << endl;
		}
	}
}

//Cancels all threads in the passed in list
void CancelThreads (list<pthread_t> threads)
{
	//TODO: Iterate through the list of threads and cancel them
	//using pthread_cancel. (5pts)
	for(list<pthread_t>::iterator i = threads.begin(); i != threads.end(); ++i){
		pthread_cancel(*i);
	}
}
	

int main(int argc, char **argv)
{	
	//The number of producer threads to create
	int num_producers = 5;
	
	//The number of consumer threads to create
	int num_consumers = 4;
	
	//Creates the producer threads
	StartProducerThreads (num_producers);
	//Creates the consumer threads
	StartConsumerThreads (num_consumers);
	//Blocks thread until all producer threads have produced all their items
	JoinThreads(producer_threads);

	// Do not proceed until all items are consumers
	while (go)
	{
		if (itemCollection.NumberOfItems() == (ITEM_COUNT/num_producers)*num_producers)
		{
			go = false;
			
			//Forcibly cancels the consumer threads
			CancelThreads(consumer_threads);
		}
	}
	
	//Print the item statistics to the terminal
	itemCollection.PrintItems();
	
	//Prevent the terminal from closing. 
	cout << "Press enter to exit." << endl;
	cin.get();

	return 0;
}
