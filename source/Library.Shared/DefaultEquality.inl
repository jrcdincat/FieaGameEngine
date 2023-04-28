#include "DefaultEquality.h"
#pragma once

namespace FieaGameEngine
{
	template<typename T>
	inline bool DefaultEquality<T>::operator()(const T& lhs, const T& rhs) const
	{
		return lhs == rhs;
	}
}