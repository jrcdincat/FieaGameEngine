#include "pch.h"
#include "GameObject.h"
#include "Datum.h"
#include "TypeManager.h"
#include "CppUnitTest.h"
#include "JsonTableParseHelper.h"
#include <memory>
#include "Monster.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace glm;
using namespace std;
using namespace std::string_literals;
using DatumTypes = Datum::DatumTypes;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Datum>(const Datum& t)
	{
		RETURN_WIDE_STRING(ToString(Datum::DatumTypesStringMap.At(t.Type())) + ToString(t.Size()) + ToString(t.Capacity()));
	}

	template<>
	std::wstring ToString<mat4>(const mat4& t)
	{
		RETURN_WIDE_STRING(ToString(glm::to_string(t)));
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(GameObjectTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			RegisterType<GameObject, Attributed>();
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

		TEST_METHOD(Constructor)
		{
			std::string a = "Name"s;
			std::string b = "Transform"s;
			std::string c = "Children"s;

			GameObject gameObject;
			Assert::AreEqual(size_t(5), gameObject.Size());
			Assert::AreEqual(gameObject[0], *gameObject.Find("this"s));
			Assert::IsTrue(gameObject.ContainsKey(a));
			Assert::IsTrue(gameObject.ContainsKey(b));
			Assert::IsTrue(gameObject.ContainsKey(c));
		}

		TEST_METHOD(Children)
		{
			auto foundFactory = Factory<Scope>::Find("Monster"s);
			Assert::IsNull(foundFactory);
			Factory<Scope>::Add(make_unique<MonsterFactory>());
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Assert::AreEqual(size_t(2), Factory<Scope>::Size());
			foundFactory = Factory<Scope>::Find("Monster"s);
			Assert::IsNotNull(foundFactory);

			GameTime gameTime;
			GameObject* gameObject = static_cast<GameObject*>(Factory<Scope>::Create("GameObject"));
			mat4 monsterVector = mat4(vec4(20), vec4(10), vec4(5), vec4(1));
			Scope* child1 = Factory<Scope>::Create("Monster");
			Monster* child1Monster = child1->As<Monster>();
			child1Monster->Name = "Child1";
			child1Monster->Transform = mat4(1);
			gameObject->Adopt(*child1Monster, "Children");
			gameObject->Name = "SuperMonster";
			gameObject->Transform = monsterVector;

			Scope* child2 = Factory<Scope>::Create("Monster");
			Monster* child2Monster = child2->As<Monster>();
			child2Monster->Name = "Child2";
			child2Monster->Transform = mat4(1);
			gameObject->Adopt(*child2Monster, "Children");

			Scope* child3 = Factory<Scope>::Create("Monster");
			Monster* child3Monster = child3->As<Monster>();
			child3Monster->Name = "Child3";
			child3Monster->Transform = mat4(1);
			gameObject->Adopt(*child3Monster, "Children");

			Assert::AreEqual("SuperMonster"s, gameObject->Name);
			Assert::AreEqual(monsterVector, gameObject->Transform);
			Assert::AreEqual("Child1"s, gameObject->At("Children").GetScope().As<Monster>()->Name);
			Assert::AreEqual("Child2"s, gameObject->At("Children").GetScope(1).As<Monster>()->Name);
			Assert::AreEqual("Child3"s, gameObject->At("Children").GetScope(2).As<Monster>()->Name);

			//gameObject->Update(gameTime);
			//auto& value = gameObject->_updateCount;
			//value;
			//Assert::AreEqual(int32_t(4), value);

			delete gameObject;
		}

		TEST_METHOD(JsonParsing)
		{
			const std::string filePath("GameObjectTest.json");

			Assert::AreEqual(size_t(0), Factory<Scope>::Size());
			auto foundFactory = Factory<Scope>::Find("Monster"s);
			Assert::IsNull(foundFactory);
			Factory<Scope>::Add(make_unique<MonsterFactory>());
			Assert::AreEqual(size_t(1), Factory<Scope>::Size());
			foundFactory = Factory<Scope>::Find("Monster"s);
			Assert::IsNotNull(foundFactory);

			std::shared_ptr<Scope> root = std::make_shared<Scope>();
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>(root);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParseHelper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(testParseHelper);

			parseCoordinator.Initialize();


			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(1U, rawWrapper->MaxDepth());
			parseCoordinator.DeserializeObjectFromFile(filePath);
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(8U, rawWrapper->MaxDepth());

			//Scope* rawRoot = rawWrapper->_root.get();
			//Assert::AreEqual(size_t(1), rawRoot->Size());
			//auto& object = (*rawRoot)["Object"];
			//Assert::AreEqual(size_t(1), object.Size());
			//auto& name = object.GetScope()["Name"];
			//auto& children = object.GetScope()["Children"];
			//Assert::AreEqual(size_t(1), name.Size());
			//Assert::AreEqual("TheName"s, name.GetString());
			//Assert::AreEqual(size_t(1), children.GetScope().Size());
			//Assert::AreEqual("InnerChild"s, children.GetScope()["ChildOne"].GetScope()["Name"].GetString());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}