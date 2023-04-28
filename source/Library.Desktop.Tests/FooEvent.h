#pragma once
#include "Event.h"

namespace UnitTests
{
	class FooEvent
	{
	public:
		void Update();

		size_t integer = 0;
	};
}