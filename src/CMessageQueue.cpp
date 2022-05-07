
#include "CMessageQueue.hpp"

CMessageQueue::CMessageQueue()
{
}

CMessageQueue::~CMessageQueue()
{
	for(int i = 0; i < mQueue.size(); i++)
	{
		delete mQueue[i];
	}

	mQueue.clear();
}

void CMessageQueue::queueMessage(TMessage *aMsg)
{
	mQueue.push_back(aMsg);
}
	
TMessage* CMessageQueue::extractFirst()
{
	if(mQueue.empty())
		return 0;
	else
	{
		TMessage *first = mQueue[0];
		mQueue.erase(mQueue.begin());
		return first;
	}
}