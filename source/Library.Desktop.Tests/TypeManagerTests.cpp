#include "pch.h"
#include "CppUnitTest.h"
#include "TypeManager.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;
using namespace UnitTests;
using DatumTypes = Datum::DatumTypes;

namespace LibraryDesktopTests
{
	TEST_CLASS(TypeManagerTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			RegisterType<AttributedFoo, Attributed>();

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

		TEST_METHOD(TypeManager)
		{
			AttributedFoo attributedFoo;
			Assert::IsTrue(TypeManager::Instance()->ContainsTypeId(attributedFoo.TypeIdInstance()));
			Assert::ExpectException<std::invalid_argument>([] { auto signatures = TypeManager::Instance()->GetSignaturesOfTypeId(2); });
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
