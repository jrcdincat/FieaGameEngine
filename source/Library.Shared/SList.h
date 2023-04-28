#pragma once

#include <cstddef>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include "DefaultEquality.h"

namespace FieaGameEngine
{
	// Our version of remove is closer std::erase not std::remove, have a version for one element, another version for multiple/all elements
	// Remove should only have non const version since it requires to be mutable (Same with insert after) (pass in regular iterator not const)
	// Could return bool with whether or not it was succesful
	// both version of find, one for not const iterator return, and one for returning const iterator


	template <typename T>
	class SList final
	{
	public:
		using value_type = T;
		using reference = value_type&;
		using size_type = std::size_t;
		using const_reference = const value_type&;
		using rvalue_reference = value_type&&;

	private:
		struct Node final
		{
			/// <summary>
			/// Generic stored data within the node.
			/// </summary>
			value_type Data;
			/// <summary>
			/// Pointer to the next node in the list.
			/// </summary>
			Node* Next{ nullptr };

			/// <summary>
			/// Constructor for a node in the list.
			/// </summary>
			/// <param name="Data">Constant reference of a generic data type.</param>
			/// <param name="next">Pointer to the next node in the list, defaults to nullptr.</param>
			Node(const_reference Data, Node* Next = nullptr);
			
			/// <summary>
			/// Constructor for a node in the list.
			/// </summary>
			/// <param name="Data">Constant reference of a generic data type.</param>
			/// <param name="next">Pointer to the next node in the list, defaults to nullptr.</param>
			Node(rvalue_reference Data, Node* Next = nullptr);
		};

		/// <summary>
		/// Node at the front of the list.
		/// </summary>
		Node* _front{ nullptr };
		/// <summary>
		/// Node at the back of the list.
		/// </summary>
		Node* _back{ nullptr };
		/// <summary>
		/// size of the list.
		/// </summary>
		size_type _size{ 0 };

	public:
		class Iterator final
		{
			friend SList;
			friend class ConstIterator;
		 
		public: 
			/// <summary>
			/// Default constructor for Iterator class.
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Returns a reference to the item contained by the node the Iterator points to.
			/// </summary>
			/// <returns>Reference</returns>
			reference operator*() const;

			/// <summary>
			/// Returns true when two iterators point to different nodes in a list.
			/// </summary>
			/// <returns>True or False</returns>
			bool operator!=(const Iterator& rhs) const; // check container, and node. Check if containers are same. two default constructed iterators should be equal. all other instance not equal
			
			/// <summary>
			/// Returns true when two iterators point to the same nodes in a list.
			/// </summary>
			/// <returns>True or False</returns>
			bool operator==(const Iterator& rhs) const; // invert the != operator when making this

			/// <summary>
			/// Reassigns iterator to the next node in a list.
			/// </summary>
			/// <returns>Iterator reference</returns>
			Iterator& operator++();

			/// <summary>
			/// Reassigns iterator to the next node in a list. However, a copy of the iterator is made first before 
			/// reassigning iterator.
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>Iterator</returns>
			Iterator operator++(int);

		private: 
			/// <summary>
			/// Constructor for iterator in the list.
			/// </summary>
			/// <param name="container">- Constant SList reference</param>
			/// <param name="node"> - Pointer to a node, defaults to nullptr</param>
			Iterator(const SList& container, Node* node = nullptr);

			/// <summary>
			/// Constant SList* associated with Iterator.
			/// </summary>
			const SList* _container{ nullptr };

			/// <summary>
			/// Node* the iterator points to.
			/// </summary>
			Node* _node{ nullptr };
		};

		class ConstIterator final
		{
			friend SList;

		public:
			/// <summary>
			/// Default constructor for ConstIterator.
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Type-cast constructor from iterator to const iterator.
			/// </summary>
			/// <param name="other">Constant iterator reference</param>
			ConstIterator(const Iterator& it);

			/// <summary>
			/// Returns a reference to the item contained by the node the Iterator points to.
			/// </summary>
			/// <returns>Constant reference to a genric type</returns>
			const_reference operator*() const;

			/// <summary>
			/// Returns true when two iterators point to different nodes in a list.
			/// </summary>
			/// <returns>True or False</returns>
			bool operator!=(const ConstIterator& rhs) const;

			/// <summary>
			/// Returns true when two iterators point to the same nodes in a list.
			/// </summary>
			/// <returns>True or False</returns>
			bool operator==(const ConstIterator& rhs) const;

			/// <summary>
			/// Reassigns iterator to the next node in a list.
			/// </summary>
			/// <returns>Constant iterator reference</returns>
			ConstIterator& operator++();

			/// <summary>
			/// Reassigns constant iterator to the next node in a list. However, a copy of the iterator is made first before 
			/// reassigning constant.
			/// </summary>
			/// <param name="">Postfix int overload identifier</param>
			/// <returns>Constant iterator</returns>
			ConstIterator operator++(int);

		private:
			/// <summary>
			/// Constant Node* the iterator points to.
			/// </summary>
			const Node* _node{ nullptr };
			/// <summary>
			/// Constant SList* associated with Iterator.
			/// </summary>
			const SList* _container{ nullptr };
		};

	public:
		/// <summary>
		/// Default constructor for SList.
		/// </summary>
		SList() = default;

		/// <summary>
		/// Initializer list constructor for SList
		/// </summary>
		/// <param name="list">- std::initializer_list of generic type</param>
		SList(std::initializer_list<value_type> list);

		/// <summary>
		/// Copy constructor that deep copies list.
		/// </summary>
		SList(const SList& rhs); 
		
		/// <summary>
		/// Copy constructor that deep copies list.
		/// </summary>
		SList(SList&& rhs) noexcept;

		/// <summary>
		/// Copy Assignment Operator that deep copies list.
		/// </summary>
		/// <param name="other">- List to copy</param>
		/// <returns>Reference to list copy</returns>
		SList& operator=(const SList& other);

		/// <summary>
		/// Destruct SList.
		/// </summary>
		~SList();

		/// <summary>
		/// Get reference to data stored in node at the front of a list.
		/// </summary>
		/// <returns>Data stored in node</returns>
		[[nodiscard]] reference Front();

		/// <summary>
		/// Get reference to data stored in node at the front of a list.
		/// </summary>
		/// <returns>Data stored in node</returns>
		[[nodiscard]] const_reference Front() const;
		
		/// <summary>
		/// Add node to the front of the list.
		/// </summary>
		/// <param name="">Data stored in node</param>
		Iterator PushFront(const_reference);

		/// <summary>
		/// Add node to the front of the list.
		/// </summary>
		/// <param name="">Data stored in node</param>
		Iterator PushFront(rvalue_reference);

		/// <summary>
		/// Removes first node in a list.
		/// </summary>
		void PopFront();

		/// <summary>
		/// Gets reference to data stored in node at the back of a list.
		/// </summary>
		/// <returns>Data stored in node</returns>
		[[nodiscard]] reference Back();

		/// <summary>
		/// Gets reference to data stored in node at the back of a list.
		/// </summary>
		/// <returns>Data stored in node</returns>
		[[nodiscard]] const_reference Back() const;
		
		/// <summary>
		/// Add node to the end of a list.
		/// </summary>
		/// <param name="">Data stored in node</param>
		Iterator PushBack(const_reference);
		
		/// <summary>
		/// Add node to the end of a list.
		/// </summary>
		/// <param name="">Data stored in node</param>
		Iterator PushBack(rvalue_reference);

		/// <summary>
		/// Removes the last node in a list.
		/// </summary>
		void PopBack();

		/// <summary>
		/// Gets number of nodes in a list.
		/// </summary>
		/// <returns>List size</returns>
		[[nodiscard]] size_type Size() const;

		/// <summary>
		/// Check if the SList is size 0.
		/// </summary>
		/// <returns>True or False</returns>
		[[nodiscard]] bool IsEmpty() const;

		/// <summary>
		/// Delete all nodes in a list.
		/// </summary>
		void Clear();
		
		/// <summary>
		/// Returns a SList::Iterator pointing to the head of the list.
		/// </summary>
		/// <returns>SList::Iterator</returns>
		[[nodiscard]] Iterator begin();

		/// <summary>
		/// Returns a SList::Iterator pointing one past the end of the list. 
		/// </summary>
		/// <returns>SList::Iterator</returns>
		[[nodiscard]] Iterator end();

		/// <summary>
		/// Returns a SList::Iterator pointing to the head of the list.
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		[[nodiscard]] ConstIterator begin() const;

		/// <summary>
		/// Returns a SList::Iterator pointing one past the end of the list. 
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] ConstIterator end() const;

		/// <summary>
		/// Returns a SList::ConstIterator pointing to the head of the list.
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		[[nodiscard]] const ConstIterator cbegin() const;

		/// <summary>
		/// Returns a SList::ConstIterator pointing one past the end of the list. 
		/// </summary>
		/// <returns>SList::ConstIterator</returns>
		[[nodiscard]] const ConstIterator cend() const;


		/// <summary>
		/// Finds a value in the list and returns an iterator with node pointing to that value.
		/// </summary>
		/// <param name="value">- Generic reference type</param>
		/// <returns>Iterator</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		[[nodiscard]] Iterator Find(reference value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;

		/// <summary>
		/// Finds a value in the list and returns an iterator with node pointing to that value.
		/// </summary>
		/// <param name="value">- Constant generic reference value</param>
		/// <returns>Iterator</returns>
		template <typename EqualityFunctor = DefaultEquality<T>>
		[[nodiscard]] const Iterator Find(const_reference value, EqualityFunctor equalityFunctor = EqualityFunctor{}) const;

		/// <summary>
		/// Inserts given value after the given iterator.
		/// </summary>
		/// <param name="it">- Iterator pointing to the node the value should be inserted after</param>
		/// <param name="value"> - constant reference value to be inserted into list</param>
		/// <returns>Iterator of value that was inserted</returns>
		Iterator InsertAfter(Iterator it, const_reference value);

		/// <summary>
		/// Remove given value from list.
		/// </summary>
		/// <param name="value">- Constant reference value to be removed</param>
		/// <returns>True or False if remove succeeded</returns>
		bool Remove(const_reference value);

		/// <summary>
		/// Remove value that given iterator points to from list.
		/// </summary>
		/// <param name="it">- Iterator pointint to value to be removed</param>
		/// <returns>True or False if remove succeeded</returns>
		bool Remove(Iterator it);
	};
}

#include "SList.inl"