#pragma once
#include <utility>
#include <cstddef>
#include <cassert>
#include "SList.h"
#include "Vector.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Hash Map
	/// </summary>
	template <typename TKey, typename TData>
	class HashMap final
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using ChainType = SList<PairType>;
		using size_type = std::size_t;

		/// <summary>
		/// Hash Map iterator
		/// </summary>
		class Iterator final
		{
			friend HashMap;
			friend class ConstIterator;
		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="other"> - Iterator to copy</param>
			Iterator(Iterator& other) = default;

			/// <summary>
			/// Default move constructor
			/// </summary>
			/// <param name="other"> - iterator to move</param>
			Iterator(Iterator&& other) = default;

			/// <summary>
			/// Default copy assignment operator
			/// </summary>
			/// <param name="rhs"> - Iterator to copy</param>
			/// <returns>HashMap::Iterator reference</returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// Default move assignment operator
			/// </summary>
			/// <param name="rhs"> - Iterator to move</param>
			/// <returns>HashMap::Iterator reference</returns>
			Iterator& operator=(Iterator&& rhs) = default;

			/// <summary>
			/// Default destructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Reference to key value pair</returns>
			PairType& operator*() const;

			/// <summary>
			/// Arrow Dereference operator.
			/// </summary>
			/// <returns>Pointer to key value pair</returns>
			PairType* operator->() const;

			/// <summary>
			/// Returns true when two iterators point to a different key value pair
			/// </summary>
			/// <param name="rhs"> - Iterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator !=(const Iterator& rhs) const;

			/// <summary>
			/// Returns true when two iterators point to the same key value pair
			/// </summary>
			/// <param name="rhs"> - Iterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator ==(const Iterator& rhs) const;

			/// <summary>
			/// Prefix operator for HashMap::Iterator
			/// </summary>
			/// <returns>HashMap::Iterator&</returns>
			Iterator& operator++();
			
			/// <summary>
			/// Postfix operator for HashMap::Iterator
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>HashMap::Iterator</returns>
			Iterator operator++(int);

		private:
			/// <summary>
			/// Iterator constructor
			/// </summary>
			/// <param name="container"> - Associated vector container</param>
			/// <param name="chainContainer"> - Slist chain that iterator is associated with</param>
			/// <param name="chainIterator"> - Slist iterator that has key value pair</param>
			/// <param name="chainIndex"> - Hash index iterator is associated with</param>
			Iterator(Vector<ChainType>& container, ChainType& chainContainer, ChainType::Iterator chainIterator, size_t chainIndex);

			/// <summary>
			/// Iterator to the SList chain that contains the key value pair 
			/// </summary>
			ChainType::Iterator _chainIterator;

			/// <summary>
			/// Hash index that iterator points to
			/// </summary>
			size_t _chainIndex { 0 };

			/// <summary>
			/// Pointer to container iterator is associated with.
			/// </summary>
			Vector<ChainType>* _container{ nullptr };
			
			/// <summary>
			/// SList of key value pairs
			/// </summary>
			ChainType* _chainContainer{ nullptr };
		};

		/// <summary>
		/// Hash Map constiterator
		/// </summary>
		class ConstIterator final
		{
			friend HashMap;
		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Default copy constructor
			/// </summary>
			/// <param name="other"> - ConstIterator to copy</param>
			ConstIterator(ConstIterator& other) = default;

			/// <summary>
			/// Default move constructor
			/// </summary>
			/// <param name="other"> - ConstIterator to move</param>
			ConstIterator(ConstIterator&& other) = default;

			/// <summary>
			/// Default copy assignment operator
			/// </summary>
			/// <param name="rhs"> - Constiterator to copy</param>
			/// <returns>HashMap::ConstIterator reference</returns>
			ConstIterator& operator=(const ConstIterator& rhs) = default;

			/// <summary>
			/// Default move assignment operator
			/// </summary>
			/// <param name="rhs"> - ConstIterator to move</param>
			/// <returns>HashMap::ConstIterator reference</returns>
			ConstIterator& operator=(ConstIterator&& rhs) = default;

			/// <summary>
			/// Default destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>Reference to key value pair</returns>
			const PairType& operator*() const;

			/// <summary>
			/// Arrow Dereference operator.
			/// </summary>
			/// <returns>Pointer to key value pair</returns>
			const PairType* operator->() const;

			/// <summary>
			/// Returns true when two iterators point to a different key value pair
			/// </summary>
			/// <param name="rhs"> - ConstIterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator !=(const ConstIterator& rhs) const;

			/// <summary>
			/// Returns true when two iterators point to the same key value pair
			/// </summary>
			/// <param name="rhs"> - ConstIterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator ==(const ConstIterator& rhs) const;

			/// <summary>
			/// Prefix operator for HashMap::ConstIterator
			/// </summary>
			/// <returns>HashMap::ConstIterator&</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix operator for HashMap::ConstIterator
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>HashMap::ConstIterator</returns>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Const Iterator constructor
			/// </summary>
			/// <param name="container"> - Associated vector container</param>
			/// <param name="chainContainer"> - Slist chain that iterator is associated with</param>
			/// <param name="chainIterator"> - Slist iterator that has key value pair</param>
			/// <param name="chainIndex"> - Hash index iterator is associated with</param>
			ConstIterator(const Vector<ChainType>& container, const ChainType& chainContainer, ChainType::ConstIterator chainIterator, size_t chainIndex);

			/// <summary>
			/// Iterator to the SList chain that contains the key value pair 
			/// </summary>
			ChainType::ConstIterator _chainIterator = nullptr;

			/// <summary>
			/// Hash index that iterator points to
			/// </summary>
			size_t _chainIndex{ 0 };

			/// <summary>
			/// Pointer to container iterator is associated with.
			/// </summary>
			const Vector<ChainType>* _container{ nullptr };

			/// <summary>
			/// SList of key value pairs
			/// </summary>
			const ChainType* _chainContainer{ nullptr };
		};

		
		/// <summary>
		/// HashMap constructor that will default to a chainsize of 11 if a size is not provided.
		/// </summary>
		explicit HashMap(size_type chainSize = 11); 

		/// <summary>
		/// Initializer list constructor.
		/// </summary>
		/// <param name="list"> - std::initializer list of generic type</param>
		HashMap(std::initializer_list<PairType> list);

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="other"> - Hash map to copy</param>
		HashMap(const HashMap& other) = default;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other"> - Hash map to move</param>
		HashMap(HashMap&& other) = default;

		/// <summary>
		/// Default copy assignment operator
		/// </summary>
		/// <param name="rhs"> - Hash map to copy</param>
		/// <returns>Reference to hash map</returns>
		HashMap& operator=(const HashMap& rhs) = default;

		/// <summary>
		/// Default move assignment operator
		/// </summary>
		/// <param name="rhs"> - Hash map to move</param>
		/// <returns>Reference to hash map</returns>
		HashMap& operator= (HashMap&& rhs) = default;

		/// <summary>
		/// Default destructor
		/// </summary>
		~HashMap() = default;

		/// <summary>
		/// Returns an Iterator pointing to data stored at hopefully existing key.
		/// Key is hashed for lookup if a precalculated hash is not provided.
		/// </summary>
		/// <param name="key"> - Key to lookup</param>
		/// <param name="hash"> - Optional precalculated hash value</param>
		/// <param name="hashFunctor"> - Optional custom hash functor</param>
		/// <returns>HashMap::Iterator</returns>
		template <typename HashFunctor = DefaultHash<TKey>>
		[[nodiscard]] Iterator Find(const TKey& key, size_t hash = _CRT_SIZE_MAX, HashFunctor hashFunctor = HashFunctor{});

		/// <summary>
		/// Returns a ConstIterator pointing to data stored at hopefully exisiting key.
		/// Key is hashed for lookup if a precalculated hash is not provided
		/// </summary>
		/// <param name="key"> - Key to lookup</param>
		/// <param name="hash"> - Optional precalculated hash value></param>
		/// <param name="hashFunctor"> - Optional custom hash functor</param>
		/// <returns>HashMap::ConstIterator</returns>
		template <typename HashFunctor = DefaultHash<TKey>>
		[[nodiscard]] ConstIterator Find(const TKey& key, size_t hash = _CRT_SIZE_MAX, HashFunctor hashFunctor = HashFunctor{}) const;

		/// <summary>
		/// Returns an iterator to either the data that was just inserted, or the data that was already stored at pre-existing key. 
		/// If insert was successful, the bool in the return pair will be true, otherwise false.
		/// </summary>
		/// <param name="pair">Key value pair to try and insert</param>
		/// <param name="hashFunctor"> - Optional parameter to provide custom hash functor</param>
		/// <returns> std::pair Iterator, bool</returns>
		template <typename HashFunctor = DefaultHash<TKey>>
		std::pair<Iterator, bool> Insert(const PairType& pair, HashFunctor hashFunctor = HashFunctor{});
		
		/// <summary>
		/// Returns an iterator to either the data that was just inserted, or the data that was already stored at pre-existing key. 
		/// If insert was successful, the bool in the return pair will be true, otherwise false.
		/// </summary>
		/// <param name="pair">Key value pair to try and insert</param>
		/// <param name="hashFunctor"> - Optional parameter to provide custom hash functor</param>
		/// <returns> std::pair Iterator, bool</returns>
		template <typename HashFunctor = DefaultHash<TKey>>
		std::pair<Iterator, bool> Insert(PairType&& pair, HashFunctor hashFunctor = HashFunctor{});


		/// <summary>
		/// Finds key in hash map. If used with assignment operator and key doesn't already exist, it will insert the data.
		/// </summary>
		/// <param name="key"> - Key to lookup</param>
		/// <returns>Reference to data</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Remove data stored in hash map with given key.
		/// </summary>
		/// <param name="key"> - Key to lookup</param>
		void Remove(const TKey& key);

		/// <summary>
		/// Deletes all key value pairs in hash map.
		/// </summary>
		void Clear();

		/// <summary>
		/// Returns the population of hash map. This value should not be used to determine whether hash map resizing is required.
		/// Load factor should be used to determine whether to resize.
		/// </summary>
		/// <returns>Population</returns>
		[[nodiscard]] const size_type Size() const;

		/// <summary>
		/// Determines whether the given key exists inside hash map
		/// </summary>
		/// <param name="key"> - Key to lookup</param>
		/// <returns>True or False</returns>
		[[nodiscard]] const bool ContainsKey (const TKey& key) const;

		/// <summary>
		/// Returns a reference to the data stored at the given key.
		/// </summary>
		/// <param name="key"> - Key to lookup in hash map</param>
		/// <returns>Reference to data stored at key location</returns>
		TData& At(const TKey& key);

		/// <summary>
		/// Returns a reference to the data stored at the given key. 
		/// </summary>
		/// <param name="key"> - Key to lookup in hash map</param>
		/// <returns>Constant reference to data stored at key lcoation</returns>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Returns a HashMap::Iterator pointing to the first value stored in the hash map.
		/// </summary>
		/// <returns>HashMap::Iterator</returns>
		[[nodiscard]] Iterator begin();

		/// <summary>
		/// Returns a HashMap::Iterator pointing to the end of vector.
		/// </summary>
		/// <returns>HashMap::Iterator</returns>
		[[nodiscard]] Iterator end();

		/// <summary>
		/// Returns a HashMap::ConstIterator pointing to the first value stored in the hash map.
		/// </summary>
		/// <returns>HashMap::ConstIterator</returns>
		[[nodiscard]] ConstIterator begin() const;

		/// <summary>
		/// Returns a HashMap::ConstIterator pointing to the end of vector.
		/// </summary>
		/// <returns>HashMap::ConstIterator</returns>
		[[nodiscard]] ConstIterator end() const;

		/// <summary>
		/// Returns a HashMap::ConstIterator pointing to the first value stored in the hash map.
		/// </summary>
		/// <returns>HashMap::ConstIterator</returns>
		[[nodiscard]] const ConstIterator cbegin() const;

		/// <summary>
		/// Returns a HashMap::ConstIterator pointing to the end of vector.
		/// </summary>
		/// <returns>HashMap::ConstIterator</returns>
		[[nodiscard]] const ConstIterator cend() const;


	private:
		/// <summary>
		/// Hash Map is made with a vector of SList
		/// </summary>
		Vector<ChainType> _hashMap;

		/// <summary>
		/// Population is just the number of data inserted. It should not be used to determine whether the hashmap requires resizing.
		/// </summary>
		size_type _population{ 0 };
	};
}

#include "HashMap.inl"
