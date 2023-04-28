#include "pch.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "Datum.h"
#include "Monster.h"
#include "TypeManager.h"
#include "CppUnitTest.h"
#include <memory>
#include "ReactionAttributed.h"
#include "Reaction.h"
#include "EventMessageAttributed.h"
#include "ActionEvent.h"
#include "Event.h"
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Datum::DatumTypes>(const Datum::DatumTypes& t)
	{
		RETURN_WIDE_STRING(Datum::DatumTypesStringMap.At(t).c_str());
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(ReactionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			RegisterType<GameObject, Attributed>();
			RegisterType<Action, Attributed>();
			RegisterType<ActionList, Action>();
			RegisterType<ActionIncrement, Action>();
			RegisterType<Monster, GameObject>();
			RegisterType<ActionEvent, Action>();
			RegisterType<Reaction, ActionList>();
			RegisterType<ReactionAttributed, Reaction>();
			RegisterType<EventMessageAttributed, Attributed>();

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Clear();

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

		TEST_METHOD(ActionEvents)
		{
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(make_unique<ActionEventFactory>());
			Factory<Scope>::Add(make_unique<ReactionAttributedFactory>());
			Assert::AreEqual(size_t(4), Factory<Scope>::Size());

			{
				GameObject gameObject;
				GameState::CreateInstance(gameObject);
				GameState& gameState = *GameState::Instance();
				Assert::IsNotNull(gameState.GetEventQueue());
				auto& eventQueue = *gameState.GetEventQueue();
				std::string aKey = "A"s;
				Datum& a = gameObject.AppendAuxiliaryAttribute(aKey) = 1;
				a;
				const string name = "Test Reaction"s;
				const string subType = "Test"s;

				ReactionAttributed* reactionScope = new ReactionAttributed;
				gameObject.Adopt(*reactionScope, "Reaction"s);
				auto reaction = reactionScope->As<ReactionAttributed>();
				reaction->SetSubType(subType);

				Assert::IsNotNull(reaction);
				Action& increment = reaction->CreateAction("ActionIncrement"s, "Increment Actions"s);
				ActionIncrement* incrementAction = increment.As<ActionIncrement>();
				Assert::IsNotNull(incrementAction);
				Assert::IsTrue(incrementAction->Target.empty());
				Assert::AreEqual(0, incrementAction->Step);
				
				Action& action = gameObject.CreateAction("ActionEvent"s, "Test Action"s);
				ActionEvent* actionEvent = action.As<ActionEvent>();
				Assert::IsNotNull(actionEvent);
				actionEvent->SetSubtype(subType);
				actionEvent->SetName(name);

				// These auxiliary attributes will be copied through EventMessageAttributed to ReactionAttributed
				actionEvent->AppendAuxiliaryAttribute("Target"s) = "A"s;
				actionEvent->AppendAuxiliaryAttribute("Step"s) = 2;
				incrementAction->Target = "A"s;
				incrementAction->Step = 2;
				Assert::AreEqual(0U, actionEvent->GetDelay());

				gameState.GetGameTime().SetCurrentTime(clock::time_point(0s));
				Assert::AreEqual(size_t(0), eventQueue.Size());
				gameState.Update();
				Assert::AreEqual(size_t(1), eventQueue.Size());

				Assert::AreEqual("A"s, incrementAction->Target);
				Assert::AreEqual(2, incrementAction->Step);
				Assert::AreEqual(3, a.GetInteger());
				gameState.GetEventQueue()->Clear();
			}
			Event<EventMessageAttributed>::UnSubscribeAll();
			GameState::DestroyInstance();
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
