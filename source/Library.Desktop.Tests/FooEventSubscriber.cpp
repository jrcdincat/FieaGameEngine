#include "pch.h"
#include "FooEventSubscriber.h"
#include "RTTI.h"
#include "FooEvent.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	void FooEventSubscriber::Notify(EventPublisher& eventPublisher)
	{
		Event<FooEvent>* fEvent = eventPublisher.As<Event<FooEvent>>();
		assert(fEvent != nullptr);
		FooEvent& fooEvent = fEvent->Message();
		fooEvent.Update();
	}
}