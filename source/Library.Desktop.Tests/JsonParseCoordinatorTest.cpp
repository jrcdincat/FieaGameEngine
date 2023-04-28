#include "pch.h"
#include "JsonParseCoordinator.h"
#include "JsonTestParseHelper.h"
#include "JsonTableParseHelper.h"
#include "JsonIntegerParseHelper.h"
#include "glm/glm.hpp"
#include "CppUnitTest.h"
#include "Bar.h"
#include "json/json.h"
#include <fstream>
#include <memory>
#include <filesystem>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace glm;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<JsonParseCoordinator::Wrapper>(const JsonParseCoordinator::Wrapper& t)
	{
		std::wstring value;
		try
		{
			value = ToString(t.ToString());
		}
		catch (const std::exception&)
		{
			value = L"Invalid ToString value for JsonParseCoordinator::Wrapper"s;
		}
		return value;
	}

	//template<>
	//std::wstring ToString<vec4>(const vec4& t)
	//{
	//	std::wstring value;
	//	try
	//	{
	//		return ToString(glm::to_string(t));
	//	}
	//	catch (const std::exception&)
	//	{
	//		value = L"Invalid ToString value for vec4"s;
	//	}
	//	return value;
	//}

	//template<>
	//std::wstring ToString<mat4>(const mat4& t)
	//{
	//	std::wstring value;
	//	try
	//	{
	//		value = ToString(glm::to_string(t));
	//	}
	//	catch (const std::exception&)
	//	{
	//		value = L"Invalid ToString value for mat4"s;
	//	}
	//	return value;
	//}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(JsonParseCoordinatorTest)
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

		TEST_METHOD(Constructor)
		{
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().size());
			Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());

			parseCoordinator.AddHelper(make_shared<JsonTestParseHelper>());
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().size());

			shared_ptr<JsonParseCoordinator::Wrapper> anotherWrapper(new JsonTestParseHelper::Wrapper);
			parseCoordinator.SetWrapper(anotherWrapper);
			Assert::AreSame(*anotherWrapper, parseCoordinator.GetWrapper());
		}

		TEST_METHOD(Clone)
		{
			std::shared_ptr<Scope> root = std::make_shared<Scope>();
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTableParseHelper::Wrapper(root));
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().size());
			Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());

			parseCoordinator.AddHelper(make_shared<JsonTableParseHelper>());
			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().size());

			auto cloneParseCoordinator = parseCoordinator.Clone();
			JsonParseCoordinator* rawCloneCoordinator = cloneParseCoordinator.get();
			Assert::AreEqual(parseCoordinator.Helpers().size(), rawCloneCoordinator->Helpers().size());
			Assert::IsTrue(&parseCoordinator.GetWrapper() != &cloneParseCoordinator->GetWrapper());

			auto cloneHelpers = cloneParseCoordinator->Helpers();
			auto helpers = parseCoordinator.Helpers();


			for (size_t index = 0; index < helpers.size(); ++index)
			{
				Assert::IsTrue(helpers[index] != cloneHelpers[index]);
			}
		}

		TEST_METHOD(RemoveHelpers)
		{
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper(new JsonTestParseHelper::Wrapper);
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().size());
			Assert::AreSame(*wrapper, parseCoordinator.GetWrapper());
			auto helper = make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(helper);
			Assert::ExpectException<runtime_error>([&parseCoordinator, &helper] { parseCoordinator.AddHelper(helper); });

			Assert::AreEqual(size_t(1), parseCoordinator.Helpers().size());
			parseCoordinator.RemoveHelper(helper->TypeIdInstance());
			Assert::AreEqual(size_t(0), parseCoordinator.Helpers().size());
		}

		TEST_METHOD(IntegerParsing)
		{
			string inputString = R"( {"integer" : 100} )";
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			Assert::IsTrue(&*wrapper == &parseCoordinator.GetWrapper());
			parseCoordinator.AddHelper(make_shared<JsonIntegerParseHelper>());

			parseCoordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(size_t(1), rawWrapper->Data.Size());
			Assert::AreEqual(100, rawWrapper->Data.GetInteger());
		}

		TEST_METHOD(IntegerArrayParsing)
		{
			string inputString = R"({ "integer": [ 10, 20, 30, 40 ]})";

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonIntegerParseHelper::Wrapper>();
			JsonParseCoordinator parseCordinator(wrapper);
			parseCordinator.AddHelper(make_shared<JsonIntegerParseHelper>());

			parseCordinator.DeserializeObject(inputString);
			JsonIntegerParseHelper::Wrapper* rawWrapper = static_cast<JsonIntegerParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(size_t(4), rawWrapper->Data.Size());
			Assert::AreEqual(10, rawWrapper->Data.GetInteger(0));
			Assert::AreEqual(20, rawWrapper->Data.GetInteger(1));
			Assert::AreEqual(30, rawWrapper->Data.GetInteger(2));
			Assert::AreEqual(40, rawWrapper->Data.GetInteger(3));
		}

		TEST_METHOD(ObjectParsing)
		{
			const string inputString = R"({"Object":{"integer": 1}})";

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParseHelper = make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParseHelper);
			parseCoordinator.Initialize();

			Assert::AreEqual(0U, testParseHelper->StartHandlerCount());
			Assert::AreEqual(0U, testParseHelper->EndHandlerCount());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(1U, rawWrapper->MaxDepth());

			parseCoordinator.DeserializeObject(inputString);

			Assert::AreEqual(2U, testParseHelper->StartHandlerCount());
			Assert::AreEqual(2U, testParseHelper->EndHandlerCount());
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(2U, rawWrapper->MaxDepth());
		}

		TEST_METHOD(DeserializeFileObjectParsing)
		{
			const std::string filename = "TestFile.json";
			const string inputString = R"({"Object":{"integer": 1}})";
			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());
			outputFile << inputString;
			outputFile.close();

			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParseHelper = make_shared<JsonTestParseHelper>();
			parseCoordinator.AddHelper(testParseHelper);
			parseCoordinator.Initialize();

			Assert::AreEqual(0U, testParseHelper->StartHandlerCount());
			Assert::AreEqual(0U, testParseHelper->EndHandlerCount());
			JsonTestParseHelper::Wrapper* rawWrapper = static_cast<JsonTestParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(1U, rawWrapper->MaxDepth());

			parseCoordinator.DeserializeObjectFromFile(filename);

			Assert::AreEqual(2U, testParseHelper->StartHandlerCount());
			Assert::AreEqual(2U, testParseHelper->EndHandlerCount());
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(2U, rawWrapper->MaxDepth());
		}

		TEST_METHOD(TableParsing)
		{
			const std::string filename = "TestFile.json";
			const string inputString = R"({"Object":{"type": "table", "value": {"BoxNumber":{"type": "integer", "value": 3}}}})";

			std::ofstream outputFile(filename);
			Assert::IsTrue(outputFile.good());
			outputFile << inputString;
			outputFile.close();

			{
				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTestParseHelper::Wrapper>();
				JsonParseCoordinator parseCoordinator(wrapper);
				auto testParseHelper = make_shared<JsonTableParseHelper>();
				parseCoordinator.AddHelper(testParseHelper);
				parseCoordinator.Initialize();

				JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(1U, wrapper->Depth());
				Assert::AreEqual(1U, rawWrapper->MaxDepth());

				parseCoordinator.DeserializeObjectFromFile(filename);
				Assert::AreEqual(1U, wrapper->Depth());
				Assert::AreEqual(2U, rawWrapper->MaxDepth());
			}

			std::shared_ptr<Scope> root = std::make_shared<Scope>();
			shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>(root);
			JsonParseCoordinator parseCoordinator(wrapper);
			auto testParseHelper = make_shared<JsonTableParseHelper>();
			parseCoordinator.AddHelper(testParseHelper);
			Assert::AreEqual(testParseHelper->TypeIdInstance(), parseCoordinator.Helpers().At(0)->TypeIdInstance());
			parseCoordinator.Initialize();

			JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(1U, rawWrapper->MaxDepth());

			parseCoordinator.DeserializeObjectFromFile(filename);
			Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(4U, rawWrapper->MaxDepth());
		}

		TEST_METHOD(TableArrayParsing)
		{
			{
				const std::string filename = "TestFile.json";
				const string inputString = R"({"Object":{"type": "table", "value": {"BoxNumber":{"type": "integer", "value": [3, 2, 1]}}}})";
				std::ofstream outputFile(filename);
				Assert::IsTrue(outputFile.good());
				outputFile << inputString;
				outputFile.close();

				std::shared_ptr<Scope> root = std::make_shared<Scope>();
				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>(root);
				JsonParseCoordinator parseCoordinator(wrapper);
				auto testParseHelper = make_shared<JsonTableParseHelper>();
				parseCoordinator.AddHelper(testParseHelper);
				parseCoordinator.Initialize();

				JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(1U, wrapper->Depth());
				Assert::AreEqual(1U, rawWrapper->MaxDepth());

				parseCoordinator.DeserializeObjectFromFile(filename);
				Assert::AreEqual(1U, wrapper->Depth());
				Assert::AreEqual(4U, rawWrapper->MaxDepth());
			}

			{
				const std::string filename = "TestFile.json";
				const string inputString = R"({"Object":{"type": "table", "value": {"BoxNumber":{"type": "integer", "value": []}}}})";
				std::ofstream outputFile(filename);
				Assert::IsTrue(outputFile.good());
				outputFile << inputString;
				outputFile.close();

				std::shared_ptr<Scope> root = std::make_shared<Scope>();
				shared_ptr<JsonParseCoordinator::Wrapper> wrapper = make_shared<JsonTableParseHelper::Wrapper>(root);
				JsonParseCoordinator parseCoordinator(wrapper);
				auto testParseHelper = make_shared<JsonTableParseHelper>();
				parseCoordinator.AddHelper(testParseHelper);
				parseCoordinator.Initialize();

				JsonTableParseHelper::Wrapper* rawWrapper = static_cast<JsonTableParseHelper::Wrapper*>(wrapper.get());
				Assert::AreEqual(1U, wrapper->Depth());
				Assert::AreEqual(1U, rawWrapper->MaxDepth());

				parseCoordinator.DeserializeObjectFromFile(filename);
				Assert::AreEqual(1U, wrapper->Depth());
				Assert::AreEqual(4U, rawWrapper->MaxDepth());
			}
		}

		TEST_METHOD(TableVectorParsing)
		{
			const std::string filePath("FileTest1.json");
			const string inputString = R"({"Object":{"type": "table", "value": {"BoxNumber":{"type": "integer", "value": [3, 2, 1]}}}, "Crate":{"type": "vector", "value": "vec4(7,6,5,4) "}})";
			std::ofstream outputFile(filePath);
			Assert::IsTrue(outputFile.good());
			outputFile << inputString;
			outputFile.close();

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
			Assert::AreEqual(4U, rawWrapper->MaxDepth());
		}

		TEST_METHOD(TableMatrixParsing)
		{
			const std::string filePath("FileTest2.json");
			const string inputString = R"({"Object":{"type": "table", "value": {"BoxNumber":{"type": "integer", "value": [3, 2, 1]}}}, "Crate":{"type": "matrix", "value": "mat4(vec4( 2.0, 2.0, 2.0, 2.0), vec4( 3.0, 3.0, 3.0, 3.0), vec4( 4.0, 4.0, 4.0, 4.0), vec4(5.0, 5.0, 5.0, 5.0)) "}})";
			std::ofstream outputFile(filePath);
			Assert::IsTrue(outputFile.good());
			outputFile << inputString;
			outputFile.close();

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
			//Assert::AreEqual(1U, wrapper->Depth());
			//Assert::AreEqual(4U, rawWrapper->MaxDepth());
			//
			//Scope* rootResult = rawWrapper->_root.get();
			//Assert::AreEqual(size_t(2), rootResult->Size());
			//auto& crateData = rootResult[0][0];
			//Assert::AreEqual(size_t(1), crateData.Size());
			//Assert::AreEqual(mat4(vec4(2.0f), vec4(3.0f), vec4(4.0f), vec4(5.0f)), crateData.GetMatrix());
		}

		TEST_METHOD(FilePath)
		{
			const std::string filePath("FileTest.json");

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
	/*		Assert::AreEqual(1U, wrapper->Depth());
			Assert::AreEqual(4U, rawWrapper->MaxDepth());

			Scope* rootResult = rawWrapper->_root.get();
			Assert::AreEqual(size_t(4), rootResult->Size());*/
			
			//auto& objectData = rootResult[0][0];
			//auto& planeData = rootResult[0][1];
			//Assert::AreEqual(size_t(1), objectData.Size());
			//Assert::AreEqual(size_t(3), planeData.Size());
			//
			//Assert::IsTrue(&objectData == &rootResult->At("Object"));
			//auto& boxNumberScope = objectData.GetScope(0);
			//auto& boxNumber = boxNumberScope.At("BoxNumber");
			//Assert::AreEqual(size_t(3), boxNumber.Size());
			//Assert::AreEqual(2, boxNumber.GetInteger());
			//Assert::AreEqual(3, boxNumber.GetInteger(1));
			//Assert::AreEqual(4, boxNumber.GetInteger(2));

			//auto& heightScope = objectData.GetScope();
			//auto& height = heightScope.At("Height");
			//Assert::AreEqual(size_t(3), height.Size());
			//Assert::AreEqual(4.3f, height.GetFloat());
			//Assert::AreEqual(5.3f, height.GetFloat(1));
			//Assert::AreEqual(7.2f, height.GetFloat(2));

			//std::string correctName = "++C";
			//auto& nameScope = objectData.GetScope(0);
			//auto& name = nameScope.At("Name");
			//Assert::AreEqual(size_t(1), name.Size());
			//Assert::AreEqual(correctName, name.GetString());

			//Assert::IsTrue(&planeData == &rootResult->At("Plane"));
			//Assert::AreEqual(vec4(7,6,5,4), planeData.GetVector(0));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}