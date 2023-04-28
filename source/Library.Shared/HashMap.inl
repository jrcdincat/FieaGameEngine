#include "HashMap.h"

namespace FieaGameEngine
{
#pragma region Iterator
	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator::Iterator(Vector<ChainType>& container, ChainType& chainContainer, ChainType::Iterator chainIterator, size_t chainIndex) :
		_container{ &container }, _chainContainer{ &chainContainer }, _chainIterator{ chainIterator }, _chainIndex{ chainIndex }
	{
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::Iterator::operator*() const
	{
		assert(_container != nullptr);
		return *(_chainIterator);
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::Iterator::operator->() const
	{
		assert(_container != nullptr);
		return &(*(_chainIterator));
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator!=(const Iterator& rhs) const
	{
		return (_container != rhs._container || _chainIndex != rhs._chainIndex || _chainIterator != rhs._chainIterator);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator& HashMap<TKey, TData>::Iterator::operator++()
	{
		auto value = _chainIterator;
		++_chainIterator;
		size_t index = _chainIndex;
		for (; index <= _container->size(); index++)
		{
			_chainIndex = index - 1;
			while (_chainIterator != _chainContainer->end())
			{
				if (_chainIterator != value)
				{
					return *this;
				}
				++_chainIterator;
			}

			if (_chainIterator == _chainContainer->end())
			{
				if (index == _container->size())
					break;

				_chainContainer = &(_container->At(index));
				_chainIterator = _chainContainer->begin();
			}
		}

		_chainIndex = _container->size() - 1;
		_chainIterator = _chainContainer->end();
		return *this;
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Iterator::operator++(int)
	{
		auto value = _chainIterator;
		++_chainIterator;
		size_t index = _chainIndex;
		for (; index <= _container->size(); index++)
		{
			_chainIndex = index - 1;
			while (_chainIterator != _chainContainer->end())
			{
				if (_chainIterator != value)
				{
					return *this;
				}
				++_chainIterator;
			}

			if (_chainIterator == _chainContainer->end())
			{
				if (index == _container->size())
					break;

				_chainContainer = &(_container->At(index));
				_chainIterator = _chainContainer->begin();
			}
		}

		_chainIndex = _container->size() - 1;
		_chainIterator = _chainContainer->end();
		return *this;
	}

#pragma endregion

#pragma region ConstIterator
	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator::ConstIterator(const Vector<ChainType>& container, const ChainType& chainContainer, ChainType::ConstIterator chainIterator, size_t chainIndex) :
		_container{ &container }, _chainContainer{ &chainContainer }, _chainIterator{ chainIterator }, _chainIndex{ chainIndex }
	{
	}

	template <typename TKey, typename TData>
	inline const HashMap<TKey, TData>::PairType& HashMap<TKey, TData>::ConstIterator::operator*() const
	{
		assert(_container != nullptr);
		return *_chainIterator;
	}

	template <typename TKey, typename TData>
	inline const HashMap<TKey, TData>::PairType* HashMap<TKey, TData>::ConstIterator::operator->() const
	{
		assert(_container != nullptr);
		return &(*_chainIterator);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return (_container != rhs._container || _chainIndex != rhs._chainIndex|| _chainIterator != rhs._chainIterator);
	}

	template <typename TKey, typename TData>
	inline bool HashMap<TKey, TData>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator& HashMap<TKey, TData>::ConstIterator::operator++() 	
	{
		auto value = _chainIterator;
		++_chainIterator;
		size_t index = _chainIndex;
		for (; index <= _container->size(); index++)
		{
			_chainIndex = index - 1;
			while (_chainIterator != _chainContainer->end())
			{
				if (_chainIterator != value)
				{
					return *this;
				}
				++_chainIterator;
			}

			if (_chainIterator == _chainContainer->end())
			{
				if (index == _container->size())
					break;

				_chainContainer = &(_container->At(index));
				_chainIterator = _chainContainer->begin();
			}
		}

		_chainIndex = _container->size() - 1;
		_chainIterator = _chainContainer->end();
		return *this;
	}

	template <typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::ConstIterator::operator++(int) 
	{
		auto value = _chainIterator;
		++_chainIterator;
		size_t index = _chainIndex;
		for (; index <= _container->size(); index++)
		{
			_chainIndex = index - 1;
			while (_chainIterator != _chainContainer->end())
			{
				if (_chainIterator != value)
				{
					return *this;
				}
				++_chainIterator;
			}

			if (_chainIterator == _chainContainer->end())
			{
				if (index == _container->size())
					break;

				_chainContainer = &(_container->At(index));
				_chainIterator = _chainContainer->begin();
			}
		}

		_chainIndex = _container->size() - 1;
		_chainIterator = _chainContainer->end();
		return *this;
	}

#pragma endregion

#pragma region HashMap
	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(size_type chainSize)
	{
		assert(chainSize > 1);

		_hashMap.Reserve(chainSize);
		for (size_t i = 0; i < chainSize; ++i)
		{
			_hashMap.push_back(ChainType{});
		}
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::HashMap(std::initializer_list<PairType> list)
	{
		_hashMap.Reserve(list.size());

		for (size_t i = 0; i < list.size(); ++i)
		{
			_hashMap.push_back(ChainType{});
		}

		for (const PairType& value : list)
		{
			Insert(std::make_pair(value.first, value.second));
		}
	}

	template<typename TKey, typename TData>
	template<typename HashFunctor>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::Find(const TKey& key, size_t hash, HashFunctor hashFunctor)
	{
		if (hash == _CRT_SIZE_MAX)
		{
			hash = hashFunctor(key);
			hash = hash % _hashMap.Capacity();
		}
		else if (_hashMap[hash].Size() == 0)
		{
			return end();
		}
			
		auto& iterator = _hashMap[hash].Find(std::make_pair(key, TData{}));
		if (iterator == _hashMap[hash].end())
		{
			return end();;
		}
		return Iterator{ _hashMap, _hashMap[hash], iterator, hash};
	}

	template<typename TKey, typename TData>
	template<typename HashFunctor>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::Find(const TKey& key, size_t hash, HashFunctor hashFunctor) const
	{
		if (hash == _CRT_SIZE_MAX)
		{
			hash = hashFunctor(key);
			hash = hash % _hashMap.Capacity();
		}

		auto& iterator = _hashMap[hash].Find(std::make_pair(key, TData{}));
		if (iterator == _hashMap[hash].end())
		{
			return cend();
		}
		return ConstIterator{ _hashMap, _hashMap[hash], iterator, hash };
	}

	template<typename TKey, typename TData>
	template<typename HashFunctor>
	inline std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(const PairType& pair, HashFunctor hashFunctor)
	{
		size_t hash = hashFunctor(pair.first);
		hash = hash % _hashMap.Capacity();

		auto iteratorIfFound = Find(pair.first, hash);
		if (iteratorIfFound != end())
		{
			return std::make_pair(iteratorIfFound, false);
		}

		auto iterator = _hashMap[hash].PushFront(pair);
		++_population;
		return std::make_pair(Iterator{_hashMap, _hashMap[hash],iterator, hash}, true);
	}

	template<typename TKey, typename TData>
	template<typename HashFunctor>
	inline std::pair<typename HashMap<TKey, TData>::Iterator, bool> HashMap<TKey, TData>::Insert(PairType&& pair, HashFunctor hashFunctor)
	{
		size_t hash = hashFunctor(pair.first);
		hash = hash % _hashMap.Capacity();

		auto iteratorIfFound = Find(pair.first, hash);
		if (iteratorIfFound != end())
		{
			return std::make_pair(iteratorIfFound, false);
		}

		auto iterator = _hashMap[hash].PushFront(std::move(pair));
		++_population;
		return std::make_pair(Iterator{ _hashMap, _hashMap[hash],iterator, hash }, true);
	}

	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::operator[](const TKey& key)
	{
		auto [position, isInserted] = (Insert(std::make_pair<const TKey&, TData>(key, TData{})));  // Structured binding syntax
		return position->second;
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Remove(const TKey& key)
	{
		auto iteratorToKey = Find(key);
		if (iteratorToKey != end())
		{
			_hashMap.At(iteratorToKey._chainIndex).Remove(iteratorToKey._chainIterator);
			--_population;
		}
	}

	template<typename TKey, typename TData>
	inline void HashMap<TKey, TData>::Clear()
	{
		for (auto& sList : _hashMap)
		{
			sList.Clear();
		}
		_population = 0;
	}

	template<typename TKey, typename TData>
	inline const HashMap<TKey, TData>::size_type HashMap<TKey, TData>::Size() const
	{
		return _population;
	}

	template<typename TKey, typename TData>
	inline const bool HashMap<TKey, TData>::ContainsKey(const TKey& key)const
	{
		return (Find(key) != end());
	}

	template<typename TKey, typename TData>
	inline TData& HashMap<TKey, TData>::At(const TKey& key)
	{
		return Find(key)->second; 
	}

	template<typename TKey, typename TData>
	inline const TData& HashMap<TKey, TData>::At(const TKey& key) const
	{
		return Find(key)->second;
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::begin()
	{
		for (size_t index = 0; index < _hashMap.Capacity(); ++index)
		{
			auto& chain = _hashMap.At(index);
			if (chain.Size() != 0)
			{
				return Iterator{ _hashMap, _hashMap[index], chain.begin(), index };
			}
		}
		return end();
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::Iterator HashMap<TKey, TData>::end()
	{
		return Iterator{_hashMap, _hashMap[_hashMap.Capacity() - 1], _hashMap[_hashMap.Capacity() - 1].end(), _hashMap.Capacity() - 1};
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::begin() const
	{
		for (size_t index = 0; index < _hashMap.Capacity(); ++index)
		{
			auto& chain = _hashMap.At(index);
			if (chain.Size() != 0)
			{
				return ConstIterator{ _hashMap, _hashMap[index], chain.begin(), index};
			}
		}
		return cend();
	}

	template<typename TKey, typename TData>
	inline HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::end() const
	{
		return ConstIterator{ _hashMap, _hashMap[_hashMap.Capacity() - 1], _hashMap[_hashMap.Capacity() - 1].end(), _hashMap.Capacity() - 1 };
	}

	template<typename TKey, typename TData>
	inline const HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cbegin() const
	{
		for (size_t index = 0; index < _hashMap.Capacity(); ++index)
		{
			auto& chain = _hashMap.At(index);
			if (chain.Size() != 0)
			{
				return ConstIterator{ _hashMap, _hashMap[index], chain.begin(), index};
			}
		}
		return cend();
	}

	template<typename TKey, typename TData>
	inline const HashMap<TKey, TData>::ConstIterator HashMap<TKey, TData>::cend() const
	{
		return ConstIterator{ _hashMap, _hashMap[_hashMap.Capacity() - 1], _hashMap[_hashMap.Capacity() - 1].end(), _hashMap.Capacity() - 1 };
	}
#pragma endregion
}