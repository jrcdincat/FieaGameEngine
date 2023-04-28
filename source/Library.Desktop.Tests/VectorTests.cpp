#include "pch.h"
#include <stdexcept>
#include "CppUnitTest.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Vector.h"

// TODO: Add separate test for Default Equality

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Vector<Foo>::Iterator>(const Vector<Foo>::Iterator& t)
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
	std::wstring ToString<Vector<Foo>::ConstIterator>(const Vector<Foo>::ConstIterator& t)
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
	TEST_CLASS(VectorTests)
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
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.Reserve(11);

			const Vector<Foo>& constVector(vector);
			Assert::AreEqual(size_t(0), constVector.size());
			Assert::AreEqual(size_t(11), constVector.Capacity());
		}

		TEST_METHOD(CopySemantics)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> vector{ a,b,c };

			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.back());

			// Copy constructor
			{
				Vector<Foo> copyOfVector(vector);
				Assert::AreEqual(size_t(3), copyOfVector.size());
				Assert::AreEqual(size_t(3), copyOfVector.Capacity());
				Assert::AreEqual(a, copyOfVector.Front());
				Assert::AreEqual(c, copyOfVector.back());
				Assert::AreNotSame(a, copyOfVector.Front());
				Assert::AreNotSame(c, copyOfVector.back());

				copyOfVector.Clear();
				copyOfVector.ShrinkToFit();
				Vector<Foo> emptyVectorCopy(copyOfVector);
				Assert::AreEqual(size_t(0), copyOfVector.size());
				Assert::AreEqual(size_t(0), copyOfVector.Capacity());
			}

			// Copy assignment operator
			{
				Vector<Foo>copyAssignmentVector;
				copyAssignmentVector = vector;
				Assert::AreEqual(size_t(3), copyAssignmentVector.size());
				Assert::AreEqual(size_t(11), copyAssignmentVector.Capacity());
				Assert::AreEqual(a, copyAssignmentVector.Front());
				Assert::AreEqual(c, copyAssignmentVector.back());
				Assert::AreNotSame(a, copyAssignmentVector.Front());
				Assert::AreNotSame(a, copyAssignmentVector.back());
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			Vector<Foo> vector{ a,b,c };

			// Move construtor
			Vector<Foo> movedVector{ std::move(vector) };
			Assert::AreEqual(size_t(3), movedVector.size());
			Assert::AreEqual(size_t(11), movedVector.Capacity());
			Assert::AreEqual(a, movedVector.Front());
			Assert::AreEqual(c, movedVector.back());
			Assert::AreEqual(b, movedVector[1]);
#pragma warning(push)
#pragma warning(disable:26800)
			Assert::IsTrue(vector.IsEmpty());
#pragma warning(pop)

			Vector<Foo> anotherMovedVector; 
			anotherMovedVector.ShrinkToFit();
			anotherMovedVector = std::move(movedVector);
			Assert::AreEqual(size_t(3), anotherMovedVector.size());
			Assert::AreEqual(size_t(11), anotherMovedVector.Capacity());
			Assert::AreEqual(a, anotherMovedVector.Front());
			Assert::AreEqual(c, anotherMovedVector.back());
			Assert::AreEqual(b, anotherMovedVector[1]);
#pragma warning(push)
#pragma warning(disable:26800)
			Assert::IsTrue(movedVector.IsEmpty());
#pragma warning(pop)
		}

		TEST_METHOD(Index)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };

			Vector<Foo> vector{ a,b,c };
			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.back());
			Assert::AreNotSame(a, vector.Front());
			Assert::AreNotSame(c, vector.back());

			Assert::AreEqual(a, vector[0]);
			Assert::AreEqual(a, vector.At(0));
			Assert::AreEqual(b, vector[1]);
			Assert::AreEqual(b, vector.At(1));
			Assert::AreEqual(c, vector[2]);
			Assert::AreEqual(c, vector.At(2));


			Assert::ExpectException<std::runtime_error>([&vector]() { vector[50]; });
			Assert::ExpectException<std::runtime_error>([&vector]() { vector.At(50); });

			const Vector<Foo> constVector{ a,b,c };
			Assert::AreEqual(a, constVector[0]);
			Assert::AreEqual(a, constVector.At(0));
			Assert::AreEqual(b, constVector[1]);
			Assert::AreEqual(b, constVector.At(1));
			Assert::AreEqual(c, constVector[2]);
			Assert::AreEqual(c, constVector.At(2));
			Assert::ExpectException<std::runtime_error>([&constVector]() { constVector[50]; });
			Assert::ExpectException<std::runtime_error>([&constVector]() { constVector.At(50); });
		}

		TEST_METHOD(Front)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			Vector<Foo> vector{ a,b,c };
			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(a, vector.Front());
			
			const Vector<Foo> constVector{ a, b, c };
			Assert::AreEqual(a, constVector.Front());
		}

		TEST_METHOD(Back)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			Vector<Foo> vector{ a,b };
			Assert::AreEqual(size_t(2), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(b, vector.back());
			vector.push_back(c);
			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(c, vector.back());

			const Vector<Foo> constVector{ a,b,c };
			Assert::AreEqual(size_t(3), constVector.size());
			Assert::AreEqual(size_t(11), constVector.Capacity());
			Assert::AreEqual(c, constVector.back());
		}
		TEST_METHOD(PushBack)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.push_back(a);
			Assert::AreEqual(size_t(1), vector.size());
			Assert::AreEqual(size_t(1), vector.Capacity());
			vector.Reserve(11);
			Assert::AreEqual(a, vector.back());
			vector.push_back(b);
			Assert::AreEqual(size_t(2), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(b, vector.back());
			vector.push_back(c);
			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(c, vector.back());

			vector.push_back(std::move(d));
			Assert::AreEqual(size_t(4), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
#pragma warning(push)
#pragma warning(disable:26800)
			Assert::AreEqual(Foo{ 40 }, vector.back());
#pragma warning(pop)

			vector.ShrinkToFit();
			vector.push_back(std::move(e)); 

#pragma warning(push)
#pragma warning(disable:26800)
			Assert::AreEqual(Foo{ 50 }, vector.back());
#pragma warning(pop)
		}

		TEST_METHOD(PopBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			Vector<Foo> vector{ a,b,c };
			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(a, vector.Front());
			Assert::AreEqual(c, vector.back());
			Assert::AreNotSame(a, vector.Front());
			Assert::AreNotSame(c, vector.back());
			vector.pop_back();
			Assert::AreEqual(size_t(2), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(b, vector.back());
			vector.pop_back();
			Assert::AreEqual(size_t(1), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::AreEqual(a, vector.back());
			vector.pop_back();
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(Clear)
		{
			{
				const Foo a{ 10 };
				const Foo b{ 20 };
				const Foo c{ 30 };
				Vector<Foo> vector{ a,b,c };
				Assert::AreEqual(size_t(3), vector.size());
				Assert::AreEqual(size_t(11), vector.Capacity());
				vector.Clear();
				Assert::AreEqual(size_t(0), vector.size());
				Assert::AreEqual(size_t(11), vector.Capacity());
			}
			{
				Foo a{ 10 };
				Foo b{ 20 };
				Foo c{ 30 };
				Vector<Foo> vector{ a,b,c };
				Assert::AreEqual(size_t(3), vector.size());
				Assert::AreEqual(size_t(11), vector.Capacity());
				vector.Clear();
				Assert::AreEqual(size_t(0), vector.size());
				Assert::AreEqual(size_t(11), vector.Capacity());
			}
		}


		TEST_METHOD(Iterator)
		{
			const Foo a{ 5 };
			const Foo b{ 6 };
			const Foo c{ 7 };

			{
				Vector<Foo> vector;
				vector.push_back(a);
				Assert::AreNotEqual(vector.end(), vector.begin());
			}

			{
				Vector<Foo> vector{ a,b,c };
				{
					size_t count = 0;
					for (auto it = vector.begin(); it != vector.end(); ++it, ++count)
					{
						;
					}
					Assert::AreEqual(vector.size(), count);
				}

				{
					size_t count = 0;
					for (auto it = vector.begin(); it != vector.end(); it++, ++count)
					{
						;
					}
					Assert::AreEqual(vector.size(), count);
				}

				{
					size_t count = 0;
					for (const Foo& value : vector)
					{
						value;
						++count;
					}
					Assert::AreEqual(vector.size(), count);
				}
				
				Vector<Foo>::Iterator iterator = vector.Find(b);
				iterator--;
				Assert::AreEqual(a, *iterator);
				iterator++;
				Assert::AreEqual(b, *iterator);
				--iterator;
				Assert::AreEqual(a, *iterator);
			}



			{
				{
					const Vector<Foo> constVector;
					const Vector<Foo>::Iterator cIterator;
					Assert::AreEqual(constVector.end(), constVector.begin());
					const Vector<Foo>::ConstIterator  constIterator{ cIterator };
					Assert::AreEqual(constVector.cend(), constVector.cbegin());
				}

				{
					const Vector<Foo> constVector{ a,b,c };
					Assert::AreEqual(a, *constVector.cbegin());
					{
						size_t count = 0;
						for (auto it = constVector.cbegin(); it != constVector.cend(); ++it, ++count)
						{
							;
						}
						Assert::AreEqual(constVector.size(), count);
					}

					{
						size_t count = 0;
						for (auto it = constVector.cbegin(); it != constVector.cend(); it++, ++count)
						{
							;
						}
						Assert::AreEqual(constVector.size(), count);
					}

					{
						size_t count = 0;
						for (const Foo& value : constVector)
						{
							value;
							++count;
						}
						Assert::AreEqual(constVector.size(), count);
					}

					{
						Vector<Foo>::ConstIterator constIterator = constVector.Find(b);
						Assert::AreEqual(b, *constIterator);
						constIterator--;
						Assert::AreEqual(a, *constIterator);
						constIterator++;
						Assert::AreEqual(b, *constIterator);
						--constIterator;
						Assert::AreEqual(a, *constIterator);
					}
				}
			}
		}

		TEST_METHOD(Find)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Vector<Foo> vector{ a,b,c };
			Vector<Foo>::Iterator it = vector.Find(b);
			Assert::AreEqual(b, *it);
			Vector<Foo>::Iterator cIt = vector.Find(c);
			Assert::AreEqual(c, *cIt);
			Vector<Foo>::Iterator aIt = vector.Find(a);
			Assert::AreEqual(a, *aIt);
			Vector<Foo>::Iterator notFoundIt = vector.Find(d);
			Assert::AreEqual(vector.end(), notFoundIt);

			Vector<Foo> emptyVector;
			Vector<Foo>::Iterator emptyIt = emptyVector.Find(a);
			Assert::AreEqual(emptyVector.end(), emptyIt);
		}

		TEST_METHOD(Reserve)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Vector<Foo> vector{ a,b,c };

			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(11), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(3), vector.size());
			Assert::AreEqual(size_t(3), vector.Capacity());
			vector.pop_back();
			Assert::AreEqual(size_t(2), vector.size());
			Assert::AreEqual(size_t(3), vector.Capacity());
			vector.pop_back();
			Assert::AreEqual(size_t(1), vector.size());
			Assert::AreEqual(size_t(3), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(1), vector.size());
			Assert::AreEqual(size_t(1), vector.Capacity());
			vector.Clear();
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(1), vector.Capacity());
		}

		TEST_METHOD(ShrinkToFit)
		{
			Vector<Foo> vector;
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(0), vector.Capacity());
			vector.Reserve(30);
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(30), vector.Capacity());
			vector.ShrinkToFit();
			Assert::AreEqual(size_t(0), vector.size());
			Assert::AreEqual(size_t(0), vector.Capacity());
		}

		TEST_METHOD(Remove)
		{
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 30 };
			Foo d{ 40 };
			Foo e{ 50 };
			{
				Vector<Foo> vector{ a,b,c };
				vector.Remove(b);
				Vector<Foo>::Iterator it = vector.Find(b);
				Assert::AreEqual(vector.end(), it);
				Vector<Foo>::Iterator itA = vector.Find(a);
				Assert::AreEqual(a, *itA);
				Vector<Foo>::Iterator itC = vector.Find(c);
				Assert::AreEqual(c, *itC);

				vector.Remove(c);
				Vector<Foo>::Iterator itC1 = vector.Find(c);
				Assert::AreEqual(vector.end(), itC1);

				vector.Remove(a);
				Vector<Foo>::Iterator itA1 = vector.Find(a);
				Assert::AreEqual(vector.end(), itA1);

				Assert::IsTrue(vector.IsEmpty());
			}

			{
				Vector<Foo> vector{ a,b,c };
				vector.Remove(a);
				Vector<Foo>::Iterator it = vector.Find(a);
				Assert::AreEqual(vector.end(), it);

				Vector<Foo> otherVector{ a,b,c };
				Assert::IsFalse(otherVector.Remove(it));
			}
		}
	private:
		inline static _CrtMemState _startMemState;
	};
}