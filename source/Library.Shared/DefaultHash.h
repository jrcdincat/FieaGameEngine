#pragma once
#include <string>

namespace FieaGameEngine
{
	/// <summary>
	/// Additive hash functor
	/// </summary>
	template<typename T>
	struct DefaultHash final
	{
		size_t operator()(const T& key);
	};

	/// <summary>
	/// Additive hash functor
	/// </summary>
	template<>
	struct DefaultHash<char*> final
	{
		size_t operator()(const char* key);
	};

	/// <summary>
	/// Additive hash functor
	/// </summary>
	template<>
	struct DefaultHash<const char*> final
	{
		size_t operator()(const char* key);
	};

	/// <summary>
	/// Additive hash functor
	/// </summary>
	template<>
	struct DefaultHash<std::string> final
	{
		size_t operator()(const std::string& key);
	};

	/// <summary>
	/// Additive hash functor
	/// </summary>
	template<>
	struct DefaultHash<const std::string> final
	{
		size_t operator()(const std::string& key);
	};


	template <typename T>
	size_t Hash(const T&, size_t length);
}
#include "DefaultHash.inl"