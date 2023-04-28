#pragma once
#include "Foo.h"
#include "DefaultHash.h"
using namespace UnitTests;

namespace FieaGameEngine // TODO, make FOO work with hash map, relocate to H file to be shared here and other test file.
{
	template <>
	struct DefaultHash<Foo> final
	{
		size_t operator()(const Foo& key)
		{
			return key.Data();
		}
	};
}