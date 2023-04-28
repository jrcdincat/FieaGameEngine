#include "DefaultHash.h"

namespace FieaGameEngine
{
	template <typename T>
	inline size_t DefaultHash<T>::operator()(const T& key)
	{
		const size_t hashPrime = 31;
		size_t hash = 0;
		const std::byte* data = reinterpret_cast<const std::byte*>(&key);


		for (size_t index = 0; index < sizeof(T); ++index)
		{
			hash += static_cast<size_t>(data[index]) * hashPrime;
		}

		return hash;
	}

	inline size_t DefaultHash<char*>::operator()(const char* key)
	{
		return Hash(key, strlen(key));
	}

	inline size_t DefaultHash<const char*>::operator()(const char* key)
	{
		return Hash(key, strlen(key));
	}

	inline size_t DefaultHash<std::string>::operator()(const std::string& key)
	{
		return Hash(key, key.length());
	}

	inline size_t DefaultHash<const std::string>::operator()(const std::string& key)
	{
		return Hash(key, key.length());
	}

	template<typename T>
	size_t Hash(const T& key, size_t length)
	{
		const size_t hashPrime = 31;
		size_t hash = 0;

		for (size_t index = 0; index < length; index++)
		{
			hash += key[index] * hashPrime;
		}

		return hash;
	}
}
