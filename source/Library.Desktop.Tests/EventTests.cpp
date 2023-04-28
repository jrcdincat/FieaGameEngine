#include "pch.h"
#include "CppUnitTest.h"
#include "TypeManager.h"
#include "GameState.h"
#include "EventQueue.h"
#include "Event.h"
#include "FooEvent.h"
#include "FooEventSubscriber.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;
using DatumTypes = Datum::DatumTypes;
using QueueEntry = FieaGameEngine::EventQueue::QueueEntry;

namespace LibraryDesktopTests
{
	TEST_CLASS(EventTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(Debug) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(Enqueue)
		{
			//GameObject gameObject;
			//GameState::CreateInstance(gameObject);
			//GameState& gameState = *GameState::Instance();			
			//FooEventSubscriber subscriber;
			//FooEvent a;
			//std::shared_ptr<Event<FooEvent>> aEvent = std::make_shared<Event<FooEvent>>(a);

			//Event<FooEvent>::Subscribe(subscriber);
			//std::shared_ptr<QueueEntry> aQueueEntry = std::make_shared<QueueEntry>(aEvent, gameState.GetGameTime(), milliseconds(0), true);
			//gameState.GetEventQueue()->Enqueue(aQueueEntry);
			//Assert::AreEqual(size_t(0), aEvent->Message().integer);
			//gameState.Update();
			//Assert::AreEqual(size_t(0), gameState.GetEventQueue()->Size());
			//Assert::IsTrue(gameState.GetEventQueue()->IsEmpty());
			//Assert::AreEqual(size_t(1), aEvent->Message().integer);
			//Event<FooEvent>::UnSubscribeAll();
			//GameState::DestroyInstance();
		}

		TEST_METHOD(Semantics)
		{
			//GameObject gameObject;
			//GameState::CreateInstance(gameObject);
			//GameState& gameState = *GameState::Instance();
			//FooEventSubscriber subscriber;
			//FooEvent a;
			//std::shared_ptr<Event<FooEvent>> aEvent = std::make_shared<Event<FooEvent>>(a);

			//Event<FooEvent>::Subscribe(subscriber);
			//std::shared_ptr<QueueEntry> aQueueEntry = std::make_shared<QueueEntry>(aEvent, gameState.GetGameTime(), milliseconds(0), true);
			//gameState.GetEventQueue()->Enqueue(aQueueEntry);
			//Assert::AreEqual(size_t(0), aEvent->Message().integer);
			//gameState.Update();
			//Assert::AreEqual(size_t(0), gameState.GetEventQueue()->Size());
			//Assert::IsTrue(gameState.GetEventQueue()->IsEmpty());
			//Assert::AreEqual(size_t(1), aEvent->Message().integer);
			//std::shared_ptr<Event<FooEvent>> bEvent(aEvent);
			////Assert::AreEqual(aEvent->TypeIdClass(), bEvent->TypeIdClass());
			//Assert::AreEqual(size_t(1), bEvent->Message().integer);
			//std::shared_ptr<Event<FooEvent>> cEvent(std::move(bEvent));
			//Assert::AreEqual(size_t(1), cEvent->Message().integer);
			//std::shared_ptr<Event<FooEvent>> dEvent(aEvent);
			//dEvent = cEvent;
			//Assert::AreEqual(size_t(1), dEvent->Message().integer);
			//std::shared_ptr<Event<FooEvent>> eEvent(aEvent);
			//eEvent = std::move(dEvent);
			//Assert::AreEqual(size_t(1), eEvent->Message().integer);
			//Event<FooEvent>::UnSubscribeAll();
			////RTTI* b = std::shared_ptr<Event<FooEvent>>(aEvent).get();
			////Assert::IsFalse(aEvent->Equals(b));
			//GameState::DestroyInstance();
		}

		TEST_METHOD(Unsubscribe)
		{
			{
				Event<FooEvent>::UnSubscribeAll();
				FooEventSubscriber subscriber;
				FooEvent a;
				Event<FooEvent> aEvent(a);
				Event<FooEvent>::Subscribe(subscriber);
				Assert::AreEqual(size_t(1), aEvent.Size());
				Event<FooEvent>::UnSubscribe(subscriber);
				Assert::AreEqual(size_t(0), aEvent.Size());
			}

			{
				FooEventSubscriber subscriber;
				FooEvent a;
				Event<FooEvent> aEvent(a);
				Event<FooEvent>::Subscribe(subscriber);
				Assert::AreEqual(size_t(1), aEvent.Size());
				Event<FooEvent>::UnSubscribe(subscriber);
				Assert::AreEqual(size_t(0), aEvent.Size());
			}
		}

		TEST_METHOD(Clear)
		{
			//// Requires better testing as queue may already be empty while pending enqueue may not
			//GameObject gameObject;
			//GameState::CreateInstance(gameObject);
			//GameState& gameState = *GameState::Instance();			
			//FooEventSubscriber subscriber;
			//FooEvent a;
			//std::shared_ptr<Event<FooEvent>> aEvent = std::make_shared<Event<FooEvent>>(a);
			//Event<FooEvent>::Subscribe(subscriber);
			//std::shared_ptr<QueueEntry> aQueueEntry = std::make_shared<QueueEntry>(aEvent, gameState.GetGameTime());
			//gameState.GetEventQueue()->Enqueue(aQueueEntry);
			//gameState.Update();
			//gameState.GetEventQueue()->Clear();
			////Assert::AreEqual(size_t(0), gameState._eventQueue.Size());			
			////Assert::IsTrue(gameState._eventQueue.IsEmpty());
			//Assert::AreEqual(size_t(1), aEvent->Message().integer);
			//Event<FooEvent>::UnSubscribeAll();
			//GameState::DestroyInstance();
		}

		TEST_METHOD(RTTITEST)
		{
			FooEvent a;
			Event<FooEvent> aEvent(a);
			RTTI* b = new Event<FooEvent>(aEvent);
			Assert::IsFalse(aEvent.Equals(b));
			delete b;
			EventPublisher c(aEvent);
			auto cEvent = c.As<Event<FooEvent>>();
			Assert::IsTrue(cEvent->Is(Event<FooEvent>::TypeIdClass()));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}