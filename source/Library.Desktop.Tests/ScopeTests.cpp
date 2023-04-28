#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "ToStringSpecializations.h"
#include "Datum.h"
#include <string>
#include "Scope.h"

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
	inline std::wstring ToString<Scope>(Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTests
{
	class Monster : public Scope
	{
		RTTI_DECLARATIONS(Monster, Scope);
	public:Monster(int hunger) :
		Hunger(hunger)
	{

	}

		  bool Equals(const RTTI* rhs) const override
		  {
			  const Monster* rhsMonster = rhs->As<Monster>();
			  if (rhsMonster != nullptr)
			  {
				  bool result = Scope::operator==(*rhsMonster);
				  return result && (Hunger == rhsMonster->Hunger);
			  }
			  return false;
		  }

		  gsl::owner<Monster*> Clone() const override
		  {
			  return new Monster(*this);
		  }
		  int Hunger;
	};
	RTTI_DEFINITIONS(Monster);
}

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
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
			const Scope scope;
			Assert::AreEqual(size_t(0), scope.Size());
			Assert::IsNull(scope.Parent());
		}

		TEST_METHOD(Clone)
		{
			Monster a(1);
			Monster* b = new Monster(2);
			a.Adopt(*b, "Child Monsters");
			Monster c(a);
			Assert::IsTrue(a.Equals(&c));
		}

		TEST_METHOD(CopySemantics)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";
			
			Scope scope; 
			Datum& aDatum = scope.Append(a);
			aDatum = "Hello World!"s;
			Datum& bDatum = scope.Append(b);
			bDatum = mat4(5);
			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(size_t(3), scope.Size());

			Datum* cDatum = scope.FindContainedScope(cScope).second;
			Assert::IsNotNull(cDatum);

			Scope copiedScope = scope;
			Assert::AreEqual(size_t(3), copiedScope.Size());
			Datum* foundDatum = copiedScope.Find(a);
			Assert::IsNotNull(foundDatum);
			Assert::IsTrue(&aDatum != foundDatum);
			Assert::IsTrue(aDatum.Type() == foundDatum->Type());
			Assert::AreNotSame(aDatum.GetString(), foundDatum->GetString());
			Assert::AreEqual(aDatum.GetString(), foundDatum->GetString());

			foundDatum = copiedScope.Find(b);
			Assert::IsNotNull(foundDatum);
			Assert::IsTrue(bDatum.Type() == foundDatum->Type());
			Assert::IsTrue(bDatum.GetMatrix() == foundDatum->GetMatrix());

			foundDatum = copiedScope.Find(c);
			Assert::IsNotNull(foundDatum);
			Assert::IsTrue(cDatum->Type() == foundDatum->Type());

			Scope copyAssignmentScope;
			copyAssignmentScope = scope;
			Assert::AreEqual(size_t(3), copyAssignmentScope.Size());
			Datum* assignmentFoundDatum = copyAssignmentScope.Find(a);
			Assert::IsNotNull(assignmentFoundDatum);
			Assert::IsTrue(&aDatum != assignmentFoundDatum);
			Assert::IsTrue(aDatum.Type() == assignmentFoundDatum->Type());
			Assert::AreNotSame(aDatum.GetString(), assignmentFoundDatum->GetString());
			Assert::AreEqual(aDatum.GetString(), assignmentFoundDatum->GetString());
		}

		TEST_METHOD(MoveSemantics)
		{
			const std::string a = "A";
			const std::string b = "B";
			const std::string c = "C";
			const std::string d = "D";

			{
				Scope scope;
				Datum& aDatum = scope.Append(a);
				aDatum = "Hello World!"s; 
				Datum& bDatum = scope.Append(b);
				bDatum = mat4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(size_t(3), scope.Size());
				Assert::AreEqual(size_t(0), cScope.Size());

				auto [datumIndex, cDatum] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(cDatum);

				Scope movedScope = std::move(scope);
	#pragma warning(push)
	#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), scope.Size());
	#pragma warning(pop)
				Assert::AreEqual(size_t(3), movedScope.Size());
				Datum* foundDatum = movedScope.Find(a);
				foundDatum = movedScope.Find(b);
				foundDatum = movedScope.Find(c);
				Assert::IsTrue(cDatum == foundDatum);
				Scope reparentedScope{ std::move(cScope) };
				Assert::IsNull(reparentedScope.Parent());
			}
			{
				Scope scope;
				Datum& aDatum = scope.Append(a);
				aDatum = "Hello World!"s;
				Datum& bDatum = scope.Append(b);
				bDatum = mat4(5);
				Scope& cScope = scope.AppendScope(c);
				Assert::AreEqual(size_t(3), scope.Size());

				Assert::AreEqual(size_t(0), cScope.Size());
				Scope movedAssignmentScope;

				auto [datumIndex, cDatum] = scope.FindContainedScope(cScope);
				Assert::IsNotNull(cDatum);
				movedAssignmentScope = std::move(scope);

	#pragma warning(push)
	#pragma warning(disable:26800)
				Assert::AreEqual(size_t(0), scope.Size());
	#pragma warning(pop)
				Assert::AreEqual(size_t(3), movedAssignmentScope.Size());
				Datum* foundAssignmentDatum = movedAssignmentScope.Find(a);
				foundAssignmentDatum = movedAssignmentScope.Find(b);				
				foundAssignmentDatum = movedAssignmentScope.Find(c);
				Assert::IsTrue(cDatum == foundAssignmentDatum);
				Scope reparentedScope;
				reparentedScope = std::move(cScope);
				Assert::IsNull(reparentedScope.Parent());
			}

		}

		TEST_METHOD(Equality)
		{
			Scope scope; 
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s;

			Scope& aReturnedScope = scope.AppendScope(a);
			
			Scope aScope;
			Scope& a2ReturnedScope = aScope.AppendScope(a);
			Assert::IsTrue(aReturnedScope == a2ReturnedScope);
			Assert::IsTrue(scope == aScope);

			scope.Append(c);
			Scope cScope;
			cScope.Append(c);
			cScope.AppendScope(b);
			Assert::IsFalse(scope == cScope);

			Scope bScope;
			bScope.AppendScope(a);
			bScope.AppendScope(b);
			Assert::IsTrue(bScope != aScope);
			Assert::IsFalse(bScope == aScope);
		}

		TEST_METHOD(Clear)
		{

		}

		TEST_METHOD(Append)
		{
			Scope scope;
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s;

			Datum& aDatum = scope.Append(a);
			Assert::AreEqual(size_t(1), scope.Size());
			Assert::IsTrue(&aDatum == scope.Find(a));
			Assert::IsTrue(aDatum == scope[0]);
			Assert::IsTrue(aDatum == scope[0]);

			Datum& bDatum = scope.Append(b);
			Assert::AreEqual(size_t(2), scope.Size());
			Assert::IsTrue(&bDatum == scope.Find(b));
			Assert::IsTrue(bDatum == scope[1]);
			Assert::IsTrue(bDatum == scope[b]);

			Datum& cDatum = scope.Append(c);
			Assert::AreEqual(size_t(3), scope.Size());
			Assert::IsTrue(&cDatum == scope.Find(c));
			Assert::IsTrue(cDatum == scope[2]);
			Assert::IsTrue(cDatum == scope[c]);
			Assert::ExpectException<invalid_argument>([&scope] {scope.Append(""); });
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;
			const string a = "A"s;
			const string b = "B"s;
			const string c = "C"s;

			Scope& aScope = scope.AppendScope(a);
			Assert::AreEqual(size_t(1), scope.Size());

			Datum* aScopeDatum = scope.Find(a);
			Assert::IsNotNull(aScopeDatum);
			Assert::IsTrue(aScope == aScopeDatum->GetScope());

			Scope& bScope = scope.AppendScope(b);
			Assert::AreEqual(size_t(2), scope.Size());
			Assert::IsTrue(&scope == bScope.Parent());

			Datum* bScopeDatum = scope.Find(b);
			Assert::IsNotNull(bScopeDatum);
			Assert::IsTrue(DatumTypes::Table == bScopeDatum->Type());
			Assert::IsTrue(bScope == bScopeDatum->GetScope());
			Assert::IsTrue(bScopeDatum == &scope[1]);


			Scope& cScope = scope.AppendScope(c);
			Assert::AreEqual(size_t(3), scope.Size());
			Assert::IsTrue(&scope == cScope.Parent());

			Datum* cScopeDatum = scope.Find(c);
			Assert::IsNotNull(cScopeDatum);
			Assert::IsTrue(DatumTypes::Table == cScopeDatum->Type());
			Assert::IsTrue(cScope == cScopeDatum->GetScope());
			Assert::IsTrue(cScopeDatum == &scope[2]);

			Scope& anotherCScope = scope.AppendScope(c);
			Assert::AreEqual(size_t(3), scope.Size());
			Assert::IsTrue(&scope == anotherCScope.Parent());
			cScopeDatum = scope.Find(c);
			Assert::IsNotNull(cScopeDatum);
			Assert::IsTrue(DatumTypes::Table == cScopeDatum->Type());
			Assert::IsTrue(anotherCScope == cScopeDatum->GetScope(1));
			Assert::ExpectException<invalid_argument>([&scope] { scope.AppendScope(""); });
		}
		
		TEST_METHOD(Orphan)
		{
			Scope scope;
			Scope& aScope = scope.AppendScope("A"s);
			Assert::IsTrue(&scope == aScope.Parent());
			auto& ownScope = *aScope.Orphan();
			Assert::IsNull(aScope.Parent());
			Assert::AreEqual(&aScope, &ownScope);
			delete &ownScope;
		}

		TEST_METHOD(Adopt)
		{
			Scope* child = new Scope;
			Assert::IsNull(child->Parent());

			Scope scope; 
			scope.Adopt(*child, "A"s);
			Assert::IsTrue(&scope == child->Parent());
			
			// Empty name test
			Assert::ExpectException<exception>([&scope, child] {scope.Adopt(*child, ""s); });

			// Existing non-table key
			//scope.Append("NonTableDatum") = 10;
			//Assert::ExpectException<exception>([&scope] { Scope s; scope.Adopt(s, "NonTableDatum"s); });

			// Self-adoption
			Assert::ExpectException<exception>([&scope] { Scope s; scope.Adopt(scope, "A"s); });

			// Ancestor-adoption
			Assert::ExpectException<exception>([&scope, &child] { child->Adopt(scope, "A"s); });
		}

		TEST_METHOD(Search)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;
			string d = "D"s;
			string e = "E"s;

			scope.Append(a);
			Assert::AreEqual(size_t(1), scope.Size());
			Scope& aScope = scope.AppendScope(b);
			scope.AppendScope(c);
			Scope& dScope = aScope.AppendScope(d);

			Datum* foundAScope = scope.Search(a);
			Assert::IsNotNull(foundAScope);
			Assert::IsNull(scope.Search(e));
			

			Assert::IsNull(dScope.Search(e));
			Assert::IsNotNull(dScope.Search(b));
			Assert::IsTrue(aScope == dScope.Search(b)->GetScope());

			Scope* foundScope;
			Datum* foundDatumA = scope.Search(a, foundScope);
			Assert::IsNotNull(foundDatumA);

			Assert::ExpectException<exception>([&scope] { scope[2000]; });
		}

		TEST_METHOD(FindContainedScope)
		{
			Scope scope;
			const string a = "A"s;
			Scope bScope;
			scope.AppendScope(a);
			Assert::AreEqual(size_t(1), scope.Size());
			auto [index, datum] = scope.FindContainedScope(bScope);
			Assert::IsNull(datum);
		}

		TEST_METHOD(Ancestry)
		{
			Scope scope;
			string a = "A"s;
			string b = "B"s;
			string c = "C"s;
			string d = "D"s;
			string e = "E"s;

			scope.Append(a);
			Assert::AreEqual(size_t(1), scope.Size());
			Scope& aScope = scope.AppendScope(b);
			scope.AppendScope(c);
			Scope& dScope = aScope.AppendScope(d);

			Assert::IsTrue(dScope.IsDescendantOf(scope));
			Assert::IsTrue(scope.IsAncestorOf(dScope));
			Assert::IsTrue(aScope.IsDescendantOf(scope));
			Assert::IsTrue(aScope.IsAncestorOf(dScope));
		}

		TEST_METHOD(RTTITest)
		{
			Scope scope;
			const RTTI* rtti = &scope;
			Assert::IsFalse(rtti->Is(Foo::TypeIdClass()));
			Assert::IsTrue(rtti->Is(Scope::TypeIdClass()));
			Assert::IsTrue(rtti->Is(rtti->TypeIdInstance()));
			Assert::AreEqual(Scope::TypeIdClass(), rtti->TypeIdInstance());

			const Foo* b = rtti->As<Foo>();
			Assert::IsNull(b);

			const Scope* s = rtti->As<Scope>();
			Assert::IsNotNull(s);
			Assert::IsTrue(&scope == s);
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}