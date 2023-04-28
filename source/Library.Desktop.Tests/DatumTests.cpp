#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Datum.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std::string_literals;
using namespace glm;
using namespace std;

using DatumTypes = Datum::DatumTypes;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<DatumTypes>(const DatumTypes& t)
	{
		std::wstring value;
		try
		{
			value = ToString(static_cast<int>(t));
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}

	template<>
	std::wstring ToString<Datum>(const Datum& t)
	{
		std::wstring value;
		try
		{
			value = ToString(t.Type()) + ToString(t.Capacity()) + ToString(t.Size());
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}

	template<>
	std::wstring ToString<vec4>(const vec4& t)
	{
		std::wstring value;
		try
		{
			value = ToString(t.x + t.r + t.s + t.y + t.g + t.t + t.z + t.z + t.b + t.p + t.w + t.a + t.q);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}

	template<>
	std::wstring ToString<mat4>(const mat4& t)
	{
		std::wstring value;
		try
		{
			value = ToString(t[0] + t[1] + t[2] + t[3]);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}
		return value;
	}

	template<>
	std::wstring ToString<RTTI>(RTTI* t)
	{
		std::wstring value;
		try
		{
			value = t != nullptr ? ToString(t->ToString()) : L"nullptr";
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
	TEST_CLASS(DatumTests)
	{
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
			{
				Datum datum;
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, datum.Type());
			}

			{
				Datum datum(DatumTypes::Vector);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
			}

			{
				const size_t capacity = 5;
				Datum datum(DatumTypes::String, capacity);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(capacity, datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
			}

			{
				const size_t capacity = 5;
				Datum datum(DatumTypes::Float, capacity);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(capacity, datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
			}

			{
				const size_t capacity = 5;
				Datum datum(DatumTypes::String, capacity);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(capacity, datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
			}

			{
				const size_t capacity = 5;
				Datum datum(DatumTypes::Vector, capacity);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(capacity, datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
			}

			{
				const size_t capacity = 5;
				Datum datum(DatumTypes::Matrix, capacity);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(capacity, datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			}

			{
				const size_t capacity = 5;
				Datum datum(DatumTypes::Pointer, capacity);
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(capacity, datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			}
			Assert::ExpectException<runtime_error>([] { Datum datum(DatumTypes::Unknown, 1); });
		}

		TEST_METHOD(CopyConstructor)
		{
			{
				Datum d1;
				Datum d2{ d1 };
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, d1.Type());
			}
			{
				Datum d1{ 10, 20, 30 };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d1.Type());
				
				Datum d2{ d1 };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d2.Type());
				Assert::AreEqual(d1.FrontInteger(), d2.FrontInteger());
				Assert::AreEqual(d1.GetConstInteger(1), d2.GetConstInteger(1));
				Assert::AreEqual(d1.BackInteger(), d2.BackInteger());
				Assert::AreNotSame(d1.FrontInteger(), d2.FrontInteger());
				Assert::AreNotSame(d1.BackInteger(), d2.BackInteger());
			}

			{
				Datum d1{ 10.0f, 20.0f, 30.0f };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Float, d1.Type());

				Datum d2{ d1 };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Float, d2.Type());
				Assert::AreEqual(d1.FrontFloat(), d2.FrontFloat());
				Assert::AreEqual(d1.GetConstFloat(1), d2.GetConstFloat(1));
				Assert::AreEqual(d1.BackFloat(), d2.BackFloat());
				Assert::AreNotSame(d1.FrontFloat(), d2.FrontFloat());
				Assert::AreNotSame(d1.BackFloat(), d2.BackFloat());
			}

			{
				Datum d1{ "A", "B", "C"};
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::String, d1.Type());

				Datum d2{ d1 };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::String, d2.Type());
				Assert::AreEqual(d1.FrontString(), d2.FrontString());
				Assert::AreEqual(d1.GetConstString(1), d2.GetConstString(1));
				Assert::AreEqual(d1.BackString(), d2.BackString());
				Assert::AreNotSame(d1.FrontString(), d2.FrontString());
				Assert::AreNotSame(d1.BackString(), d2.BackString());
			}

			{
				Datum d1{ vec4(10), vec4(20), vec4(30)};
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d1.Type());

				Datum d2{ d1 };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d2.Type());
				Assert::AreEqual(d1.FrontVector(), d2.FrontVector());
				Assert::AreEqual(d1.GetConstVector(1), d2.GetConstVector(1));
				Assert::AreEqual(d1.BackVector(), d2.BackVector());
				Assert::AreNotSame(d1.FrontVector(), d2.FrontVector());
				Assert::AreNotSame(d1.BackVector(), d2.BackVector());
			}

			{
				Datum d1{ mat4(10), mat4(20), mat4(30)};
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d1.Type());

				Datum d2{ d1 };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d2.Type());
				Assert::AreEqual(d1.FrontMatrix(), d2.FrontMatrix());
				Assert::AreEqual(d1.GetConstMatrix(1), d2.GetConstMatrix(1));
				Assert::AreEqual(d1.BackMatrix(), d2.BackMatrix());
				Assert::AreNotSame(d1.FrontMatrix(), d2.FrontMatrix());
				Assert::AreNotSame(d1.BackMatrix(), d2.BackMatrix());
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;

				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d1.Type());

				Datum d2{ d1 };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d2.Type());
				Assert::AreEqual(d1.FrontPointer(), d2.FrontPointer());
				Assert::AreEqual(d1.GetPointer(1), d2.GetPointer(1));
				Assert::AreEqual(d1.BackPointer(), d2.BackPointer());
				Assert::AreNotSame(d1.FrontPointer(), d2.FrontPointer());
				Assert::AreNotSame(d1.BackPointer(), d1.BackPointer());
			}
		}

		TEST_METHOD(MoveConstructor)
		{
			{
				Datum d1;
				Datum d2{ std::move(d1) };
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, d1.Type());
#pragma warning(pop)
				Assert::AreEqual(size_t(0), d2.Size());
				Assert::AreEqual(size_t(0), d2.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, d2.Type());
			}
			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d1.Type());

				Datum d2{ std::move(d1) };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d2.Type());
				Assert::AreEqual(a, d2.FrontInteger());
				Assert::AreEqual(b, d2.GetConstInteger(1));
				Assert::AreEqual(c, d2.BackInteger());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d1.Type());
#pragma warning(pop)
			}

			{
				const float a = 10.0f;
				const float b = 20.0f;
				const float c = 30.0f;
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Float, d1.Type());

				Datum d2{ std::move(d1) };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Float, d2.Type());
				Assert::AreEqual(a, d2.FrontFloat());
				Assert::AreEqual(b, d2.GetConstFloat(1));
				Assert::AreEqual(c, d2.BackFloat());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Float, d1.Type());
#pragma warning(pop)
			}

			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::String, d1.Type());

				Datum d2{ std::move(d1) };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::String, d2.Type());
				Assert::AreEqual(a, d2.FrontString());
				Assert::AreEqual(b, d2.GetConstString(1));
				Assert::AreEqual(c, d2.BackString());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::String, d1.Type());
#pragma warning(pop)
			}

			{
				const vec4 a{10};
				const vec4 b{20};
				const vec4 c{30};
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d1.Type());

				Datum d2{ std::move(d1) };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d2.Type());
				Assert::AreEqual(a, d2.FrontVector());
				Assert::AreEqual(b, d2.GetConstVector(1));
				Assert::AreEqual(c, d2.BackVector());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d1.Type());
#pragma warning(pop)
			}

			{
				const mat4 a{ 10 };
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d1.Type());

				Datum d2{ std::move(d1) };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d2.Type());
				Assert::AreEqual(a, d2.FrontMatrix());
				Assert::AreEqual(b, d2.GetConstMatrix(1));
				Assert::AreEqual(c, d2.BackMatrix());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d1.Type());
#pragma warning(pop)
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d1.Type());

				Datum d2{ std::move(d1) };
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d2.Type());
				Assert::AreEqual(a, d2.FrontPointer());
				Assert::AreEqual(b, d2.GetPointer(1));
				Assert::AreEqual(c, d2.BackPointer());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d1.Type());
#pragma warning(pop)
			}
		}

		TEST_METHOD(MoveAssignmentOperator)
		{
			{
				Datum d1;
				Datum d2;
				d2 = std::move(d1);
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, d1.Type());
#pragma warning(pop)
				Assert::AreEqual(size_t(0), d2.Size());
				Assert::AreEqual(size_t(0), d2.Capacity());
				Assert::AreEqual(DatumTypes::Unknown, d2.Type());
			}
			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d1.Type());

				Datum d2;
				d2 = std::move(d1);
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d2.Type());
				Assert::AreEqual(a, d2.FrontInteger());
				Assert::AreEqual(b, d2.GetConstInteger(1));
				Assert::AreEqual(c, d2.BackInteger());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Integer, d1.Type());
#pragma warning(pop)
			}

			{
				const float a = 10.0f;
				const float b = 20.0f;
				const float c = 30.0f;
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Float, d1.Type());

				Datum d2;
				d2 = std::move(d1);
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Float, d2.Type());
				Assert::AreEqual(a, d2.FrontFloat());
				Assert::AreEqual(b, d2.GetConstFloat(1));
				Assert::AreEqual(c, d2.BackFloat());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Float, d1.Type());
#pragma warning(pop)
			}

			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::String, d1.Type());

				Datum d2; 
				d2 = std::move(d1);
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::String, d2.Type());
				Assert::AreEqual(a, d2.FrontString());
				Assert::AreEqual(b, d2.GetConstString(1));
				Assert::AreEqual(c, d2.BackString());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::String, d1.Type());
#pragma warning(pop)
			}

			{
				const vec4 a{ 10 };
				const vec4 b{ 20 };
				const vec4 c{ 30 };
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d1.Type());

				Datum d2; 
				d2 = std::move(d1);
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d2.Type());
				Assert::AreEqual(a, d2.FrontVector());
				Assert::AreEqual(b, d2.GetConstVector(1));
				Assert::AreEqual(c, d2.BackVector());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Vector, d1.Type());
#pragma warning(pop)
			}

			{
				const mat4 a{ 10 };
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d1.Type());

				Datum d2;
				d2 = std::move(d1);
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d2.Type());
				Assert::AreEqual(a, d2.FrontMatrix());
				Assert::AreEqual(b, d2.GetConstMatrix(1));
				Assert::AreEqual(c, d2.BackMatrix());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, d1.Type());
#pragma warning(pop)
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum d1{ a, b, c };
				Assert::AreEqual(size_t(3), d1.Size());
				Assert::AreEqual(size_t(3), d1.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d1.Type());

				Datum d2;
				d2 = std::move(d1);
				Assert::AreEqual(size_t(3), d2.Size());
				Assert::AreEqual(size_t(3), d2.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d2.Type());
				Assert::AreEqual(a, d2.FrontPointer());
				Assert::AreEqual(b, d2.GetPointer(1));
				Assert::AreEqual(c, d2.BackPointer());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), d1.Size());
				Assert::AreEqual(size_t(0), d1.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, d1.Type());
#pragma warning(pop)
			}
		}

		TEST_METHOD(TypeCastConstructors)
		{
			{
				const int a = 1;
				Datum datum = a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(a, datum.GetInteger());
			}

			{
				const float a = 1.0f;
				Datum datum = a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(a, datum.GetFloat());
			}

			{
				const std::string a = "Hello";
				Datum datum = a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(a, datum.GetString());
			}

			{
				const vec4 a{ 1 };
				Datum datum = a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(a, datum.GetVector());
			}

			{
				const mat4 a{ 1 };
				Datum datum = a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(a, datum.GetMatrix());
			}

			{
				Foo aFoo{ 10 };
				RTTI* a = &aFoo;
				Datum datum = a;
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(a, datum.GetPointer());
			}
		}

		TEST_METHOD(DatumToDatumCopyAssignment)
		{
			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				int d[] = { a, b, c };
				Datum datum;
				datum.SetStorage(d, std::size(d));
				Datum copyDatum{ a,b };
				datum = copyDatum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(size_t(2), copyDatum.Size());
				Assert::AreEqual(size_t(2), copyDatum.Capacity());
				Assert::AreNotSame(datum.FrontInteger(), copyDatum.FrontInteger());
			}

			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				Datum datum{ a,b,c };
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(a, copyDatum.FrontInteger());
				Assert::AreEqual(c, copyDatum.BackInteger());
				Assert::AreNotSame(datum.FrontInteger(), copyDatum.FrontInteger());
			}

			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				int d[] = { a, b, c };
				Datum datum;
				datum.SetStorage(d, std::size(d));
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), copyDatum.Size());
				Assert::AreEqual(size_t(3), copyDatum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, copyDatum.Type());
				Assert::AreEqual(a, copyDatum.FrontInteger());
				Assert::AreEqual(c, copyDatum.BackInteger());
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
			}

			{
				const float a = 10.0f;
				const float b = 20.0f;
				const float c = 30.0f;
				Datum datum{ a,b,c };
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Float , datum.Type());
				Assert::AreEqual(a, copyDatum.FrontFloat());
				Assert::AreEqual(c, copyDatum.BackFloat());
				Assert::AreNotSame(datum.FrontFloat(), copyDatum.FrontFloat());
			}

			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";
				Datum datum{ a,b,c };
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::String, datum.Type());
				Assert::AreEqual(a, copyDatum.FrontString());
				Assert::AreEqual(c, copyDatum.BackString());
				Assert::AreNotSame(datum.FrontString(), copyDatum.FrontString());
			}

			{
				const vec4 a{ 10 };
				const vec4 b{ 20 };
				const vec4 c{ 30 };
				Datum datum{ a,b,c };
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(a, copyDatum.FrontVector());
				Assert::AreEqual(c, copyDatum.BackVector());
				Assert::AreNotSame(datum.FrontVector(), copyDatum.FrontVector());
			}

			{
				const mat4 a{ 10 };
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum datum{ a,b,c };
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(a, copyDatum.FrontMatrix());
				Assert::AreEqual(c, copyDatum.BackMatrix());
				Assert::AreNotSame(datum.FrontMatrix(), copyDatum.FrontMatrix());
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum datum{ a,b,c };
				Datum copyDatum;
				copyDatum = datum;
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Pointer, datum.Type());
				Assert::AreEqual(a, copyDatum.FrontPointer());
				Assert::AreEqual(c, copyDatum.BackPointer());
				Assert::AreNotSame(datum.FrontPointer(), copyDatum.FrontPointer());
			}
		}

#pragma region Equality
		TEST_METHOD(DatumToDatumEquality)
		{
			{
				Datum d1;
				Datum d2 { 2 };
				Assert::IsFalse(d1 == d2);
			}

			{
				Datum d1;
				Datum d2;
				Assert::AreEqual(d1, d1);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}

			{
				Datum d1 = 10;
				Datum d2 = 10;
				Assert::AreEqual(d1, d2);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}

			{
				Datum d1 = 10.0f;
				Datum d2 = 10.0f;
				Assert::AreEqual(d1, d2);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}

			{
				Datum d1 = "A"s;
				Datum d2 = "A"s;
				Assert::AreEqual(d1, d2);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}

			{
				string a = "A"s;
				Datum d1 = "A"s;
				Datum d2;
				d2.SetStorage(&a, 1);
				Assert::AreEqual(d1, d2);
			}

			{
				Datum d1 = vec4(1);
				Datum d2 = vec4(1);
				Assert::AreEqual(d1, d2);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}

			{
				Datum d1 = mat4(1);
				Datum d2 = mat4(1);
				Assert::AreEqual(d1, d2);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}

			{
				Foo aFoo(10);
				Foo bFoo(10);
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				Datum d1 = a;
				Datum d2 = b;
				Assert::AreEqual(d1, d2);
				Assert::IsTrue(d1 == d2);
				Assert::IsTrue(d2 == d1);
			}
		}

		TEST_METHOD(DatumToDatumInequality)
		{
			{
				Datum d1 = 10;
				Datum d2 = 20;
				Assert::IsTrue(d1 != d2);
				Assert::IsTrue(d2 != d1);
			}

			{
				Datum d1 = 10.0f;
				Datum d2 = 20.0f;
				Assert::IsTrue(d1 != d2);
				Assert::IsTrue(d2 != d1);
			}

			{
				Datum d1 = "A"s;
				Datum d2 = "B"s;
				Assert::IsTrue(d1 != d2);
				Assert::IsTrue(d2 != d1);
			}

			{
				Datum d1{ vec4{ 10 } };
				Datum d2{ vec4{ 20 } };
				Assert::IsTrue(d1 != d2);
				Assert::IsTrue(d2 != d1);
			}

			{
				Datum d1{ mat4{ 10 } };
				Datum d2{ mat4{ 20 } };
				Assert::IsTrue(d1 != d2);
				Assert::IsTrue(d2 != d1);
			}

			
			{
				Foo aFoo(10);
				Foo bFoo(20);
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				Datum d1 = a;
				Datum d2 = b;
				Assert::IsTrue(d1 != d2);
				Assert::IsTrue(d2 != d1);
			}			
		}

		TEST_METHOD(DatumToScalarEquality)
		{
			{
				const int a = 10;
				Datum datum = a;
				Assert::IsTrue(datum == a);
				Assert::IsTrue(a == datum);
			}

			{
				const float a = 10.0f;
				Datum datum = a;
				Assert::IsTrue(datum == a);
				Assert::IsTrue(a == datum);
			}

			{
				const string a = "A"s;
				Datum datum = a;
				Assert::IsTrue(datum == a);
				Assert::IsTrue(a == datum);
			}

			{
				const vec4 a(1);
				Datum datum = a;
				Assert::IsTrue(datum == a);
				Assert::IsTrue(a == datum);
			}

			{
				const mat4 a(1);
				Datum datum = a;
				Assert::IsTrue(datum == a);
				Assert::IsTrue(a == datum);
			}

			{
				Foo aFoo(10);
				RTTI* a = &aFoo;

				Datum datum = a;
				Assert::IsTrue(datum == a);
				Assert::IsTrue(a == datum);
			}
		}

		TEST_METHOD(DatumToScalarInEquality)
		{
			Datum datum;
			{
				const int a = 10;
				Assert::IsTrue(datum != a);
				Assert::IsTrue(a != datum);
			}

			{
				const float a = 10.0f;
				Assert::IsTrue(datum != a);
				Assert::IsTrue(a != datum);
			}

			{
				const string a = "A"s;
				Assert::IsTrue(datum != a);
				Assert::IsTrue(a != datum);
			}

			{
				const vec4 a(1);
				Assert::IsTrue(datum != a);
				Assert::IsTrue(a != datum);
			}

			{
				const mat4 a(1);
				Assert::IsTrue(datum != a);
				Assert::IsTrue(a != datum);
			}

			{
				Foo aFoo(10);
				RTTI* a = &aFoo;				
				Assert::IsTrue(datum != a);
				Assert::IsTrue(a != datum);
			}
		}
#pragma endregion
		TEST_METHOD(SetType)
		{
			Datum datum; 
			Assert::AreEqual(DatumTypes::Unknown, datum.Type());
			datum.SetType(DatumTypes::Integer);
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			Assert::ExpectException<invalid_argument>([&datum] { datum.SetType(DatumTypes::Float); });
		}

		TEST_METHOD(Clear)
		{
			{
				const int b = 20;
				const int c = 30;
				Datum datum;
				datum = 10;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			{
				const float b = 20.0f;
				const float c = 30.0f;
				Datum datum;
				datum = 10.0f;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";
				Datum datum;
				datum = a;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			{
				const vec4 b{ 20.0f };
				const vec4 c{ 30.0f };
				Datum datum;
				datum = vec4{ 10 };

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			{
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum datum;
				datum = mat4{ 10 };

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum datum;
				datum = a;

				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(2), datum.Capacity());
			}
			{
				int a = 10;
				Datum datum;
				datum.SetStorage(&a, 1);
				Assert::ExpectException<runtime_error>([&datum] {datum.Clear(); });
			}

			{
				Datum datum = { "A"s , "B", "C" };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				datum.Clear();
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}
		}

		TEST_METHOD(ShrinkToFit)
		{
			{
				int a = 10;
				int b = 20;
				int c = 30;
				int d[] = { a,b,c };

				Datum datum;
				datum.SetStorage(d, std::size(d));
				Assert::ExpectException<runtime_error>([&datum] {datum.ShrinkToFit(); });
			}

			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				Datum datum{ DatumTypes::Integer, 5 };
			
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.ShrinkToFit();
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}
			{
				const float a = 10.0f;
				const float b = 20.0f;
				const float c = 30.0f;
				Datum datum{ DatumTypes::Float, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.ShrinkToFit();
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}
			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";
				Datum datum{ DatumTypes::String, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.ShrinkToFit();
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}
			{
				const vec4 a{ 10 };
				const vec4 b{ 20 };
				const vec4 c{ 30 };
				Datum datum{ DatumTypes::Vector, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.ShrinkToFit();
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}
			{
				const mat4 a{ 10 };
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum datum{ DatumTypes::Matrix, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.ShrinkToFit();
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum datum{ DatumTypes::Pointer, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.ShrinkToFit();
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
			}
		}

		TEST_METHOD(Resize)
		{
			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				Datum datum{ DatumTypes::Integer, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.Resize(4);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(2);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(6);
				Assert::AreEqual(size_t(6), datum.Size());
				Assert::AreEqual(size_t(6), datum.Capacity());
				Assert::AreEqual(0, datum.BackInteger());
			}
			{
				const float a = 10.0f;
				const float b = 20.0f;
				const float c = 30.0f;
				Datum datum{ DatumTypes::Float, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.Resize(4);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(2);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(6);
				Assert::AreEqual(size_t(6), datum.Size());
				Assert::AreEqual(size_t(6), datum.Capacity());
				Assert::AreEqual(0.0f, datum.BackFloat());
			}
			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";
				Datum datum{ DatumTypes::String, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.Resize(4);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(2);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(6);
				Assert::AreEqual(size_t(6), datum.Size());
				Assert::AreEqual(size_t(6), datum.Capacity());
				Assert::AreEqual(std::string{}, datum.BackString());
			}
			{
				const vec4 a{ 10 };
				const vec4 b{ 20 };
				const vec4 c{ 30 };
				Datum datum{ DatumTypes::Vector, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.Resize(4);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(2);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(6);
				Assert::AreEqual(size_t(6), datum.Size());
				Assert::AreEqual(size_t(6), datum.Capacity());
				Assert::AreEqual(vec4{ 0 }, datum.BackVector());
			}
			{
				const mat4 a{ 10 };
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum datum{ DatumTypes::Matrix, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.Resize(4);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(2);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(6);
				Assert::AreEqual(size_t(6), datum.Size());
				Assert::AreEqual(size_t(6), datum.Capacity());
				Assert::AreEqual(mat4{ 0 }, datum.BackMatrix());
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				RTTI* defaultPtr{};
				Datum datum{ DatumTypes::Pointer, 5 };

				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				datum.PushBack(b);
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.Resize(4);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(2);
				Assert::AreEqual(size_t(4), datum.Size());
				Assert::AreEqual(size_t(4), datum.Capacity());
				datum.Resize(6);
				Assert::AreEqual(size_t(6), datum.Size());
				Assert::AreEqual(size_t(6), datum.Capacity());
				Assert::AreEqual(defaultPtr, datum.BackPointer());
			}
		}

#pragma region SetStorage
		TEST_METHOD(SetStorageIntegers)
		{
			{
				int a = 10;
				int b = 20;
				int c = 30;
				Datum datum{ a, b, c };
				Assert::ExpectException<runtime_error>([&datum] { int a{ 1 }; datum.SetStorage(&a, 1); });
				Assert::ExpectException<invalid_argument>([&datum] { int a{ 1 }; datum.SetStorage(&a, 0); });
			}
			int a = 10;
			int b = 20;
			int c = 30;
			int d[] = { a, b, c };
			int e = 40;

			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			Assert::IsTrue(datum.IsExternal());
			Assert::AreEqual(&a, &datum.GetInteger());

			datum.SetStorage(d, std::size(d));
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(&d[0], &datum.GetInteger());
			Assert::AreEqual(d[0], datum.GetInteger(0));
			Assert::AreEqual(d[1], datum.GetInteger(1));
			Assert::AreEqual(d[2], datum.GetInteger(2));

			datum.Set(e);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(&d[0], &datum.GetInteger());
			Assert::AreEqual(e, datum.GetInteger());

			datum.Set(e, 1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetInteger(1));

			datum.Set(e, 2);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetInteger(2));

			Assert::ExpectException<runtime_error>([&datum] { float a = 1.0f; datum.SetStorage(&a, 1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(5); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.PushBack(50); });
		}
		

		TEST_METHOD(SetStorageFloats)
		{
			{
				float a = 10.0f;
				float b = 20.0f;
				float c = 30.0f;
				Datum datum{ a, b, c };
				Assert::ExpectException<runtime_error>([&datum] { float a{ 1.0f }; datum.SetStorage(&a, 1); });
			}

			float a = 10.0f;
			float b = 20.0f;
			float c = 30.0f;
			float d[] = { a, b, c };
			float e = 40.0f;
			
			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Float, datum.Type());
			Assert::IsTrue(datum.IsExternal());
			Assert::AreEqual(&a, &datum.GetFloat());

			datum.SetStorage(d, std::size(d));
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(&d[0], &datum.GetFloat());
			Assert::AreEqual(d[0], datum.GetFloat(0));
			Assert::AreEqual(d[1], datum.GetFloat(1));
			Assert::AreEqual(d[2], datum.GetFloat(2));

			datum.Set(e);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(&d[0], &datum.GetFloat());
			Assert::AreEqual(e, datum.GetFloat());

			datum.Set(e, 1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetFloat(1));

			datum.Set(e, 2);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetFloat(2));

			Assert::ExpectException<runtime_error>([&datum] { int a = 1; datum.SetStorage(&a, 1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(5); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.PushBack(50.0f); });
		}
		TEST_METHOD(SetStorageStrings)
		{
			{
				string a = "A";
				string b = "B";
				string c = "C";
				Datum datum{ a, b, c };
				Assert::ExpectException<runtime_error>([&datum] { string a{ "A" }; datum.SetStorage(&a, 1); });
			}

			string a = "A";
			string b = "B";
			string c = "C";
			string d[] = { a, b, c };
			string e = "D";

			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::String, datum.Type());
			Assert::IsTrue(datum.IsExternal());

			datum.SetStorage(d, std::size(d));
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(d[0], datum.GetString(0));
			Assert::AreEqual(d[1], datum.GetString(1));
			Assert::AreEqual(d[2], datum.GetString(2));

			datum.Set(e);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetString());

			datum.Set(e, 1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetString(1));

			datum.Set(e, 2);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetString(2));

			Assert::ExpectException<runtime_error>([&datum] { vec4 a{ 1 }; datum.SetStorage(&a, 1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(5); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.PushBack("blah"s); });
		}

		TEST_METHOD(SetStorageVector)
		{
			{
				vec4 a{ 10 };
				vec4 b{ 20 };
				vec4 c{ 30 };
				Datum datum{ a, b, c };
				Assert::ExpectException<runtime_error>([&datum] { vec4 a{ 1 }; datum.SetStorage(&a, 1); });
			}

			vec4 a{ 10 };
			vec4 b{ 20 };
			vec4 c{ 30 };
			vec4 d[] = { a, b, c };
			vec4 e{ 50 };

			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Vector, datum.Type());
			Assert::IsTrue(datum.IsExternal());

			datum.SetStorage(d, std::size(d));
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(d[0], datum.GetVector(0));
			Assert::AreEqual(d[1], datum.GetVector(1));
			Assert::AreEqual(d[2], datum.GetVector(2));

			datum.Set(e);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetVector());

			datum.Set(e, 1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetVector(1));

			datum.Set(e, 2);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetVector(2));

			Assert::ExpectException<runtime_error>([&datum] { string a = "A"; datum.SetStorage(&a, 1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(5); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.PushBack(vec4(23)); });
		}

		TEST_METHOD(SetStorageMatrices)
		{
			{
				mat4 a{ 10 };
				mat4 b{ 20 };
				mat4 c{ 30 };
				Datum datum{ a, b, c };
				Assert::ExpectException<runtime_error>([&datum] { mat4 a{ 1 }; datum.SetStorage(&a, 1); });
			}

			mat4 a{ 10 };
			mat4 b{ 20 };
			mat4 c{ 30 };
			mat4 d[] = { a, b, c };
			mat4 e{ 50 };

			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			Assert::IsTrue(datum.IsExternal());

			datum.SetStorage(d, std::size(d));
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(d[0], datum.GetMatrix(0));
			Assert::AreEqual(d[1], datum.GetMatrix(1));
			Assert::AreEqual(d[2], datum.GetMatrix(2));

			datum.Set(e);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetMatrix());

			datum.Set(e, 1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetMatrix(1));

			datum.Set(e, 2);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetMatrix(2));

			Assert::ExpectException<runtime_error>([&datum] { Foo aFoo{ 1 }; RTTI* a = &aFoo; datum.SetStorage(&a, 1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(5); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.PushBack(mat4(23)); });
		}

		TEST_METHOD(SetStoragePointers)
		{

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum datum{ a, b, c };
				Assert::ExpectException<runtime_error>([&datum] { Foo eFoo{ 1 }; RTTI* a = &eFoo; datum.SetStorage(&a, 1); });
			}

			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;
			RTTI* d[] = { a, b, c };
			Foo eFoo{ 30 };
			RTTI* e = &eFoo;

			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			Assert::IsTrue(datum.IsExternal());

			datum.SetStorage(d, std::size(d));
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(d[0], datum.GetPointer(0));
			Assert::AreEqual(d[1], datum.GetPointer(1));
			Assert::AreEqual(d[2], datum.GetPointer(2));

			datum.Set(e);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetPointer());

			datum.Set(e, 1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetPointer(1));

			datum.Set(e, 2);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(e, datum.GetPointer(2));

			Assert::ExpectException<runtime_error>([&datum] { mat4 a{ 1 }; datum.SetStorage(&a, 1); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(5); });
			Assert::ExpectException<runtime_error>([&datum] { datum.Reserve(1); });
			Assert::ExpectException<runtime_error>([&datum, c] { datum.PushBack(c); });
		}
#pragma endregion

#pragma region Set
		TEST_METHOD(SetInteger)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;
			const int d = 40;
			const int e = 50;
			Datum datum{ a, b, c };

			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			datum.Set(d, 1);
			Assert::AreEqual(d, datum.GetConstInteger(1));
			datum.Set(e, 0);
			Assert::AreEqual(e, datum.FrontInteger());
			Assert::ExpectException<invalid_argument>([&datum, e] { datum.Set(e, 20); });
		}

		TEST_METHOD(SetFloat)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;
			const float d = 40.0f;
			const float e = 50.0f;
			Datum datum{ a, b, c };

			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::Float, datum.Type());
			datum.Set(d, 1);
			Assert::AreEqual(d, datum.GetConstFloat(1));
			datum.Set(e, 0);
			Assert::AreEqual(e, datum.FrontFloat());
			Assert::ExpectException<invalid_argument>([&datum, e] { datum.Set(e, 20); });
		}

		TEST_METHOD(SetStrings)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";
			const std::string d = "D";
			const std::string e = "E";
			Datum datum{ a, b, c };

			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::String, datum.Type());
			datum.Set(d, 1);
			Assert::AreEqual(d, datum.GetConstString(1));
			datum.Set(e, 0);
			Assert::AreEqual(e, datum.FrontString());
			Assert::ExpectException<invalid_argument>([&datum, e] { datum.Set(e, 20); });
		}

		TEST_METHOD(SetVector)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };
			const vec4 d{ 40 };
			const vec4 e{ 50 };
			Datum datum{ a, b, c };

			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::Vector, datum.Type());
			datum.Set(d, 1);
			Assert::AreEqual(d, datum.GetConstVector(1));
			datum.Set(e, 0);
			Assert::AreEqual(e, datum.FrontVector());
			Assert::ExpectException<invalid_argument>([&datum, e] { datum.Set(e, 20); });
		}

		TEST_METHOD(SetMatrix)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };
			const mat4 d{ 40 };
			const mat4 e{ 50 };
			Datum datum{ a, b, c };

			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			datum.Set(d, 1);
			Assert::AreEqual(d, datum.GetConstMatrix(1));
			datum.Set(e, 0);
			Assert::AreEqual(e, datum.FrontMatrix());
			Assert::ExpectException<invalid_argument>([&datum, e] { datum.Set(e, 20); });
		}

		TEST_METHOD(SetPointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			Foo dFoo{ 40 };
			Foo eFoo{ 50 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;
			RTTI* d = &dFoo;
			RTTI* e = &eFoo;
			Datum datum{ a, b, c };

			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			datum.Set(d, 1);
			Assert::AreEqual(d, datum.GetPointer(1));
			datum.Set(e, 0);
			Assert::AreEqual(e, datum.FrontPointer());
			Assert::ExpectException<invalid_argument>([&datum, e] { datum.Set(e, 20); });
		}
#pragma endregion

		TEST_METHOD(SetIntegerFromString)
		{
			std::string a{ "1"s };
			std::string b{ "2"s };

			{
				Datum datum{ DatumTypes::Integer };
				Assert::ExpectException<invalid_argument>([&datum, a] { datum.SetFromString(a); });
			}

			{
				Datum datum{ 10,20,30 };
				datum.SetFromString(a);
				Assert::AreEqual(1, datum.FrontInteger());
			}
			
			{
				Datum datum{ 10,20,30 };
				datum.SetFromString(b, 1);
				Assert::AreEqual(2, datum.GetInteger(1));
			}
		}
		TEST_METHOD(SetFloatFromString)
		{
			std::string a{ "1.0"s };
			float b = 1.0f;

			{
				Datum datum{ DatumTypes::Float };
				Assert::ExpectException<invalid_argument>([&datum, a] { datum.SetFromString(a); });
			}

			{
				Datum datum{ 10.0f, 20.0f, 30.0f };
				datum.SetFromString(a);
				Assert::AreEqual(b, datum.FrontFloat());
			}

			{
				Datum datum{ 10.0f, 20.0f, 30.0f };
				datum.SetFromString(a, 1);
				Assert::AreEqual(b, datum.GetFloat(1));
			}
		}

		TEST_METHOD(SetStringFromString)
		{
			std::string a{ "1"s };
			std::string b{ "2"s };

			{
				Datum datum{ DatumTypes::String };
				Assert::ExpectException<invalid_argument>([&datum, a] { datum.SetFromString(a); });
			}

			{
				Datum datum{ "A","B","C" };
				datum.SetFromString(a);
				Assert::AreEqual(a, datum.FrontString());
			}

			{
				Datum datum{ "A","B","C" };
				datum.SetFromString(b, 1);
				Assert::AreEqual(b, datum.GetString(1));
			}
		}

		TEST_METHOD(SetVectorFromString)
		{
			std::string a{ "vec4( 1.0, 1.0, 1.0, 1.0 )"s };
			vec4 b{ 1 };
			{
				Datum datum{ DatumTypes::Vector };
				Assert::ExpectException<invalid_argument>([&datum, a] { datum.SetFromString(a); });
			}

			{
				Datum datum{ vec4{ 2 } };
				datum.SetFromString(a);
				Assert::AreEqual(b, datum.FrontVector());
			}

			{
				Datum datum{ vec4{ 2 }, vec4{ 5 } };
				datum.SetFromString(a, 1);
				Assert::AreEqual(b, datum.GetVector(1));
			}
		}
		TEST_METHOD(SetMatrixFromString)
		{
			std::string a{ "mat4(vec4( 1.0, 1.0, 1.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ), vec4( 1.0, 1.0, 1.0, 1.0 ))"s };
			mat4 b{ vec4(1), vec4(1), vec4(1),vec4(1) };

			{
				Datum datum{ DatumTypes::Matrix };
				Assert::ExpectException<invalid_argument>([&datum, a] { datum.SetFromString(a); });
			}

			{
				Datum datum{ b };
				datum.SetFromString(a);
				Assert::AreEqual(b, datum.FrontMatrix());
			}

			{
				Datum datum{ mat4(5), mat4(6) };
				datum.SetFromString(a, 1);
				Assert::AreEqual(b, datum.GetMatrix(1));
			}
		}

		TEST_METHOD(ToString)
		{
			{
				Datum datum{ 10, 20, 30 };
				Assert::AreEqual("10"s, datum.ToString(0));
				Assert::AreEqual("20"s, datum.ToString(1));
				Assert::ExpectException<invalid_argument>([&datum] { datum.ToString(50); });
			}

			{
				float a = 10.0f;
				float b = 20.0f;
				float c = 30.0f;
				Datum datum{ a, b, c };
				Assert::AreEqual(std::to_string(a), datum.ToString(0));
				Assert::AreEqual(std::to_string(b), datum.ToString(1));
				Assert::ExpectException<invalid_argument>([&datum] { datum.ToString(50); });
			}

			{
				std::string a = "A";
				std::string b = "B";
				std::string c = "C";
				Datum datum{ a, b, c };
				Assert::AreEqual(a, datum.ToString(0));
				Assert::AreEqual(b, datum.ToString(1));
				Assert::ExpectException<invalid_argument>([&datum] { datum.ToString(50); });
			}

			{
				vec4 a{ 1 };
				vec4 b{ 1 };
				vec4 c{ 1 };
				Datum datum{ a, b, c };
				Assert::AreEqual(glm::to_string(a), datum.ToString(0));
				Assert::AreEqual(glm::to_string(b), datum.ToString(1));
				Assert::ExpectException<invalid_argument>([&datum] { datum.ToString(50); });
			}

			{
				mat4 a{ 1 };
				mat4 b{ 1 };
				mat4 c{ 1 };
				Datum datum{ a, b, c };
				Assert::AreEqual(glm::to_string(a), datum.ToString(0));
				Assert::AreEqual(glm::to_string(b), datum.ToString(1));
				Assert::ExpectException<invalid_argument>([&datum] { datum.ToString(50); });
			}

			{
				Foo aFoo{ 10 };
				Foo bFoo{ 20 };
				Foo cFoo{ 30 };
				RTTI* a = &aFoo;
				RTTI* b = &bFoo;
				RTTI* c = &cFoo;
				Datum datum{ a, b, c };
				std::string aString = a->ToString();
				std::string bString = b->ToString();
				Assert::AreEqual(aString, datum.ToString(0));
				Assert::AreEqual(bString, datum.ToString(1));
				Assert::ExpectException<invalid_argument>([&datum] { datum.ToString(50); });
			}
		}

#pragma region Get
		TEST_METHOD(GetInteger)
		{
			{
				const int a = 10;
				const int b = 20;
				const int c = 30;
				Datum datum{ a, b, c};
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(a, datum.GetConstInteger());
				Assert::AreEqual(a, datum.GetInteger());
				Assert::AreEqual(b, datum.GetConstInteger(1));
				Assert::AreEqual(b, datum.GetInteger(1));
				Assert::AreEqual(c, datum.GetConstInteger(2));
				Assert::AreEqual(c, datum.GetInteger(2));
			}
			{
				int a = 10;
				int b = 20;
			    int c = 30;
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Integer, datum.Type());
				Assert::AreEqual(a, datum.GetConstInteger());
				Assert::AreEqual(a, datum.GetInteger());
				Assert::AreEqual(b, datum.GetConstInteger(1));
				Assert::AreEqual(b, datum.GetInteger(1));
				Assert::AreEqual(c, datum.GetConstInteger(2));
				Assert::AreEqual(c, datum.GetInteger(2));
			}
		}

		TEST_METHOD(GetFloat)
		{
			{
				const float a = 10;
				const float b = 20;
				const float c = 30;
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(a, datum.GetConstFloat());
				Assert::AreEqual(a, datum.GetFloat());
				Assert::AreEqual(b, datum.GetConstFloat(1));
				Assert::AreEqual(b, datum.GetFloat(1));
				Assert::AreEqual(c, datum.GetConstFloat(2));
				Assert::AreEqual(c, datum.GetFloat(2));
			}
			{
				float a = 10;
				float b = 20;
				float c = 30;
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Float, datum.Type());
				Assert::AreEqual(a, datum.GetConstFloat());
				Assert::AreEqual(a, datum.GetFloat());
				Assert::AreEqual(b, datum.GetConstFloat(1));
				Assert::AreEqual(b, datum.GetFloat(1));
				Assert::AreEqual(c, datum.GetConstFloat(2));
				Assert::AreEqual(c, datum.GetFloat(2));
			}
		}

		TEST_METHOD(GetString)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";
			Datum datum{ a, b, c };
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::String, datum.Type());
			Assert::AreEqual(a, datum.GetConstString());
			Assert::AreEqual(a, datum.GetString());
			Assert::AreEqual(b, datum.GetConstString(1));
			Assert::AreEqual(b, datum.GetString(1));
			Assert::AreEqual(c, datum.GetConstString(2));
			Assert::AreEqual(c, datum.GetString(2));
		}

		TEST_METHOD(GetVector)
		{
			{
				const vec4 a{ 10 };
				const vec4 b{ 20 };
				const vec4 c{ 30 };
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(a, datum.GetConstVector());
				Assert::AreEqual(a, datum.GetVector());
				Assert::AreEqual(b, datum.GetConstVector(1));
				Assert::AreEqual(b, datum.GetVector(1));
				Assert::AreEqual(c, datum.GetConstVector(2));
				Assert::AreEqual(c, datum.GetVector(2));
			}
			{
				vec4 a{ 10 };
				vec4 b{ 20 };
				vec4 c{ 30 };
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Vector, datum.Type());
				Assert::AreEqual(a, datum.GetConstVector());
				Assert::AreEqual(a, datum.GetVector());
				Assert::AreEqual(b, datum.GetConstVector(1));
				Assert::AreEqual(b, datum.GetVector(1));
				Assert::AreEqual(c, datum.GetConstVector(2));
				Assert::AreEqual(c, datum.GetVector(2));
			}
		}

		TEST_METHOD(GetMatrix)
		{
			{
				const mat4 a{ 10 };
				const mat4 b{ 20 };
				const mat4 c{ 30 };
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(a, datum.GetConstMatrix());
				Assert::AreEqual(a, datum.GetMatrix());
				Assert::AreEqual(b, datum.GetConstMatrix(1));
				Assert::AreEqual(b, datum.GetMatrix(1));
				Assert::AreEqual(c, datum.GetConstMatrix(2));
				Assert::AreEqual(c, datum.GetMatrix(2));
			}
			{
				mat4 a{ 10 };
				mat4 b{ 20 };
				mat4 c{ 30 };
				Datum datum{ a, b, c };
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(3), datum.Capacity());
				Assert::AreEqual(DatumTypes::Matrix, datum.Type());
				Assert::AreEqual(a, datum.GetConstMatrix());
				Assert::AreEqual(a, datum.GetMatrix());
				Assert::AreEqual(b, datum.GetConstMatrix(1));
				Assert::AreEqual(b, datum.GetMatrix(1));
				Assert::AreEqual(c, datum.GetConstMatrix(2));
				Assert::AreEqual(c, datum.GetMatrix(2));
			}
		}

		TEST_METHOD(GetPointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;
	
			Datum datum{ a, b, c };
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			Assert::AreEqual(a, datum.GetPointer());
			Assert::AreEqual(b, datum.GetPointer(1));
			Assert::AreEqual(c, datum.GetPointer(2));
		}
#pragma endregion

#pragma region PushBack
		TEST_METHOD(PushBackInteger)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;
			
			{
				Datum datum{ a };
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
			}

			{
				Datum datum{ DatumTypes::Integer, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(b);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetInteger());
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetInteger());
				Assert::AreEqual(c, datum.GetInteger(1));
				Assert::AreEqual(c, datum.GetConstInteger(1));
			}
		}


		TEST_METHOD(PushBackFloat)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;

			{
				Datum datum{ a };
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
			}

			{
				Datum datum{ DatumTypes::Float, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(b);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetFloat());
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetFloat());
				Assert::AreEqual(c, datum.GetFloat(1));
				Assert::AreEqual(c, datum.GetConstFloat(1));
			}
		}
		
		TEST_METHOD(PushBackString)
		{
			{
				const std::string a = "A";
				const std::string b = "B";
				const std::string c = "C";

				{
					Datum datum{ a };
					Assert::AreEqual(size_t(1), datum.Size());
					Assert::AreEqual(size_t(1), datum.Capacity());
					datum.PushBack(b);
				}

				{
					Datum datum{ DatumTypes::String };
					Assert::AreEqual(size_t(0), datum.Size());
					Assert::AreEqual(size_t(0), datum.Capacity());
					datum.PushBack(b);
					datum.Reserve(5);
					Assert::AreEqual(size_t(1), datum.Size());
					Assert::AreEqual(size_t(5), datum.Capacity());
					Assert::AreEqual(b, datum.GetString());
					datum.PushBack(c);
					Assert::AreEqual(size_t(2), datum.Size());
					Assert::AreEqual(size_t(5), datum.Capacity());
					Assert::AreEqual(b, datum.GetString());
					Assert::AreEqual(c, datum.GetString(1));
					Assert::AreEqual(c, datum.GetConstString(1));
				}
			}
			{
				std::string a = "A";
				std::string b = "B";
				std::string c = "C";
				Datum datum{ DatumTypes::String };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(0), datum.Capacity());
				datum.PushBack(std::move(b));
				datum.Reserve(5);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(b, datum.GetString());
				datum.PushBack(std::move(c));
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetString());
				Assert::AreEqual(c, datum.GetString(1));
				Assert::AreEqual(c, datum.GetConstString(1));
#pragma warning(pop)
			}
		}

		TEST_METHOD(PushBackVector)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };

			{
				Datum datum{ a };
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
			}

			{
				Datum datum{ DatumTypes::Vector, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(b);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetVector());
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetVector());
				Assert::AreEqual(c, datum.GetVector(1));
				Assert::AreEqual(c, datum.GetConstVector(1));
			}
		}

		TEST_METHOD(PushBackMatrix)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };

			{
				Datum datum{ a };
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
			}

			{
				Datum datum{ DatumTypes::Matrix, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(b);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetMatrix());
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetMatrix());
				Assert::AreEqual(c, datum.GetMatrix(1));
				Assert::AreEqual(c, datum.GetConstMatrix(1));
			}
		}

		TEST_METHOD(PushBackPointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;

			{
				Datum datum{ a };
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(1), datum.Capacity());
				datum.PushBack(b);
			}

			{
				Datum datum{ DatumTypes::Pointer, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(b);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetPointer());
				datum.PushBack(c);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.GetPointer());
				Assert::AreEqual(c, datum.GetPointer(1));
			}
		}
#pragma endregion

#pragma region PopBack()
		TEST_METHOD(PopBackInteger)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;

			{
				Datum datum{ DatumTypes::Integer, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetInteger());
				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetInteger());
				Assert::AreEqual(b, datum.GetInteger(1));
				Assert::AreEqual(b, datum.GetConstInteger(1));
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PopBackInteger();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.BackInteger());
			}
		}

		TEST_METHOD(PopBackFloat)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;

			{
				Datum datum{ DatumTypes::Float, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetFloat());
				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetFloat());
				Assert::AreEqual(b, datum.GetFloat(1));
				Assert::AreEqual(b, datum.GetConstFloat(1));
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PopBackFloat();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.BackFloat());
			}
		}

		TEST_METHOD(PopBackString)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";

			{
				Datum datum{ DatumTypes::String, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetString());
				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetString());
				Assert::AreEqual(b, datum.GetString(1));
				Assert::AreEqual(b, datum.GetConstString(1));
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PopBackString();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.BackString());
			}
		}

		TEST_METHOD(PopBackVector)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };

			{
				Datum datum{ DatumTypes::Vector, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetVector());
				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetVector());
				Assert::AreEqual(b, datum.GetVector(1));
				Assert::AreEqual(b, datum.GetConstVector(1));
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PopBackVector();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.BackVector());
			}
		}

		TEST_METHOD(PopBackMatrix)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };

			{
				Datum datum{ DatumTypes::Matrix, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetMatrix());
				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetMatrix());
				Assert::AreEqual(b, datum.GetMatrix(1));
				Assert::AreEqual(b, datum.GetConstMatrix(1));
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PopBackMatrix();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.BackMatrix());
			}
		}

		TEST_METHOD(PopBackPointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;

			{
				Datum datum{ DatumTypes::Pointer, 5 };
				Assert::AreEqual(size_t(0), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PushBack(a);
				Assert::AreEqual(size_t(1), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetPointer());
				datum.PushBack(b);
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(a, datum.GetPointer());
				Assert::AreEqual(b, datum.GetPointer(1));
				datum.PushBack(c);
				Assert::AreEqual(size_t(3), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				datum.PopBackPointer();
				Assert::AreEqual(size_t(2), datum.Size());
				Assert::AreEqual(size_t(5), datum.Capacity());
				Assert::AreEqual(b, datum.BackPointer());
			}
		}
#pragma endregion

#pragma region Front
		TEST_METHOD(FrontInteger)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;
			
			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(a, datum.FrontInteger());
			Assert::AreEqual(datum.GetInteger(), datum.FrontInteger());
		}

		TEST_METHOD(FrontFloat)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Float, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(a, datum.FrontFloat());
			Assert::AreEqual(datum.GetFloat(), datum.FrontFloat());
		}

		TEST_METHOD(FrontString)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::String, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(a, datum.FrontString());
			Assert::AreEqual(datum.GetString(), datum.FrontString());
		}

		TEST_METHOD(FrontVector)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Vector, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(a, datum.FrontVector());
			Assert::AreEqual(datum.GetVector(), datum.FrontVector());
		}

		TEST_METHOD(FrontMatrix)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(a, datum.FrontMatrix());
			Assert::AreEqual(datum.GetMatrix(), datum.FrontMatrix());
		}

		TEST_METHOD(FrontPointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(a, datum.FrontPointer());
			Assert::AreEqual(datum.GetPointer(), datum.FrontPointer());
		}
#pragma endregion

#pragma region Back
		TEST_METHOD(BackInteger)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Integer, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(c, datum.BackInteger());
			Assert::AreEqual(datum.GetInteger(2), datum.BackInteger());
		}

		TEST_METHOD(BackFloat)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Float, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(c, datum.BackFloat());
			Assert::AreEqual(datum.GetFloat(2), datum.BackFloat());
		}

		TEST_METHOD(BackString)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::String, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(c, datum.BackString());
			Assert::AreEqual(datum.GetString(2), datum.BackString());
		}

		TEST_METHOD(BackVector)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Vector, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(c, datum.BackVector());
			Assert::AreEqual(datum.GetVector(2), datum.BackVector());
		}

		TEST_METHOD(BackMatrix)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(c, datum.BackMatrix());
			Assert::AreEqual(datum.GetMatrix(2), datum.BackMatrix());
		}

		TEST_METHOD(BackPointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;

			Datum datum{ a };
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Pointer, datum.Type());
			datum.PushBack(b);
			datum.PushBack(c);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(3), datum.Capacity());
			Assert::AreEqual(c, datum.BackPointer());
			Assert::AreEqual(datum.GetPointer(2), datum.BackPointer());
		}
#pragma endregion

#pragma region Remove
		TEST_METHOD(RemoveInteger)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;
			const int d = 40;

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontInteger());
			datum.Remove(b);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontInteger());
			Assert::AreEqual(c, datum.GetConstInteger(1));
			Assert::AreEqual(d, datum.GetConstInteger(2));
			Assert::AreEqual(d, datum.BackInteger());
			Assert::IsFalse(datum.Remove(b));
		}

		TEST_METHOD(RemoveIntegerAt)
		{
			const int a = 10;
			const int b = 20;
			const int c = 30;
			const int d = 40;

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontInteger());
			datum.RemoveAt(1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontInteger());
			Assert::AreEqual(c, datum.GetConstInteger(1));
			Assert::AreEqual(d, datum.GetConstInteger(2));
			Assert::AreEqual(d, datum.BackInteger());
			Assert::IsFalse(datum.Remove(b));
			Assert::IsFalse(datum.Remove(2));
		}

		TEST_METHOD(RemoveFloat)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;
			const float d = 40.0f;

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontFloat());
			datum.Remove(b);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontFloat());
			Assert::AreEqual(c, datum.GetConstFloat(1));
			Assert::AreEqual(d, datum.GetConstFloat(2));
			Assert::AreEqual(d, datum.BackFloat());
			Assert::IsFalse(datum.Remove(b));
		}

		TEST_METHOD(RemoveFloatAt)
		{
			const float a = 10.0f;
			const float b = 20.0f;
			const float c = 30.0f;
			const float d = 40.0f;

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontFloat());
			datum.RemoveAt(1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontFloat());
			Assert::AreEqual(c, datum.GetConstFloat(1));
			Assert::AreEqual(d, datum.GetConstFloat(2));
			Assert::AreEqual(d, datum.BackFloat());
			Assert::IsFalse(datum.Remove(b));
			Assert::IsFalse(datum.Remove(2));
		}

		TEST_METHOD(RemoveString)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";
			const std::string d = "D";

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontString());
			datum.Remove(b);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontString());
			Assert::AreEqual(c, datum.GetConstString(1));
			Assert::AreEqual(d, datum.GetConstString(2));
			Assert::AreEqual(d, datum.BackString());
			Assert::IsFalse(datum.Remove(b));
		}

		TEST_METHOD(RemoveStringAt)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";
			const std::string d = "D";

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontString());
			datum.RemoveAt(1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontString());
			Assert::AreEqual(c, datum.GetConstString(1));
			Assert::AreEqual(d, datum.GetConstString(2));
			Assert::AreEqual(d, datum.BackString());
			Assert::IsFalse(datum.Remove(b));
			Assert::IsFalse(datum.Remove(2));
		}

		TEST_METHOD(RemoveVector)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };
			const vec4 d{ 40 };

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontVector());
			datum.Remove(b);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontVector());
			Assert::AreEqual(c, datum.GetConstVector(1));
			Assert::AreEqual(d, datum.GetConstVector(2));
			Assert::AreEqual(d, datum.BackVector());
			Assert::IsFalse(datum.Remove(b));
		}

		TEST_METHOD(RemoveVectorAt)
		{
			const vec4 a{ 10 };
			const vec4 b{ 20 };
			const vec4 c{ 30 };
			const vec4 d{ 40 };

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontVector());
			datum.RemoveAt(1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontVector());
			Assert::AreEqual(c, datum.GetConstVector(1));
			Assert::AreEqual(d, datum.GetConstVector(2));
			Assert::AreEqual(d, datum.BackVector());
			Assert::IsFalse(datum.Remove(b));
			Assert::IsFalse(datum.Remove(2));
		}

		TEST_METHOD(RemoveMatrix)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };
			const mat4 d{ 40 };

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontMatrix());
			datum.Remove(b);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontMatrix());
			Assert::AreEqual(c, datum.GetConstMatrix(1));
			Assert::AreEqual(d, datum.GetConstMatrix(2));
			Assert::AreEqual(d, datum.BackMatrix());
			Assert::IsFalse(datum.Remove(b));
		}

		TEST_METHOD(RemoveMatrixAt)
		{
			const mat4 a{ 10 };
			const mat4 b{ 20 };
			const mat4 c{ 30 };
			const mat4 d{ 40 };

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontMatrix());
			datum.RemoveAt(1);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontMatrix());
			Assert::AreEqual(c, datum.GetConstMatrix(1));
			Assert::AreEqual(d, datum.GetConstMatrix(2));
			Assert::AreEqual(d, datum.BackMatrix());
			Assert::IsFalse(datum.Remove(b));
			Assert::IsFalse(datum.Remove(2));
		}

		TEST_METHOD(RemoveAtExternalCheck)
		{
			mat4 a{ 10 };
			Datum datum;
			datum.SetStorage(&a, 1);
			Assert::AreEqual(size_t(1), datum.Size());
			Assert::AreEqual(size_t(1), datum.Capacity());
			Assert::AreEqual(DatumTypes::Matrix, datum.Type());
			Assert::IsTrue(datum.IsExternal());
			Assert::ExpectException<std::runtime_error>([&datum] { datum.RemoveAt(0); });
		}

		TEST_METHOD(RemovePointer)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };
			Foo cFoo{ 30 };
			Foo dFoo{ 40 };
			RTTI* a = &aFoo;
			RTTI* b = &bFoo;
			RTTI* c = &cFoo;
			RTTI* d = &dFoo;

			Datum datum = { a, b, c, d };
			Assert::AreEqual(size_t(4), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontPointer());
			datum.Remove(b);
			Assert::AreEqual(size_t(3), datum.Size());
			Assert::AreEqual(size_t(4), datum.Capacity());
			Assert::AreEqual(a, datum.FrontPointer());
			Assert::AreEqual(c, datum.GetPointer(1));
			Assert::AreEqual(d, datum.GetPointer(2));
			Assert::AreEqual(d, datum.BackPointer());
			Assert::IsFalse(datum.Remove(b));
		}

		TEST_METHOD(RTTI_Test)
		{
			Foo aFoo{ 10 };
			Foo bFoo{ 20 };

			RTTI* a = &aFoo;
			RTTI* b = &bFoo;

			Bar aBar{ 30 };
			RTTI* c = &aBar;

			Assert::IsFalse(c->Equals(b));
			Assert::AreEqual("RTTI"s, c->ToString());
			Assert::IsFalse(a->Is(1));

			Foo cFoo;
			RTTI* d = &cFoo;
			Bar* cBar = d->As<Bar>();
			Assert::IsNull(cBar);
		}
#pragma endregion

	private:
		inline static _CrtMemState _startMemState;
	};
}
