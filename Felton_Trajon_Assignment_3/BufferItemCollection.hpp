/*
 * BufferItemCollection implements a 
 * thread-safe collection of BufferItems as a monitor.  
 */ 

#ifndef _BUFFERITEMCOLLECTION_HPP
#define _BUFFERITEMCOLLECTION_HPP

#include <pthread.h>
#include <list>
#include <unordered_map>
#include <iostream>

using namespace std;

#include "BufferItem.hpp"

class BufferItemCollection
{
private:
	//Implements the collection of BufferItems
	list<BufferItem> items;
	
	//Mutex used to make this collection thread-safe
	pthread_mutex_t mutex;

	//Lists some final statistics to the terminal
	void PrintThreadStats ();
	
public:

	BufferItemCollection ();

	~BufferItemCollection ();

	//Inserts a BufferItem into the collection
	void InsertItem (BufferItem item);

	//Returns the number of items the collection contains
	int NumberOfItems ();

	//Lists to terminal the contents of the collection along with some statistics
	void PrintItems ();
};
#endif