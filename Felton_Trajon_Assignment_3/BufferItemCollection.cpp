#include "BufferItemCollection.hpp"

BufferItemCollection::BufferItemCollection()
{
	//TODO: Initialize the mutex declared in the .hpp (2 pts)
	mutex = PTHREAD_MUTEX_INITIALIZER;
}

BufferItemCollection::~BufferItemCollection()
{
	//TODO: Destory the mutex declared in the .hpp (2 pts)
	pthread_mutex_destroy(&mutex);
}

void BufferItemCollection::InsertItem (BufferItem item)
{
	//TODO: Implement a monitor operation that inserts 
	//the parameter into the collection.
    //Utilize the mutex declared in the .hpp to
	//make it thread-safe (5 pts) 
	pthread_mutex_lock(&mutex);
		items.push_back(item);
	pthread_mutex_unlock(&mutex);
}

int BufferItemCollection::NumberOfItems ()
{
	//TODO: Implement a monitor operation that returns 
	//the number of items contained in the collection.
    //Utilize the mutex declared in the .hpp to
	//make it thread-safe (5 pts) 
	pthread_mutex_lock(&mutex);
		int size = items.size();
	pthread_mutex_unlock(&mutex);
	
	return size;
}

void BufferItemCollection::PrintThreadStats ()
{
	//Use hash maps to tally up the collection's items by threads
	//The ulong is a thread id that serves as a key to the int value, which is a tally.    
	unordered_map<ulong, int> consumer_freq_map;
	unordered_map<ulong, int> producer_freq_map;
 
    for (auto item: items) 
	{
        consumer_freq_map[item.receiver_thread]++;
    }
	
	for (auto item: items)
	{
		producer_freq_map[item.sender_thread]++;
	}
 
	//Outputs the statistics to the terminal
	cout << "Producer Thread Stats" << endl;
	for (auto it: producer_freq_map)
		std::cout << it.first << " - " 
                  << it.second <<  '\n';
 
	cout << "Consumer Thread Stats" << endl;
    for (auto it: consumer_freq_map) {
        std::cout << it.first << " - " 
                  << it.second <<  '\n';
	}
}

void BufferItemCollection::PrintItems ()
{	
	//TODO: Make this into a monitor operation by
	//using the mutex declared in the .hpp. (2 pts)
	pthread_mutex_lock(&mutex);
	cout << "Producer Thread, Consumer Thread, Item" << endl;
	
	for (BufferItem i : items)
	{
		cout << i.sender_thread << "," << i.receiver_thread << "," << i._id << endl;
	}

	PrintThreadStats ();
	cout << "Total responses: " << items.size() << endl;
	pthread_mutex_unlock(&mutex);
}
