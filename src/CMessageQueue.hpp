
#ifndef CMESSAGEQUEUE_INCLUDE
#define CMESSAGEQUEUE_INCLUDE

#include <vector>
#include "TMessage.hpp"

class CMessageQueue
{
private:
	std::vector<TMessage*> mQueue;

public:
	CMessageQueue();
	~CMessageQueue();

	void queueMessage(TMessage *);
	TMessage *extractFirst();
};

#endif