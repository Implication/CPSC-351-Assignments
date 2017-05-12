#include "BufferItem.hpp"


BufferItem::BufferItem ()
{
	_id = -1000;
	sender_thread = 0;
	receiver_thread = 0;
}
	
BufferItem::BufferItem (int id)
	:_id(id)
{
	sender_thread = 0;
	receiver_thread = 0;
}
