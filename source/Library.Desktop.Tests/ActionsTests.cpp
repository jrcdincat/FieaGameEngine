#include "pch.h"
#include "GameObject.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionIncrement.h"
#include "Datum.h"
#include "Monster.h"
#include "TypeManager.h"
#include "CppUnitTest.h"
#include "JsonTableParseHelper.h"
#include <memory>

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
	TEST_CLASS(ActionTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			RegisterType<GameObject, Attributed>();
			RegisterType<Action, Attributed>();
			RegisterType<ActionList, Action>();
			RegisterType<ActionIncrement, Action>();
			RegisterType<Monster, GameObject>();


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

		TEST_METHOD(AddFactory)
		{
			auto foundActionListFactory = Factory<Scope>::Find("ActionList"s);
			Assert::IsNull(foundActionListFactory);
			auto foundActionIncrementFactory = Factory<Scope>::Find("ActionIncrement"s);
			Assert::IsNull(foundActionIncrementFactory);
			auto foundGameObjectFactory = Factory<Scope>::Find("GameObject"s);
			Assert::IsNull(foundGameObjectFactory);

			Factory<Scope>::Add(make_unique<ActionListFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Assert::AreEqual(size_t(3), Factory<Scope>::Size());

			foundActionListFactory = Factory<Scope>::Find("ActionList"s);
			Assert::IsNotNull(foundActionListFactory);
			foundActionIncrementFactory = Factory<Scope>::Find("ActionIncrement");
			Assert::IsNotNull(foundActionIncrementFactory);
			foundGameObjectFactory = Factory<Scope>::Find("GameObject");
			Assert::IsNotNull(foundGameObjectFactory);
		}

		TEST_METHOD(ActionListTest)
		{
			Factory<Scope>::Add(make_unique<ActionListFactory>());
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());

			GameTime gameTime;
			Scope* actionListScope = Factory<Scope>::Create("ActionList");
			ActionList* actionList = actionListScope->As<ActionList>();
			Action& action = actionList->CreateAction("ActionIncrement", "ActionIncrementA");
			ActionIncrement* actionIncrementA = static_cast<ActionIncrement*>(&action);
			actionIncrementA->Target = "Count";
			actionIncrementA->Step = 3;
			actionIncrementA->SetName("TheName");
			Assert::AreEqual("TheName"s, actionIncrementA->Name());

			Datum& count = actionList->Append("Count");
			count = 3;
			Assert::AreEqual(3, actionList->Find("Count")->GetInteger());
			actionList->Update(gameTime);
			Assert::AreEqual(6, actionList->Find("Count")->GetInteger());
			delete actionList;
		}

		TEST_METHOD(ActionIncrementTest)
		{
			const std::string filePath("ActionTest.json");
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<MonsterFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(make_unique<ActionListFactory>());

			{
				GameTime gameTime;
				GameObject* gameObject = static_cast<GameObject*>(Factory<Scope>::Create("GameObject"));
				Scope* action = Factory<Scope>::Create("ActionIncrement");
				ActionIncrement* actionIncrement = action->As<ActionIncrement>();
				actionIncrement->Target = "Count";
				actionIncrement->Step = -1;
				Datum& count = gameObject->Append("Count");
				count = 3;
				gameObject->Adopt(*actionIncrement, "ActionIncrement");
				actionIncrement->Update(gameTime);
				Assert::AreEqual(2, count.GetInteger());
				delete gameObject;
			}			

			//std::shared_ptr<Scope> root = std::make_shared<ActionList>();
			//Datum* nameDatum = root->Find("Name");
			//Assert::IsNotNull(nameDatum);
			//Assert::AreEqual(Datum::DatumTypes::String, nameDatum->Type());
			//Assert::IsTrue(nameDatum->IsExternal());

			//Datum* actionsDatum = root->Find("Actions");
			//Assert::IsNotNull(actionsDatum);
			//Assert::AreEqual(Datum::DatumTypes::Table, actionsDatum->Type());
			//Assert::IsFalse(actionsDatum->IsExternal());

			//shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>(root);
			//JsonParseCoordinator parseCoordinator(wrapper);
			//auto tableParseHelper = make_shared<JsonTableParseHelper>();
			//parseCoordinator.AddHelper(tableParseHelper);
			//parseCoordinator.Initialize();

			//JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			//Assert::AreEqual(1U, wrapper->Depth());
			//Assert::AreEqual(1U, rawWrapper->MaxDepth());
			//parseCoordinator.DeserializeObjectFromFile(filePath);

			//Assert::AreEqual(size_t(1), actionsDatum->Size());
			//auto& incrementCountScope = actionsDatum->GetScope();	
			//ActionIncrement* incrementCount = incrementCountScope.As<ActionIncrement>();
			//Assert::IsNotNull(incrementCount);
			//Assert::AreEqual("Increment Count"s, incrementCount->Name());
			//Assert::AreEqual("A"s, incrementCount->Target);
			//Assert::AreEqual(-5, incrementCount->Step);


			////Assert::AreEqual(1U, wrapper->Depth());
			////Assert::AreEqual(6U, wrapper->MaxDepth());

			//Scope* rawRoot = rawWrapper->_root.get();
			////Assert::AreEqual(size_t(2), rawRoot->Size());
			//auto a = rawRoot->ContainsKey("Object");
			//auto b = rawRoot->ContainsKey("A");
			//auto c = rawRoot->ContainsKey("Actions");
			//a;
			//b;
			//c;
			////auto& object = (*rawRoot)["Object"];
			////Assert::AreEqual(size_t(1), object.Size());
			////auto& count = object.GetScope()["Count"];
			////Assert::AreEqual(3, count.GetInteger());

			//auto& actions = (*rawRoot)["Actions"];
			//Assert::AreEqual(size_t(1), actions.Size());
			//auto& increment = actions.GetScope()["Increment"];
			//auto& name = increment.GetScope()["Name"];
			//Assert::AreEqual("Increment Count"s, name.GetString());
			//auto& target = increment.GetScope()["Target"];
			//Assert::AreEqual("A"s, target.GetString());

			//Assert::AreEqual("A"s, target.GetString());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
