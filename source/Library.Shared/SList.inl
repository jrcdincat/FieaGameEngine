#include "SList.h"

namespace FieaGameEngine
{
	template<typename T>
	inline SList<T>::Node::Node(const_reference data, Node* next) :
		Data{ data }, Next{ next }
	{
	}

	template<typename T>
	inline SList<T>::Node::Node(rvalue_reference data, Node* next) :
		Data{ std::move(data) }, Next{ next }
	{
	}

#pragma region Iterator
	template<typename T>
	inline SList<T>::reference SList<T>::Iterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Cannot dereference iterator node that is nullptr.");
		}

		return _node->Data;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return (_node != rhs._node);
	}

	template<typename T>
	inline  bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	inline SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("Iterator is not associated with a container.");
		}

		if (_node == nullptr)
		{
			throw std::runtime_error("Iterator node is nullptr.");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		operator++();
		return it;
	}

	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList& container, Node* node) :
		_container{ &container }, _node{node}
	{
	}
#pragma endregion

#pragma region ConstIterator
	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& it) :
		_container{ it._container }, _node{ it._node }
	{
	}

	template<typename T>
	inline typename SList<T>::const_reference SList<T>::ConstIterator::operator*() const
	{
		if (_node == nullptr)
		{
			throw std::runtime_error("Cannot dereference const iterator node that is nullptr.");
		}

		return _node->Data;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		return (_node != rhs._node);
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (_container == nullptr)
		{
			throw std::runtime_error("ConstIterator is not associated with a container.");
		}

		if (_node == nullptr)
		{
			throw std::runtime_error("ConstIterator node is nullptr.");
		}

		if (_node != nullptr)
		{
			_node = _node->Next;
		}

		return *this;
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		operator++();
		return it;
	}

#pragma endregion

	template<typename T>
	inline SList<T>::SList(std::initializer_list<SList<T>::value_type> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	SList<T>::SList(const SList& rhs)
	{
		Node* currentNode = rhs._front;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->Data);
			currentNode = currentNode->Next;
		}
	}

	template<typename T>
	SList<T>::SList(SList&& rhs) noexcept
	{
		Node* currentNode = rhs._front;
		while (currentNode != nullptr)
		{
			PushBack(std::move(currentNode->Data));
			currentNode = currentNode->Next;
		}
		rhs._size = 0;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(const SList& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			Node* currentNode = rhs._front;
			while(currentNode != nullptr)
			{
				PushBack(currentNode->Data);
				currentNode = currentNode->Next;
			}
		}

		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{ 
		Clear();
	}

	template <typename T>
	inline SList<T>::reference SList<T>::Front()
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return _front->Data;
	}

	template <typename T>
	inline SList<T>::const_reference SList<T>::Front() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty.");
		}

		return _front->Data;
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::PushFront(SList<T>::const_reference item)
	{
		_front = new Node{ item, _front};

		if (_size == 0)
		{
			_back = _front;
		}
		++_size;

		return Iterator{ *this, _front };
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::PushFront(SList<T>::rvalue_reference item)
	{
		_front = new Node{ std::move(item), _front };

		if (_size == 0)
		{
			_back = _front;
		}
		++_size;

		return Iterator{ *this, _front };
	}


	template<typename T>
	inline void SList<T>::PopFront()
	{
		if (_front != nullptr)
		{
			Node* nextNode = _front->Next;
			delete _front;
			_front = nextNode;
			--_size;

			if (_size == 0)
			{
				_back = nullptr;
			}
			else if (_size == 1)
			{
				_back = _front;
			}
		}
	}

	template<typename T>
	inline SList<T>::reference SList<T>::Back()
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty.");
		}
		return _back->Data;
	}

	template<typename T>
	inline SList<T>::const_reference SList<T>::Back() const
	{
		if (_size == 0)
		{
			throw std::runtime_error("List is empty.");
		}
		return _back->Data;
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::PushBack(SList::const_reference item)
	{
		Node* node = new Node{ item };

		if (_size == 0)
		{
			_front = node;
		}
		else
		{
			_back->Next = node;
		}

		_back = node;
		++_size;

		return Iterator{ *this, node };
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::PushBack(rvalue_reference item)
	{
		Node* node = new Node{ std::move(item) };

		if (_size == 0)
		{
			_front = node;
		}
		else
		{
			_back->Next = node;
		}

		_back = node;
		++_size;

		return Iterator{ *this, node };
	}

	template<typename T>
	inline void SList<T>::PopBack() // TODO: Change so doesn't use loop, but instead does copy and destroys next node instead, maybe not change? 
	{
		if (_size == 1)
		{
			PopFront();
		}
		else if (_size > 1)
		{
			Node* currentNode = _front;
			assert(currentNode != nullptr);

			while (currentNode->Next != _back)
			{
				currentNode = currentNode->Next;
			}
			currentNode->Next = nullptr;
			delete _back;
			_back = currentNode;
			--_size;
		}
	}

	template <typename T>
	inline typename SList<T>::size_type FieaGameEngine::SList<T>::Size() const
	{
		return _size;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return _size == 0;
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		Node* currentNode = _front;
		while (currentNode != nullptr)
		{
			Node* nodeToDelete = currentNode;

			currentNode = nodeToDelete->Next;
			delete nodeToDelete;
		}

		_size = 0;
		_front = _back = nullptr;
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::begin()
	{
		return Iterator{ *this, _front };
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::end()
	{
		return Iterator{ *this };
	}

	template<typename T>
	inline SList<T>::ConstIterator SList<T>::begin() const
	{
		return ConstIterator{ Iterator{ *this, _front } };
	}

	template<typename T>
	inline SList<T>::ConstIterator SList<T>::end() const
	{
		return ConstIterator{ Iterator{ *this } };
	}


	template<typename T>
	inline const SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return ConstIterator{ Iterator{ *this, _front } };
	}

	template<typename T>
	inline const SList<T>::ConstIterator SList<T>::cend() const
	{
		return ConstIterator{ Iterator{ *this } };
	}


	template<typename T>
	template<typename EqualityFunctor>
	inline SList<T>::Iterator SList<T>::Find(reference value, EqualityFunctor equalityFunctor) const
	{
		Node* currentNode = _front;

		while (currentNode != nullptr)
		{
			if (equalityFunctor(currentNode->Data, value))
			{
				return Iterator{ *this, currentNode };
			}
			currentNode = currentNode->Next;
		}

		return Iterator{ *this };
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline const SList<T>::Iterator SList<T>::Find(const_reference value, EqualityFunctor equalityFunctor) const
	{
		Node* currentNode = _front;

		while (currentNode != nullptr)
		{
			if (equalityFunctor(currentNode->Data, value))
			{
				return Iterator{ *this, currentNode };
			}
			currentNode = currentNode->Next;
		}

		return Iterator{ *this };
	}

	template<typename T>
	inline SList<T>::Iterator SList<T>::InsertAfter(SList<T>::Iterator it, const_reference value)
	{
		Iterator insertedIterator;
		if (it._node == nullptr)
		{
			insertedIterator = PushBack(value);
		}
		else
		{
			Node* nodeToInsert = new Node{ value, it._node->Next };
			it._node->Next = nodeToInsert;
			insertedIterator = Iterator{ *this, nodeToInsert };
			_size++;
		}
		return insertedIterator;
	}

	template <typename T>
	inline bool SList<T>::Remove(Iterator it)
	{
		if (it != end())
		{
			if (_size == 1 || it == begin())
			{
				PopFront();
				return true;
			}

			for (Iterator iterator = begin(); iterator != it; ++iterator)
			{
				if (iterator._node->Next == it._node)
				{
					iterator._node->Next = it._node->Next;
					delete it._node;
					--_size;
					return true;
				}
			}
		}
		return false;
	}

	template<typename T>
	inline bool SList<T>::Remove(const_reference value)
	{
		Iterator iterator = Find(value);
		return (Remove(iterator));
	}
}