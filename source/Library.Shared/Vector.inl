#include "Vector.h"

namespace FieaGameEngine
{
#pragma region Iterator
	template<typename T> 
	inline Vector<T>::Iterator::Iterator(const Vector& container, value_type* data):
		_container{&container}, _data{data}
	{
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::Iterator::operator*() const
	{
		if (_data == nullptr || _data == _container->end()._data)
		{
			throw std::runtime_error("Cannot dereference iterator data that is nullptr.");
		}

		return *(_data);
	}

	template<typename T> 
	inline bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		if (this->_container != rhs._container)
		{
			return false;
		}
		
		return (_data != rhs._data);
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	inline Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		_data = (_data + 1);
		return *this;
	}

	template<typename T>
	inline Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator it = *this;
		_data = (_data + 1);
		return it;
	}

	template<typename T>
	inline Vector<T>::Iterator& Vector<T>::Iterator::operator--()
	{
		_data = (_data - 1);
		return *this;
	}

	template<typename T>
	inline Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
	{
		Iterator it = *this;
		_data = (_data - 1);
		return it;
	}

#pragma endregion
#pragma region ConstIterator
	template<typename T> 
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& it):
		_container{ it._container }, _data{ it._data }
	{
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::ConstIterator::operator*() const
	{
		if (_data == _container->end()._data)
		{
			throw std::runtime_error("Cannot dereference iterator data that is nullptr.");
		}

		return *(_data);
	}

	template<typename T> 
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const
	{
		if (this->_container != rhs._container)
		{
			return false;
		}

		return (_data != rhs._data);
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const
	{
		return !operator!=(rhs);
	}

	template<typename T>
	inline Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		_data = (_data + 1);
		return *this;
	}

	template<typename T>
	inline Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator it = *this;
		_data = (_data + 1);
		return it;
	}

	template<typename T>
	inline Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
	{
		_data = (_data - 1);
		return *this;
	}

	template<typename T>
	inline Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
	{
		ConstIterator it = *this;
		_data = (_data - 1);
		return it;
	}
#pragma endregion
#pragma region Vector
	template<typename T>
	inline Vector<T>::Vector(size_type capacity)
	{
		Reserve(capacity);
	}

	template<typename T> 
	inline Vector<T>::Vector(std::initializer_list<value_type> list)
	{
		Reserve(11);

		for (const auto& value : list)
		{
			push_back(value);
		}
	}

	template<typename T>
	Vector<T>::Vector(const Vector& other)
	{
		if (other.size() < 1)
		{
			return;
		}

		Reserve(other.size());
		assert(_data != nullptr);

		for (const auto& value : other)
		{
			push_back(value);
		}
	}

	template<typename T> 
	Vector<T>::Vector(Vector&& other) noexcept:
		_data{other._data}, _capacity{other._capacity}, _size{other._size}
	{
		other._size = 0;
		other._capacity = 0;
		other._data = nullptr;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Reserve(rhs.Capacity());
			for (const auto& value : rhs)
			{
				push_back(value);
			}
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		_data = rhs._data;
		_size = rhs._size;
		_capacity = rhs._capacity;

		rhs._size = 0;
		rhs._capacity = 0;
		rhs._data = nullptr;
		return *this;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		Clear();
		ShrinkToFit();
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::operator[](size_type position)
	{
		if (position >= _size)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return _data[position];
	}

	template<typename T> 
	inline Vector<T>::reference Vector<T>::At(size_type position)
	{
		if (position >= _size || position < 0)
		{
			throw std::runtime_error("Index out of bounds.");
		}
		return _data[position];
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::operator[](size_type position) const
	{
		if (position >= _size) // null check here instead?
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return _data[position];
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::At(size_type position) const
	{
		if (position >= _size || position < 0)
		{
			throw std::runtime_error("Index out of bounds.");
		}

		return _data[position];
	}

	template<typename T> 
	inline void Vector<T>::pop_back()
	{
		assert(_data != nullptr);
		_data[_size - 1].~value_type();
		--_size;
	}


	template<typename T>
	inline void Vector<T>::push_back(const_reference value) 
	{
		// Expand capacity if capacity is already full
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}

		// placement new
		// (memory address we want to put new initialized object) Invoked the constructor of T, but didn't invoke malloc beforehand
		new (_data + _size) T(value);
		++_size;
	}

	template<typename T>
	inline void Vector<T>::push_back(rvalue_reference value)
	{
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1; // TODO
			Reserve(newCapacity);
		}

		new (_data + _size) T(std::move(value));
		++_size;
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::Front()
	{
		assert(_data != nullptr);
		return _data[0];
	}

	template<typename T> 
	inline Vector<T>::const_reference Vector<T>::Front() const
	{
		assert(_data != nullptr);
		return _data[0];
	}

	template<typename T>
	inline Vector<T>::reference Vector<T>::back()
	{
		assert(_data != nullptr);
		return _data[_size - 1];
	}

	template<typename T>
	inline Vector<T>::const_reference Vector<T>::back() const
	{
		assert(_data != nullptr);
		return _data[_size - 1];
	}

	template<typename T>
	inline Vector<T>::size_type Vector<T>::size() const
	{
		return _size;
	}

	template<typename T>
	inline Vector<T>::size_type Vector<T>::Capacity() const
	{
		return _capacity;
	}

	template<typename T> 
	inline void Vector<T>::Reserve(size_type capacity)
	{
		if (capacity > _capacity)
		{
			// Reinterpret_cast has no runtime cost, and does no compile time type checking
			T* data = reinterpret_cast<T*>(realloc(_data, capacity * sizeof(T)));
			assert(data != nullptr);
			_data = data;
			_capacity = capacity;
		}
	}

	template<typename T> 
	inline bool Vector<T>::Remove(const_reference value)
	{
		return (Remove(Find(value)));
	}

	template<typename T>
	inline bool Vector<T>::Remove(Iterator it)
	{
		if (it == end() || it._data == nullptr || it._container != this)
		{
			return false;
		}

		it._data->~value_type();
		size_type numberOfDataToMove = (end()._data - 1) - it._data;
		std::memmove(it._data, (it._data + 1), sizeof(*_data) * numberOfDataToMove);
		--_size;
		return true;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline Vector<T>::Iterator Vector<T>::Find(const_reference value, EqualityFunctor equalityFunctor)
	{
		auto position = begin();
		for (; position != end(); ++position)
		{
			if (equalityFunctor(*position._data, value))
			{
				break;
			}
		}
		return position;
	}

	template<typename T>
	template<typename EqualityFunctor>
	inline Vector<T>::ConstIterator Vector<T>::Find(const_reference value, EqualityFunctor equalityFunctor) const
	{
		auto position = begin();
		for (; position != end(); ++position)
		{
			if (equalityFunctor(*position._data, value))
			{
				break;
			}
		}
		return position;
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return (_size == 0);
	}

	template<typename T> 
	inline void Vector<T>::Clear()
	{
		for (auto it = begin(); it != end(); ++it)
		{
			it._data->~value_type();
		}

		_size = 0;
	}

	template<typename T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (_data != nullptr)
		{
			if (_size == 0)
			{
				free(_data);
				_data = nullptr;
				_capacity = 0;
			}
			else
			{
				auto data = reinterpret_cast<T*>(realloc(_data, _size * sizeof(value_type)));
				assert(data != nullptr);
				_data = data;
				_capacity = _size;
			}
		}
	}

	template<typename T> 
	inline Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator{ *this, _data };
	}

	template<typename T>
	inline Vector<T>::Iterator Vector<T>::end()
	{
		return Iterator{ *this, (_data + _size)};
	}

	template<typename T> 
	inline Vector<T>::Iterator Vector<T>::begin() const
	{
		return Iterator{ Iterator{ *this, _data} };
	}

	template<typename T> 
	inline Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator{ *this,  (_data + _size) };
	}

	template<typename T> 
	inline Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return ConstIterator{ Iterator{ *this, _data}};
	}

	template<typename T>
	inline Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return ConstIterator{ Iterator{ *this, (_data + _size)} };
	}
#pragma endregion
}
