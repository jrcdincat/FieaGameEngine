#include "pch.h"
#include <stdexcept>
#include "CppUnitTest.h"
#include "Foo.h"
#include "HashMap.h"
#include "DefaultHash.h"
#include "DefaultHashSpecializations.h"
#include "ToStringSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<HashMap<Foo, int>::Iterator>(const HashMap<Foo, int>::Iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}

	template<>
	std::wstring ToString<HashMap<Foo, int>::ConstIterator>(const HashMap<Foo, int>::ConstIterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(HashMapTests)
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

		TEST_METHOD(Constructor)
		{
			HashMap<Foo, int> hashMap;
			Assert::AreEqual(size_t(0),hashMap.Size());

			HashMap<Foo, int> hashMap1{ 7 };
			Assert::AreEqual(size_t(0), hashMap1.Size());
		}

		TEST_METHOD(Find)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			{
				HashMap<Foo, int> hashMap{ 7 };
				Assert::AreEqual(hashMap.end(), hashMap.Find(c));
				const HashMap<Foo, int> constHashMap{ hashMap };
				Assert::AreEqual(constHashMap.cend(), constHashMap.Find(c));
			}

			HashMap<Foo, int> hashMap{ 7 };
			hashMap.Insert(std::make_pair(a, 2));
			hashMap.Insert(std::make_pair(b, 3));
			hashMap.Insert(std::make_pair(a, 3));
			hashMap.Insert(std::make_pair(c, 3));
			Assert::AreEqual(size_t(3), hashMap.Size());

			const HashMap<Foo, int> constHashMap{ hashMap };
			Assert::AreNotEqual(constHashMap.end(), constHashMap.Find(c));			
			Assert::AreEqual(3, (*constHashMap.Find(c)).second);
		}

		TEST_METHOD(Insert)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			HashMap<int, int> hashMap{ 7 };
			hashMap.Insert(std::make_pair(1, 2));
		}

		TEST_METHOD(Index)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Foo f{ 60 };
			Foo g{ 60 };

			HashMap<Foo, int> hashMap{ 11 };
			Assert::IsTrue(hashMap.Insert(std::make_pair(a, 6)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(b, 5)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(c, 4)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(d, 3)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(e, 2)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(f, 1)).second);
			Assert::AreEqual(size_t(6), hashMap.Size());
			Assert::AreEqual(5, hashMap.At(b));
			hashMap[g] = 4;
			Assert::AreEqual(4, hashMap.Find(g)->second);
			const HashMap<Foo, int> constHashMap{ hashMap };
			Assert::AreEqual(5, constHashMap.At(b));
		}

		TEST_METHOD(Remove)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Foo f{ 60 };

			HashMap<Foo, int> hashMap{ 11 };
			Assert::IsTrue(hashMap.Insert(std::make_pair(a, 6)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(b, 5)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(c, 4)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(d, 3)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(e, 2)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(f, 1)).second);
			Assert::AreEqual(size_t(6), hashMap.Size());
			hashMap.Remove(a);
			hashMap.Remove(b);
			Assert::AreEqual(size_t(4), hashMap.Size());
			Assert::AreEqual(hashMap.end(), hashMap.Find(a));
			Assert::AreEqual(hashMap.end(), hashMap.Find(b));
			Assert::AreEqual(c, hashMap.Find(c)->first);
			Assert::AreEqual(d, hashMap.Find(d)->first);
			Assert::AreEqual(e, hashMap.Find(e)->first);
			Assert::AreEqual(f, hashMap.Find(f)->first);
		}

		TEST_METHOD(Clear)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Foo f{ 60 };

			HashMap<Foo, int> hashMap{ 11 };
			Assert::IsTrue(hashMap.Insert(std::make_pair(a, 6)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(b, 5)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(c, 4)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(d, 3)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(e, 2)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(f, 1)).second);
			Assert::AreEqual(size_t(6), hashMap.Size());
			hashMap.Clear();
			Assert::AreEqual(size_t(0), hashMap.Size());
			Assert::IsFalse(hashMap.ContainsKey(a));
			Assert::IsFalse(hashMap.ContainsKey(b));
			Assert::IsFalse(hashMap.ContainsKey(c));
			Assert::IsFalse(hashMap.ContainsKey(d));
			Assert::IsFalse(hashMap.ContainsKey(e));
			Assert::IsFalse(hashMap.ContainsKey(f));
		}

		TEST_METHOD(Size)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Foo f{ 60 };

			HashMap<Foo, int> hashMap{ 11 };
			Assert::IsTrue(hashMap.Insert(std::make_pair(a, 6)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(b, 5)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(c, 4)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(d, 3)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(e, 2)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(f, 1)).second);
			Assert::AreEqual(size_t(6), hashMap.Size());

			const HashMap<Foo, int> constHashMap{ hashMap };
			Assert::AreEqual(size_t(6), constHashMap.Size());
		}

		TEST_METHOD(ContainsKey)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Foo f{ 60 };
			Foo g{ 70 };

			HashMap<Foo, int> hashMap{ 11 };
			Assert::IsTrue(hashMap.Insert(std::make_pair(a, 6)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(b, 5)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(c, 4)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(d, 3)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(e, 2)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(f, 1)).second);
			Assert::AreEqual(size_t(6), hashMap.Size());
			Assert::IsTrue(hashMap.ContainsKey(a));
			Assert::IsFalse(hashMap.ContainsKey(g));
		}

		TEST_METHOD(Iterator)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Foo f{ 60 };

			HashMap<Foo, int> hashMap{ 11 };
			Assert::IsTrue(hashMap.Insert(std::make_pair(a, 6)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(b, 5)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(c, 4)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(d, 3)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(e, 2)).second);
			Assert::IsTrue(hashMap.Insert(std::make_pair(f, 1)).second);
			Assert::AreEqual(size_t(6), hashMap.Size());

			HashMap<int,int>::Iterator iterator;

			{
				int count = 0;
				auto it1 = hashMap.begin();
				it1;
				for (auto it = hashMap.begin(); it != hashMap.end(); ++it)
				{
					++count;
				}
				Assert::AreEqual(6, count);
			}

			{
				int count = 0;
				for (auto it = hashMap.begin(); it != hashMap.end(); it++)
				{
					++count;
				}
				Assert::AreEqual(6, count);
			}

			{				
				int count = 0;
				for (const auto& value : hashMap)
				{
					value;
					count++;
				}

				Assert::AreEqual(6, count);
			}

			{
				HashMap<Foo, int> otherHashMap{ 7 };
				Assert::AreNotEqual(hashMap.begin(), otherHashMap.begin());
			}

			{
				const HashMap<Foo, int> constHashMap;
				Assert::AreEqual(size_t(0), constHashMap.Size());
				auto beginConstIt = constHashMap.begin();
				auto endConstIt = constHashMap.end();
				auto cbeginIt = constHashMap.cbegin();
				HashMap<Foo, int>::ConstIterator constIterator = beginConstIt;
				Assert::AreNotSame(beginConstIt, constIterator);
				Assert::AreEqual(beginConstIt, constIterator);
				HashMap<Foo, int>::ConstIterator constIterator2{ constIterator };

				HashMap<Foo, int> map;
				Assert::AreEqual(size_t(0), map.Size());
				HashMap<Foo, int>::Iterator beginIt = map.begin();
				HashMap<Foo, int>::Iterator it = beginIt;
				Assert::AreNotSame(beginIt, it);
				Assert::AreEqual(beginIt, it);
				HashMap<Foo, int>::Iterator it2{ it };
				
			}

			const HashMap<Foo, int> constHashMap{ hashMap };
			{
				int count = 0;
				for (auto it = constHashMap.cbegin(); it != constHashMap.cend(); ++it)
				{
					++count;
				}
				Assert::AreEqual(6, count);
			}

			{
				int count = 0;
				for (auto it = constHashMap.begin(); it != constHashMap.end(); it++)
				{
					++count;
				}
				Assert::AreEqual(6, count);
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}