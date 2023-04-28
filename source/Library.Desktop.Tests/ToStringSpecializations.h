#pragma once
#include "CppUnitTest.h"
#include "Foo.h"
#include "Bar.h"
#include "HashMap.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace UnitTests;
using namespace std::string_literals;
using namespace FieaGameEngine;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<const UnitTests::Foo, int>>(const std::pair<const UnitTests::Foo , int>& t)
	{
		std::wstring s = ToString(t.first);
		RETURN_WIDE_STRING(s);
	}

	template<>
	inline std::wstring ToString<UnitTests::Bar>(const UnitTests::Bar& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Bar>(const UnitTests::Bar* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Bar>(UnitTests::Bar* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<std::pair<const UnitTests::Bar, int>>(const std::pair<const UnitTests::Bar, int>& t)
	{
		std::wstring s = ToString(t.first);
		RETURN_WIDE_STRING(s);
	}
}