#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "DefaultHash.h"
#include "HashMap.h"
#include <string>
#include "DefaultHashSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(DefaultHashTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Iniitialize)
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

		TEST_METHOD(IntegerHash)
		{
			const int a{ 10 };
			const int b{ 20 };
			const int c{ 10 };

			DefaultHash<int> hashFunc;
			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreNotEqual(hashFunc(b), hashFunc(c));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}

		TEST_METHOD(StringHash)
		{
			const std::string a = "Hello"s;
			const std::string b = "Goodbye"s;
			const std::string c(a);

			{
				DefaultHash<std::string> hashFunc;
				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(c));
				Assert::AreNotSame(a, c);
				Assert::AreEqual(hashFunc(a), hashFunc(c));
			}

			{
				DefaultHash <const std::string> hashFunc;
				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(c));
				Assert::AreNotSame(a, c);
				Assert::AreEqual(hashFunc(a), hashFunc(c));
			}
		}

		TEST_METHOD(CStringHash)
		{
			const char* a = "Hello";
			const char* b = "Goodbye";
			b;
			char c[6];
			strcpy_s(c, a);

			{
				DefaultHash<char*> hashFunc;
				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(c));
				Assert::AreEqual(hashFunc(a), hashFunc(c));
			}

			{
				DefaultHash<const char*> hashFunc;
				Assert::AreEqual(hashFunc(a), hashFunc(a));
				Assert::AreNotEqual(hashFunc(a), hashFunc(b));
				Assert::AreNotEqual(hashFunc(b), hashFunc(c));
				Assert::AreEqual(hashFunc(a), hashFunc(c));
			}
		}

		TEST_METHOD(FooHash)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 10 };

			DefaultHash<Foo> hashFunc;
			Assert::AreEqual(hashFunc(a), hashFunc(a));
			Assert::AreNotEqual(hashFunc(a), hashFunc(b));
			Assert::AreEqual(hashFunc(a), hashFunc(c));
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}