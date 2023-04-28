#include "Factory.h"

namespace FieaGameEngine
{
	template <typename T>
	inline const Factory<T>* Factory<T>::Find(const std::string& name)
	{
		auto foundFactory = _factoryMap.Find(name);
		return foundFactory != _factoryMap.end() ? foundFactory->second.get() : nullptr;
	}

	template <typename T>
	inline gsl::owner<T*> Factory<T>::Create(const std::string& name)
	{
		auto foundFactory = Find(name); 
		return foundFactory != nullptr ? foundFactory->Create() : nullptr;
	}

	template <typename T>
	inline size_t Factory<T>::Size() 
	{ 
		return _factoryMap.Size();
	}

	template <typename T> 
	inline void Factory<T>::Add(std::unique_ptr<Factory>&& factoryToAdd) 
	{
		auto result = _factoryMap.Insert(std::make_pair(factoryToAdd->ClassName(), std::move(factoryToAdd)));
		assert(result.second == true);
	}

	template <typename T> 
	inline void Factory<T>::Clear() 
	{ 
		_factoryMap.Clear(); 
	}

	template <typename T> 
	inline void Factory<T>::Remove(const std::string& name) 
	{ 
		_factoryMap.Remove(name); 
	}
}