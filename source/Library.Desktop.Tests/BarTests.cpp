#include "pch.h"
#include "CppUnitTest.h"
#include "ToStringSpecializations.h"
#include "Bar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(BarTests)
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
#if defined(DEBUG) || defined(_DEBUG)
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
			const Bar a;
			Assert::AreEqual(0, a.Data());

			const Bar b(10);
			Assert::AreEqual(10, b.Data());
		}

		TEST_METHOD(CopySemantics)
		{
			const Bar a(10);
			Assert::AreEqual(10, a.Data());

			const Bar b(a);
			Assert::AreEqual(a, b);

			Bar c;
			c = a;
			Assert::AreEqual(c, a);
		}

		TEST_METHOD(EqualityOperators)
		{
			const Bar a(10);
			const Bar b(a);
			Assert::AreEqual(a, b);

			const Bar c;
			Assert::IsTrue(c != a); // Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(SetData)
		{
			Bar a;
			Assert::AreEqual(0, a.Data());

			const int data = 10;
			a.SetData(data);
			Assert::AreEqual(data, a.Data());
		}

		TEST_METHOD(MoveSemantics)
		{
			const int data = 10;
			Bar a(data);
			Assert::AreEqual(data, a.Data());

			Bar b(std::move(a));
			Assert::AreEqual(data, b.Data());

			Bar c;
			Assert::AreEqual(0, c.Data());
			c = std::move(b);
			Assert::AreEqual(data, c.Data());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}