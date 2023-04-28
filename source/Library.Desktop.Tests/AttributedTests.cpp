#include "pch.h"
#include "CppUnitTest.h"
#include "AttributedFoo.h"
#include "TypeManager.h"
#include "AttributedBar.h"
#include "Datum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace std::string_literals;
using namespace UnitTests;
using DatumTypes = Datum::DatumTypes;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<Datum>(const Datum& t)
	{
		RETURN_WIDE_STRING(ToString(Datum::DatumTypesStringMap.At(t.Type())) + ToString(t.Size()) + ToString(t.Capacity()));
	}
}

namespace LibraryDesktopTests
{
	TEST_CLASS(AttributedTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			RegisterType<AttributedFoo, Attributed>();
			RegisterType<AttributedBar, AttributedFoo>();

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
			std::string a = "ExternalInteger"s;
			std::string b = "ExternalFloat"s;
			std::string c = "ExternalString"s;
			std::string d = "ExternalVector"s;
			std::string e = "ExternalMatrix"s;
			std::string f = "ExternalIntegerArray"s;
			std::string g = "ExternalFloatArray"s;
			std::string h = "ExternalStringArray"s;
			std::string i = "ExternalVectorArray"s;
			std::string j = "ExternalMatrixArray"s;
			std::string k = "NestedScope"s;
			std::string l = "NestedScopeArray"s;


			AttributedFoo foo;
			Assert::AreEqual(size_t(13), foo.Size());
			Assert::AreEqual(foo[0], *foo.Find("this"s));
			Assert::IsTrue(foo.ContainsKey(a));
			Assert::IsTrue(foo.ContainsKey(b));
			Assert::IsTrue(foo.ContainsKey(c));
			Assert::IsTrue(foo.ContainsKey(d));
			Assert::IsTrue(foo.ContainsKey(e));
			Assert::IsTrue(foo.ContainsKey(f));
			Assert::IsTrue(foo.ContainsKey(g));
			Assert::IsTrue(foo.ContainsKey(h));
			Assert::IsTrue(foo.ContainsKey(i));
			Assert::IsTrue(foo.ContainsKey(j));
			Assert::IsTrue(foo.ContainsKey(k));
			Assert::IsTrue(foo.ContainsKey(l));
		}

		TEST_METHOD(CopySemantics)
		{
			std::string a = "Monster"s;
			std::string b = "Professor"s;
			std::string c = "ExternalInteger"s;
			std::string d = "ExternalFloat"s;

			{
				AttributedFoo aFoo; 
				Assert::AreEqual(size_t(13), aFoo.Size());
				Assert::IsFalse(aFoo.ContainsKey(a));
				aFoo.AppendAuxiliaryAttribute(a);
				Assert::AreEqual(size_t(14), aFoo.Size());
				Assert::IsTrue(aFoo.ContainsKey(a));
				AttributedFoo bFoo{ aFoo };
				aFoo.ExternalFloat = 10.9f;
				Assert::AreEqual(size_t(14), bFoo.Size());
				Assert::IsTrue(bFoo.ContainsKey(a));
				Assert::IsFalse(aFoo.Equals(&bFoo));
			}
			{
				AttributedFoo aFoo;
				Assert::AreEqual(size_t(13), aFoo.Size());
				Assert::IsFalse(aFoo.ContainsKey(a));
				aFoo.AppendAuxiliaryAttribute(a);
				Assert::AreEqual(size_t(14), aFoo.Size());
				Assert::IsTrue(aFoo.ContainsKey(a));
				AttributedFoo bFoo; 
				Assert::AreEqual(size_t(13), bFoo.Size());
				bFoo = aFoo;
				Assert::IsTrue(aFoo.Equals(&bFoo));
				Assert::AreEqual(size_t(14), bFoo.Size());
				Assert::IsTrue(bFoo.ContainsKey(a));
				
				aFoo.AppendAuxiliaryAttribute(b);
				Assert::AreEqual(size_t(14), bFoo.Size());
				Assert::AreEqual(size_t(15), aFoo.Size());

				//Assert::AreEqual(aFoo.At(c), bFoo.At(c));
				//Assert::AreNotSame(aFoo.At(c), bFoo.At(c));
				//Assert::AreEqual(aFoo.At(d), bFoo.At(d));
			}
		}

		TEST_METHOD(MoveSemantics)
		{
			std::string a = "Monster"s;
			std::string b = "Professor"s;
			{
				AttributedFoo aFoo;
				Assert::AreEqual(size_t(13), aFoo.Size());
				Assert::IsFalse(aFoo.ContainsKey(a));
				aFoo.AppendAuxiliaryAttribute(a);
				Assert::AreEqual(size_t(14), aFoo.Size());
				Assert::IsTrue(aFoo.ContainsKey(a));
				AttributedFoo bFoo{ std::move(aFoo) };
				Assert::AreEqual(size_t(14), bFoo.Size());
				Assert::IsTrue(bFoo.ContainsKey(a));
#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), aFoo.Size());
#pragma warning(pop)
			}
			{
				AttributedFoo aFoo;
				Assert::AreEqual(size_t(13), aFoo.Size());
				Assert::IsFalse(aFoo.ContainsKey(a));
				aFoo.AppendAuxiliaryAttribute(a);
				Assert::AreEqual(size_t(14), aFoo.Size());
				Assert::IsTrue(aFoo.ContainsKey(a));
				AttributedFoo bFoo;
				Assert::AreEqual(size_t(13), bFoo.Size());
				bFoo = std::move(aFoo);
				Assert::AreEqual(size_t(14), bFoo.Size());
				Assert::IsTrue(bFoo.ContainsKey(a));

#pragma warning(push)
#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), aFoo.Size());
#pragma warning(pop)
			}
		}

		TEST_METHOD(Equals)
		{
			Scope scope;
			AttributedFoo aFoo;
			Assert::AreEqual(size_t(13), aFoo.Size());
			Assert::IsFalse(aFoo.Equals(&scope));
		}

		TEST_METHOD(Accessors)
		{
			std::string a = "MonsterFloat"s;
			std::string b = "ProfessorInteger"s;
			AttributedFoo foo;
			Assert::AreEqual(size_t(13), foo.Size());

			Datum& aDatum = foo.AppendAuxiliaryAttribute(a);
			aDatum = 2.0f;
			Assert::AreEqual(size_t(14), foo.Size());

			Datum& bDatum = foo.AppendAuxiliaryAttribute(b);
			bDatum = 3;
			Assert::AreEqual(size_t(15), foo.Size());
			
			auto [first, last] = foo.Attributes();
			Assert::AreEqual(size_t(0), first);
			Assert::AreEqual(size_t(15), last);
			
			auto [firstPrescribed, lastPrescribed] = foo.PrescribedAttributes();
			Assert::AreEqual(size_t(0), firstPrescribed);
			Assert::AreEqual(size_t(13), lastPrescribed);

			auto [firstAuxiliary, lastAuxiliary] = foo.AuxiliaryAttributes();
			Assert::AreEqual(size_t(13), firstAuxiliary);
			Assert::AreEqual(size_t(15), lastAuxiliary);
		}

		TEST_METHOD(AttributeChecks)
		{
			std::string a = "MonsterFloat"s;
			std::string b = "ProfessorInteger"s;
			std::string c = "ExternalInteger"s;
			AttributedFoo foo;
			Assert::AreEqual(size_t(13), foo.Size());

			Datum& aDatum = foo.AppendAuxiliaryAttribute(a);
			aDatum = 2.0f;
			Assert::AreEqual(size_t(14), foo.Size());

			Assert::IsTrue(foo.IsAttribute(a));
			Assert::IsTrue(foo.IsAuxiliaryAttribute(a));
			Assert::IsFalse(foo.IsAttribute(b));
			Assert::IsTrue(foo.IsPrescribedAttribute(c));
		}

		TEST_METHOD(AppendAuxiliaryAttribute)
		{
			std::string a = "MonsterFloat"s;
			std::string b = "";
			AttributedFoo foo;
			Assert::AreEqual(size_t(13), foo.Size());

			Datum& aDatum = foo.AppendAuxiliaryAttribute(a);
			aDatum = 2.0f;
			Assert::AreEqual(size_t(14), foo.Size());
			Assert::ExpectException<std::invalid_argument>([&foo, &a] { foo.AppendAuxiliaryAttribute(a); });
			Assert::ExpectException<std::invalid_argument>([&foo, &b] { foo.AppendAuxiliaryAttribute(b); });
		}

	private: 
		inline static _CrtMemState _startMemState;
	};
}