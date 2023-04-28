#include "pch.h"
#include "EventPublisher.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher);
	EventPublisher::EventPublisher(Vector<IEventSubscriber*>& subscribers) : _subscribers{ &subscribers }
	{
	}

	bool EventPublisher::Equals(const RTTI*) const
	{
		return false;
	}

	void EventPublisher::Deliver()
	{
		for (auto s : *_subscribers)
		{
			s->Notify(*this);
		}
	}
}
