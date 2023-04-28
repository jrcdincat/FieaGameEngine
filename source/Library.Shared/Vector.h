#pragma once

#include <cstddef>
#include <cassert>
#include <initializer_list>
#include "DefaultEquality.h"
#include <stdexcept>

// 1/31 class
// Store std::pair of Slists 
// constant key and potentially a non constant value
// const Tkey - type of the key
// const or not const TData - type of associated data
// Insertion, first take Tkey and hash it
// Test with a Foo as a key. 
// Create a unit test class specific to our hash functor
// Operator [] does an insert if there is nothing there, if there is something there then instead, it returns the value that is already stored there.
// If hashMap[a] and a is not a key currently in hashmap, then it will default construct (so make sure there is a default constructor) 
// probing or quadratic probing to resolve collisions
// At 70% load, can rehash (resize function - HIGHLY recommended to have, but optional) this can help reduce collisions
// 4-5 lines of code to write a resize, resizing/rehasing requires not just reallocating/reserving, but all the keys have to be reinserted because they will be at different locations than before since
// the container sizing changed. so they must be rehashed to make them valid for the newly sized hash map.
// (create type aliases) - the 'using' statements to do this. 
// Vector of std::pairs of Slists of nodes of T datas
// Vector should be fixed in size, unless user calls Resize 
// Shouldn't auto resize, very expensive call
// Go back and make a fixed size RESIZE method in VECTOR
// Hashmap doesn't need a capacity member
// Every chain (placement new for all Slist) at vector hashmap creation
// Hashmap should have a variable (load factor - number of population divided by number of chains in hashmap)
// Hashmap population count that counts each time you insert an element into hashmap (you can have a load factor greater than 1 even if you have some Slist that are empty. Thus, could be slightly inaccurate because of collisions.
// This is because the collisions cause a clunky distribution.
// Alternatively, can just do linear lookup through each element to determine what population is to get accurate number, which is fine for smaller hash maps. 
// Alternativey, keep track of number of populated chains, where they actually were moved. 
// Don't use Std;:hash, write your own hash functor.DefaultHash<TKey>, hash function template. Unless a user supplies a hash function, ours should be the default hash function to be used
// Hash function just has operator()() - not marked const, return size_T, pass in a reference to a TKey that is constant.
// Default hash class MUST be Independently tested, USE the TEST CODE FROM 1/30 class. Test with std::string, Foo, char* (IF THIS IS NOT TESTED INDEPENDENTLY _ WILL LOSE POINTS)
// pass constant reference of TKey because it may be expensive to copy it, so thus we don't want to copy it
// memcpy
// memcmp (mem compare that compares two blocks of memory 
// For hashing, we want to use the bytes (do not dereference, memcpy/memcmp)
// use a uint8_t to get the bytes value
// can hash by traversing each byte
// 
// const auto hashPrime = 32;
// size_t hash = 0;
// const std::byte* data = reinterpret_cast<const std::byte*>(&c); ----- std::byte is a uint8_t
// for (std::size_t index = 0; index < sizeof(TKey); ++index
// {
//		hash += static_cast<size_t>(data[index]) * hashPrime; // we add the bytes to get a number. This is an example of a additive hash 
// }
// This currently does not work with FOO, because FOOs refers to pointers and not to the specific data we want
// Wrtie generic version of this first, then write specializations for each type that we can predict, example, foos, std::string, char* need them for ALL 3 OF THESE. 
// FOO specialization should be written inside our unit test project, the others should be in shared
// 


namespace FieaGameEngine
{
	/// <summary>
	/// Vector class that stores generic data
	/// </summary>
	template <typename T>
	class Vector final
	{
	public: 
		using value_type = T;
		using reference = value_type&;
		using size_type = std::size_t;
		using const_reference = const value_type&;
		using rvalue_reference = T&&;

		class Iterator final
		{
			friend Vector;
			friend class ConstIterator;
		public: 
			/// <summary>
			/// Default constructor.
			/// </summary>
			Iterator() = default;
			
			/// <summary>
			/// Iterator copy constructor.
			/// </summary>
			/// <param name="it"> - iterator to copy</param>
			Iterator(const Iterator& it) = default;

			/// <summary>
			/// Returns a reference to the data stored in iterator.
			/// </summary>
			/// <returns>Generic reference</returns>
			reference operator*() const;
			
			/// <summary>
			/// Returns true when two iterators point to different data.
			/// </summary>
			/// <param name="rhs"> - iterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator!=(const Iterator& rhs) const;
			
			/// <summary>
			/// Returns true when two iterators point to the same data.
			/// </summary>
			/// <param name="rhs"> - iterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator==(const Iterator& rhs) const;
			
			/// <summary>
			/// Prefix increment Vector::Iterator.
			/// </summary>
			/// <returns>Iterator reference</returns>			
			Iterator& operator++();
			
			/// <summary>
			/// PostFix increment Vector::Iterator.
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>Vector::Iterator</returns>
			Iterator operator++(int);
			
			/// <summary>
			/// Prefix decrement Vector::Iterator.
			/// </summary>
			/// <returns>Iterator reference</returns>
			Iterator& operator--();

			/// <summary>
			/// Postfix decrement Vector::Iterator.
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>Vector::Iterator</returns>
			Iterator operator--(int);

		private:

			/// <summary>
			/// Constructor for vector iterator.
			/// </summary>
			/// <param name="container"> - vector iterator should be associated with</param>
			/// <param name="data"> - data stored in iterator, defaults to nullptr</param>
			Iterator(const Vector& container, value_type* data = nullptr);

			/// <summary>
			/// Vector iterator is associated with.
			/// </summary>
			const Vector* _container{ nullptr };

			/// <summary>
			/// Data iterator points to.
			/// </summary>
			value_type* _data{ nullptr };
		};

		class ConstIterator final
		{
			friend Vector;
		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			ConstIterator() = default;
			
			/// <summary>
			/// Type-cast constructor from iterator to const iterator.
			/// </summary>
			/// <param name="it"> - Constant iterator reference</param>
			ConstIterator(const Iterator& it);

			/// <summary>
			/// Returns a constant reference to the data stored in iterator.
			/// </summary>
			/// <returns>Constant generic reference</returns>
			const_reference operator*() const;
			
			/// <summary>
			/// Returns true when two iterators point to different data.
			/// </summary>
			/// <param name="rhs"> - iterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator!=(const ConstIterator& rhs) const;
			
			/// <summary>
			/// Returns true when two iterators point to the same data.
			/// </summary>
			/// <param name="rhs"> - iterator to compare with</param>
			/// <returns>True or False</returns>
			bool operator==(const ConstIterator& rhs) const;
			
			/// <summary>
			/// Prefix increment Vector::ConstIterator.
			/// </summary>
			/// <returns>Constant iterator reference</returns>
			ConstIterator& operator++();
			
			/// <summary>
			/// PostFix increment Vector::ConstIterator.
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>Vector::ConstIterator</returns>
			ConstIterator operator++(int);
			
			/// <summary>
			/// Prefix decrement Vector::ConstIterator.
			/// </summary>
			/// <returns>Constant iterator reference</returns>
			ConstIterator& operator--();
			
			/// <summary>
			/// Postfix decrement Vector::ConstIterator.
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>Vector::ConstIterator</returns>
			ConstIterator operator--(int);
		
		private:
			/// <summary>
			/// Vector iterator is associated with.
			/// </summary>
			const Vector* _container{ nullptr };

			/// <summary>
			/// Data iterator points to.
			/// </summary>
			const value_type* _data{ nullptr };
		};

		// Default Constructor
		/// <summary>
		/// Vector default constructor. Default reserves 0.
		/// </summary>
		Vector(size_type capacity = 0);

		/// <summary>
		/// Initializer list constructor. Default reserves 11.
		/// </summary>
		/// <param name="list"> - std::initializer list of generic type</param>
		Vector(std::initializer_list<value_type> list);
		
		/// <summary>
		/// Vector copy constructor.
		/// </summary>
		/// <param name="other"> - Vector to copy</param>
		Vector(const Vector& other);

		/// <summary>
		/// Vector move constructor
		/// </summary>
		/// <param name="other"> - Vector to move</param>
		Vector(Vector&& other) noexcept;
		
		/// <summary>
		/// Vector copy assignment operator.
		/// </summary>
		/// <param name="rhs"> - Vector to copy</param>
		/// <returns>Vector reference</returns>
		Vector& operator=(const Vector& rhs);
		
		/// <summary>
		/// Vector move assignemnt operator.
		/// </summary>
		/// <param name="rhs"> - Vector to move</param>
		/// <returns>Vector reference</returns>
		Vector& operator=(Vector&& rhs) noexcept;
		
		/// <summary>
		/// Vector destructor.
		/// </summary>
		~Vector();

		/// <summary>
		/// Get the data at the specified position.
		/// </summary>
		/// <param name="position"> - index position in vector memory</param>
		/// <returns>Generic reference</returns>
		reference operator[](size_type position);

		/// <summary>
		/// Get the data at the specified position.
		/// </summary>
		/// <param name="position"> - index position in vector memory</param>
		/// <returns>Generic reference</returns>
		reference At(size_type position);

		/// <summary>
		/// Get the data at the specified position.
		/// </summary>
		/// <param name="position"> - index position in vector memory</param>
		/// <returns>Constant generic reference</returns>
		const_reference operator[](size_type position) const;
		
		/// <summary>
		/// Get the data at the specified position.
		/// </summary>
		/// <param name="position"> - index position in vector memory</param>
		/// <returns>Constant generic reference</returns>
		const_reference At(size_type position) const;

		/// <summary>
		/// Removes value at the back of the vector.
		/// </summary>
		void pop_back();

		/// <summary>
		/// Add a value to the back of vector.
		/// </summary>
		/// <param name="value"> - Const reference value</param>
		void push_back(const_reference value);
		
		/// <summary>
		/// Add a moved value to the back of vector.
		/// </summary>
		/// <param name="value"> - R value refererence</param>
		void push_back(rvalue_reference value);

		/// <summary>
		/// Returns the data stored in the front of vector.
		/// </summary>
		/// <returns>Generic reference of stored data</returns>
		[[nodiscard]] reference Front();

		/// <summary>
		/// Returns the data stored in the front of vector.
		/// </summary>
		/// <returns>Constant generic reference of stored data</returns>
		[[nodiscard]] const_reference Front() const;
		
		/// <summary>
		/// Returns the data stored in the back of vector.
		/// </summary>
		/// <returns>Generic reference of stored data</returns>
		[[nodiscard]] reference back();
		
		/// <summary>
		/// Returns the data stored in the back of vector.
		/// </summary>
		/// <returns>Constant generic reference of stored data</returns>
		[[nodiscard]] const_reference back() const;
	
		/// <summary>
		/// Returns the amount of data stored in vector.
		/// </summary>
		/// <returns>size of vector</returns>
		[[nodiscard]] size_type size() const;
		
		/// <summary>
		/// Returns the amount of vector data that memory has been allocated for.
		/// </summary>
		/// <returns>Capacity of vector</returns>
		[[nodiscard]] size_type Capacity() const;
		

		/// <summary>
		/// Expand vector memory to match the requested capacity.
		/// </summary>
		/// <param name="capacity"> - Desired vector capacity</param>
		void Reserve(size_type capacity);
		
		/// <summary>
		/// Removes given value from vector.
		/// </summary>
		/// <param name="value"> - Value to be removed</param>
		/// <returns>True or False</returns>
		bool Remove(const_reference value);
		
		/// <summary>
		/// Removes data stored in vector that the iterator points to.
		/// </summary>
		/// <param name="it"> - Iterator pointing to value to be removed</param>
		/// <returns>True or False</returns>
		bool Remove(Iterator it);

		/// <summary>
		/// Finds a value and returns a Vector::Iterator that may have the searched for data
		/// </summary>
		/// <param name="value"> - Generic reference type</param>
		/// <param name="equalityFunctor"> - Equality functor, by default uses provided Default Equality</param>
		/// <returns>Vector::Iterator</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		Iterator Find(const_reference value, EqualityFunctor equalityFunctor = EqualityFunctor{});
		
		 
		/// <summary>
		/// Finds a value and returns a Vector::ConstIterator that may have the searched for data.
		/// </summary>
		/// <param name="value"> - Generic reference type </param>
		/// <param name="equalityFunctor"> - Equality functor, by default uses provided DefaultEquality</param>
		/// <returns>Vector::ConstIterator</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>	
		ConstIterator Find(const_reference value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;
		
		/// <summary>
		/// Check if the vector size is 0
		/// </summary>
		/// <returns>True or False/returns>
		[[nodiscard]] bool IsEmpty() const;
		
		/// <summary>
		/// Destruct all data stored in vector, size is reset to 0, but capacity remains the same.
		/// </summary>
		void Clear();

		/// <summary>
		/// Free extra memory so that that the capacity and size of a vector are equal.
		/// </summary>
		void ShrinkToFit();

		/// <summary>
		/// Returns a Vector::Iterator pointing to the datat at the beginning of the allocated memory for vector.
		/// </summary>
		/// <returns>Vector::Iterator</returns>
		[[nodiscard]] Iterator begin();

		/// <summary>
		/// Returns a Vector::Iterator pointing to one past the end of the allcoated memory space for vector.
		/// </summary>
		/// <returns>Iterator::Vector</returns>
		[[nodiscard]] Iterator end();

		/// <summary>
		/// Returns a Vector::Iterator pointing to the data at the beginning of the allocated memory for vector.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] Iterator begin() const;
		
		/// <summary>
		/// Returns a Vector::Iterator poining to one past the end of the allocated memory space for vector.
		/// </summary>
		/// <returns>Vector::Iterator</returns>
		[[nodiscard]] Iterator end() const;

		/// <summary>
		/// Returns a Vector::ConstIterator pointing to the data at the beginning of the allocated memory for vector.
		/// </summary>
		/// <returns>Vector::ConstIterator</returns>
		[[nodiscard]] ConstIterator cbegin() const;

		/// <summary>
		/// Returns a Vector::ConstIterator pointing one past the end of the allocated memory space for vector.
		/// </summary>
		/// <returns>Vector::ConstIterator</returns>
		[[nodiscard]] ConstIterator cend() const;

	private:
		/// <summary>
		/// Individual stored in vector
		/// </summary>
		value_type* _data{ nullptr };

		/// <summary>
		/// Amount of allocated memory
		/// </summary>
		size_type _capacity{ 0 };

		/// <summary>
		/// Number of data within vector 
		/// </summary>
		size_type _size{ 0 };
	};
}

#include "Vector.inl"