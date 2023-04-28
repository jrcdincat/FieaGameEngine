#pragma once
#include <string>

namespace FieaGameEngine
{
	template<typename T> 
	struct DefaultEquality final
	{
		bool operator()(const T& lhs, const T& rhs) const;
	};

	/// <summary>
	/// Takes std:: pair and compares its keys. This implementation is bad, equality should only be TKey instead of std::pair.
	/// </summary>
	template<typename TKey, typename TData>
	struct DefaultEquality<std::pair<const TKey, TData>> final 
	{
		bool operator()(const std::pair<const TKey, TData> lhs, const std::pair<const TKey, TData> rhs) const
		{
			return lhs.first == rhs.first;
		}
	};
}
#include "DefaultEquality.inl"