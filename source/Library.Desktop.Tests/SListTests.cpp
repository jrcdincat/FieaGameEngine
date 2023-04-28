#include "pch.h"
#include <stdexcept>
#include "CppUnitTest.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "SList.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<SList<Foo>::Iterator>(const SList<Foo>::Iterator& t)
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
	std::wstring ToString<SList<Foo>::ConstIterator>(const SList<Foo>::ConstIterator& t)
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
	TEST_CLASS(SListTests)
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
			SList<Foo> list;
			const SList<Foo>& constList(list);
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list]() { auto& front = list.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&constList]() { auto& front = constList.Front(); UNREFERENCED_LOCAL(front); });
			Assert::ExpectException<std::runtime_error>([&list]() { auto& back = list.Back(); UNREFERENCED_LOCAL(back); });
			Assert::ExpectException<std::runtime_error>([&constList]() { auto& back = constList.Back(); UNREFERENCED_LOCAL(back); });
		}

		TEST_METHOD(CopySemantics)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());

			{
				SList<Foo> copyOfList(list);

				Assert::AreEqual(size_t(3), copyOfList.Size());
				Assert::AreEqual(a, copyOfList.Front());
				Assert::AreEqual(c, copyOfList.Back());
				Assert::AreNotSame(a, copyOfList.Front());
				Assert::AreNotSame(a, copyOfList.Back());
			}

			{
				SList<Foo>copyAssignmentList;
				copyAssignmentList = list;
				Assert::AreEqual(size_t(3), copyAssignmentList.Size());
				Assert::AreEqual(a, copyAssignmentList.Front());
				Assert::AreEqual(c, copyAssignmentList.Back());
				Assert::AreNotSame(a, copyAssignmentList.Front());
				Assert::AreNotSame(a, copyAssignmentList.Back());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 40 };

			SList<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());

			{
				SList<Foo> movedList(std::move(list));

				Assert::AreEqual(size_t(3), movedList.Size());
				Assert::AreEqual(a, movedList.Front());
				Assert::AreEqual(c, movedList.Back());
				Assert::AreNotSame(a, movedList.Front());
				Assert::AreNotSame(a, movedList.Back());

				movedList.PushBack(std::move(d));
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), list.Size());
#pragma warning(pop)
				Assert::AreEqual(e, movedList.Back());

			}
		}

		TEST_METHOD(Front)
		{
			const Foo a{ 3 };
			const Foo b{ 2 };
			const Foo c{ 1 };

			SList<Foo> list{ a };

			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			list.PushBack(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Front());
			list.PushFront(c);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(c, list.Front());

			const SList<Foo>& constList(list);
			Assert::AreEqual(list.Front(), constList.Front());
		}

		TEST_METHOD(PushFront)
		{
			const Foo a{ 2 };
			const Foo b{ 1 };
			SList<Foo> list;

			list.PushFront(a);
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PushFront(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(a, list.Back());
		}
		
		TEST_METHOD(PopFront)
		{
			const Foo a{ 10 };
			const Foo b{ 11 };
			const Foo c{ 12 };
			SList<Foo> list{ a, b, c };

			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());

			list.PopFront();
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(c, list.Back());

			list.PopFront();
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(c, list.Back());

			list.PopFront();
			Assert::AreEqual(size_t(0), list.Size());
			Assert::ExpectException<std::runtime_error>([&list]() { Foo data = list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list]() { Foo data = list.Back(); });
		}
		
		TEST_METHOD(Back)
		{
			const Foo a{ 2 };
			const Foo b{ 1 };
			const Foo c{ 0 };
			SList<Foo> list{ a };

			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Back());
			list.PushFront(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Back());
			list.PushBack(c);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(c, list.Back());


			const SList<Foo>& constList(list);
			Assert::AreEqual(list.Back(), constList.Back());
		}
		
		TEST_METHOD(PushBack)
		{
			const Foo a{ 2 };
			const Foo b{ 1 };
			SList<Foo> list;
			
			list.PushBack(a);
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(a, list.Back());
			list.PushBack(b);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(b, list.Back());
		}

		TEST_METHOD(PopBack)
		{
			{
				const Foo a{ 10 };
				SList<Foo> list{ a };
				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(a, list.Front());
				list.PopBack();
				Assert::AreEqual(size_t(0), list.Size());
			}

			{
				const Foo a{ 11 };
				const Foo b{ 12 };
				const Foo c{ 13 };
				const Foo d{ 14 };
				SList<Foo> list{ a,b,c,d };

				Assert::AreEqual(size_t(4), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(d, list.Back());
				list.PopBack();

				Assert::AreEqual(size_t(3), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(c, list.Back());
				list.PopBack();

				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(b, list.Back());
				list.PopBack();

				Assert::AreEqual(size_t(1), list.Size());
				Assert::AreEqual(a, list.Front());
				Assert::AreEqual(a, list.Back());
				list.PopBack();

				Assert::IsTrue(list.IsEmpty());
			}
		}
		
		TEST_METHOD(Size)
		{
			const Foo a{ 2 };
			const Foo b{ 1 };
			SList<Foo> list;

			Assert::AreEqual(size_t(0), list.Size());
			list.PushBack(a);
			Assert::AreEqual(size_t(1), list.Size());
			list.PushFront(b);
			Assert::AreEqual(size_t(2), list.Size());
			list.PopBack();
			Assert::AreEqual(size_t(1), list.Size());
			list.PopFront();
			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(IsEmpty)
		{
			const Foo a{ 11 };
			SList<Foo> list{ a };
			Assert::IsFalse(list.IsEmpty());
			list.PopFront();
			Assert::AreEqual(size_t(0), list.Size());
			Assert::AreEqual(true, list.IsEmpty());
		}

		TEST_METHOD(Clear)
		{
			const Foo a{ 2 };
			const Foo b{ 1 };
			SList<Foo> list{ a,b };
			Assert::AreEqual(size_t(2), list.Size());
			list.Clear();
			Assert::AreEqual(size_t(0), list.Size());
		}

		TEST_METHOD(Iterator)
		{
			const Foo a{ 5 };
			const Foo b{ 6 };
			const Foo c{ 7 };

			{
				SList<Foo> list;
				Assert::AreEqual(list.end(), list.begin());
				list.PushBack(a);
				Assert::AreNotEqual(list.end(),list.begin());
			}

			{
				SList<Foo> list{ a,b,c };
				{
					size_t count = 0;
					for (auto it = list.begin(); it != list.end(); ++it, ++count)
					{
						;
					}
					Assert::AreEqual(list.Size(), count);
				}

				{
					size_t count = 0;
					for (auto it = list.begin(); it != list.end(); it++, ++count)
					{
						;
					}
					Assert::AreEqual(list.Size(), count);
				}

				{
					size_t count = 0;
					for (const Foo& value : list)
					{
						value;
						++count;
					}
					Assert::AreEqual(list.Size(), count);
				}		
			}

			{
				SList<Foo> list;
				SList<Foo>::Iterator iterator;
				Assert::ExpectException<std::runtime_error>([&list]() { auto it = list.begin(); it++; });
				Assert::ExpectException<std::runtime_error>([&iterator]() { iterator++; });

				{
					const SList<Foo> constList;
					const SList<Foo>::Iterator cIterator;
					Assert::AreEqual(constList.end(), constList.begin());
					Assert::ExpectException<std::runtime_error>([&cIterator]() {auto it = cIterator; it++; });
					const SList<Foo>::ConstIterator  constIterator{ cIterator };
					Assert::ExpectException<std::runtime_error>([&constIterator]() { *constIterator; });
					Assert::ExpectException<std::runtime_error>([&constList]() { auto it = constList.cbegin(); it++; });
					Assert::ExpectException<std::runtime_error>([&constIterator]() {auto it = constIterator; it++; });
					Assert::AreEqual(constList.cend(), constList.cbegin());
				}

				{
					const SList<Foo> constList{ a,b,c };
					Assert::AreEqual(a, *constList.cbegin());
					{
						size_t count = 0;
						for (auto it = constList.cbegin(); it != constList.cend(); ++it, ++count)
						{
							;
						}
						Assert::AreEqual(constList.Size(), count);
					}

					{
						size_t count = 0;
						for (auto it = constList.cbegin(); it != constList.cend(); it++, ++count)
						{
							;
						}
						Assert::AreEqual(constList.Size(), count);
					}

					{
						size_t count = 0;
						for (const Foo& value : constList)
						{
							value;
							++count;
						}
						Assert::AreEqual(constList.Size(), count);
					}
				}
			}
		}

		TEST_METHOD(Find)
		{
			{
				const Foo a{ 1 };
				const Foo b{ 2 };
				const Foo c{ 3 };
				const Foo d{ 4 };
				const Foo e{ 5 };

				{
					SList<Foo> list{ a,b,c,d,e };
					Assert::AreEqual(c, *list.Find(c));
					Assert::AreEqual(e, *list.Find(e));
				}

				{
					SList<Foo> list;
					SList<Foo>::Iterator it = list.Find(a);
					Assert::ExpectException<std::runtime_error>([&list, a]() { auto it = list.Find(a); *it; });
					Assert::AreEqual(list.end(), it);
				}

				{
					SList<Foo> list{ a };
					SList<Foo>::Iterator it = list.Find(a);
					Assert::AreEqual(size_t(1), list.Size());
					Assert::AreEqual(a, *it);


					SList<Foo>::Iterator it2 = list.Find(b);
					Assert::ExpectException<std::runtime_error>([&list, b]() { auto it = list.Find(b); *it; });
					Assert::AreEqual(list.end(), it2);
				}
			}

			{
				Foo a{ 1 };
				Foo b{ 2 };
				Foo c{ 3 };
				Foo d{ 4 };
				Foo e{ 5 };

				{
					SList<Foo> list{ a,b,c,d,e };
					Assert::AreEqual(c, *list.Find(c));
					Assert::AreEqual(e, *list.Find(e));
				}

				{
					SList<Foo> list;
					SList<Foo>::Iterator it = list.Find(a);
					Assert::ExpectException<std::runtime_error>([&list, a]() { auto it = list.Find(a); *it; });
					Assert::AreEqual(list.end(), it);
				}

				{
					SList<Foo> list{ a };
					SList<Foo>::Iterator it = list.Find(a);
					Assert::AreEqual(size_t(1), list.Size());
					Assert::AreEqual(a, *it);


					SList<Foo>::Iterator it2 = list.Find(b);
					Assert::ExpectException<std::runtime_error>([&list, b]() { auto it = list.Find(b); *it; });
					Assert::AreEqual(list.end(), it2);
				}
			}

			{
				Foo f{ 2 };
				Foo g{ 3 };
				SList<Foo> list{ f,g };
				Assert::AreEqual(size_t(2), list.Size());
				Assert::AreEqual(g, *list.Find(g));

				SList<Foo> emptyList;
				Assert::AreEqual(size_t(0), emptyList.Size());
				Assert::AreEqual(emptyList.end(), emptyList.Find(g));
				Assert::ExpectException<std::runtime_error>([&emptyList, g]() { auto it = emptyList.Find(g); *it; });
			}
		}

		TEST_METHOD(InsertAfter)
		{
			const Foo a{ 10 };
			const Foo b{ 11 };
			const Foo c{ 12 };
			SList<Foo> list{ a,c };

			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());

			SList<Foo>::Iterator it = list.begin();
			list.InsertAfter(it, b);
			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());

			list.Clear();
			list.InsertAfter(list.end(), b);
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(b, list.Front());
			Assert::AreEqual(list.Front(), list.Back());

			list.InsertAfter(list.end(), a);
			Assert::AreEqual(size_t(2), list.Size());
			Assert::AreEqual(a, list.Back());
			Assert::AreEqual(b, list.Front());
		}

		TEST_METHOD(Remove)
		{
			Foo a{ 10 };
			Foo b{ 11 }; 
			Foo c{ 12 };
			Foo e{ 13 };
			SList<Foo> list{ a,b,c };

			Assert::AreEqual(size_t(3), list.Size());
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::IsTrue(list.Remove(b));
			Assert::AreEqual(size_t(2), list.Size());
			Assert::IsFalse(list.Remove(e));
			Assert::AreEqual(size_t(2), list.Size());

			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::IsTrue(list.Remove(a));
			Assert::AreEqual(size_t(1), list.Size());
			Assert::AreEqual(c, list.Front());
			Assert::AreEqual(c, list.Back());
			Assert::IsTrue(list.Remove(c));
			Assert::IsTrue(list.IsEmpty());
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}
