#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"

namespace UnitTests
{
	class FooEventSubscriber : public FieaGameEngine::IEventSubscriber
	{
	public:
		void Notify(FieaGameEngine::EventPublisher& eventPublisher) override;
		~FooEventSubscriber() override {};
	};
}