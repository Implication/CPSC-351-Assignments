/*
 * BufferItem are the units of information that
 * are transferred through a BoundedBuffer
 */ 

#ifndef _BUFFERITEM_HPP
#define _BUFFERITEM_HPP

#include <stdlib.h>
#include <chrono>

using namespace std::chrono;

struct BufferItem
{
public:
	//An unique id number
	int _id;

	//The id of the thread that produced this item
	ulong sender_thread;
	
	//The id of the thread that consumed this item
	ulong receiver_thread;
 
	BufferItem ();
	
	BufferItem (int id);
};

#endif