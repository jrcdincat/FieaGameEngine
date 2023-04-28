#include "pch.h"
#include "CppUnitTest.h"
#include <memory>
#include "ConcreteFactory.h"
#include "AttributedFoo.h"
#include "JsonParseCoordinator.h"
#include "JsonTableParseHelper.h"
#include "glm/glm.hpp"
#include "json/json.h"
#include <fstream>
#include <filesystem>
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(FactoryTests)
	{
	public: 
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
			RegisterType<AttributedFoo, Attributed>();
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

		TEST_METHOD(AddRemove)
		{
			Assert::AreEqual(size_t(0), Factory<Scope>::Size());
			auto foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNull(foundFactory);
			Factory<Scope>::Add(make_unique<AttributedFooFactory>());
			Assert::AreEqual(size_t(1), Factory<Scope>::Size());
			foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNotNull(foundFactory);

			Factory<Scope>::Remove("AttributedFoo"s);
			Assert::AreEqual(size_t(0), Factory<Scope>::Size());
			foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNull(foundFactory);
		}

		TEST_METHOD(Find)
		{
			Assert::AreEqual(size_t(0), Factory<Scope>::Size());
			auto foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNull(foundFactory);
			Factory<Scope>::Add(make_unique<AttributedFooFactory>());
			Assert::AreEqual(size_t(1), Factory<Scope>::Size());
			foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNotNull(foundFactory);
		}

		TEST_METHOD(Create)
		{
			Assert::AreEqual(size_t(0), Factory<Scope>::Size());
			auto foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNull(foundFactory);
			Factory<Scope>::Add(make_unique<AttributedFooFactory>());
			Assert::AreEqual(size_t(1), Factory<Scope>::Size());
			foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			Assert::IsNotNull(foundFactory);
			Scope* attributedFooInstance = foundFactory->Create();
			Assert::IsNotNull(attributedFooInstance);
			delete attributedFooInstance;
			Scope* staticAttributedFoo = Factory<Scope>::Create("AttributedFoo"s);
			Assert::IsNotNull(staticAttributedFoo);
			delete staticAttributedFoo;
			Assert::IsNull(Factory<Scope>::Create("FailedCreate"));
		}

		TEST_METHOD(ClassKeyword)
		{
			//const std::string filePath("FactoryTest.json");

			//Assert::AreEqual(size_t(0), Factory<Scope>::Size());
			//auto foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			//Assert::IsNull(foundFactory);
			//Factory<Scope>::Add(make_unique<AttributedFooFactory>());
			//Assert::AreEqual(size_t(1), Factory<Scope>::Size());
			//foundFactory = Factory<Scope>::Find("AttributedFoo"s);
			//Assert::IsNotNull(foundFactory);

			//std::shared_ptr<Scope> root = std::make_shared<Scope>();
			//shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>(root);
			//JsonParseCoordinator parseCoordinator(wrapper);
			//auto testParseHelper = make_shared<JsonTableParseHelper>();
			//parseCoordinator.AddHelper(testParseHelper);

			//parseCoordinator.Initialize();


			//JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			//Assert::AreEqual(1U, wrapper->Depth());
			//Assert::AreEqual(1U, rawWrapper->MaxDepth());
			//parseCoordinator.DeserializeObjectFromFile(filePath);
			//Assert::AreEqual(1U, wrapper->Depth());
			//Assert::AreEqual(4U, rawWrapper->MaxDepth());

			//Scope* rawRoot = rawWrapper->_root.get();
			//Assert::AreEqual(size_t(1), rawRoot->Size());

		}

	private:
		inline static _CrtMemState _startMemState;
	};
}