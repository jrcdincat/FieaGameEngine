#include "Datum.h"

namespace FieaGameEngine
{
#pragma region Datum
	template<typename T>
	inline void Datum::PushBackInitializerList(std::initializer_list<T>& list)
	{
		Reserve(list.size());
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	inline const Datum::DatumTypes Datum::Type() const
	{
		return _type;
	}

	inline void Datum::SetType(DatumTypes type)
	{
		if (_type != DatumTypes::Unknown && _type != type)
		{
			throw std::invalid_argument("Datum type is already set, types cannot be changed after they have been set previously.");
		}
		
		_type = type;
	}

	inline const Datum::size_type Datum::Size() const
	{
		return _size;
	}

	inline const Datum::size_type Datum::Capacity() const
	{
		return _capacity;
	}

	template<typename T>
	inline void Datum::PushBackDefault(size_t numberOfElements, T defaultValue)
	{
		for (size_type index = 0; index < numberOfElements; ++index)
		{
			PushBack(defaultValue);
		}
	}

	inline const bool Datum::IsExternal() const
	{
		return _isExternal;
	}

#pragma region Set
	inline void Datum::Set(int value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}

		_data.i[index] = value;
	}

	inline void Datum::Set(float value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}

		_data.f[index] = value;
	}

	inline void Datum::Set(const std::string& value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}

		_data.s[index] = value;
	}

	inline void Datum::Set(Scope* value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}
		_data.table[index] = value;
	}

	inline void Datum::Set(glm::vec4 value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}

		_data.v[index] = value;
	}

	inline void Datum::Set(glm::mat4 value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}

		_data.m[index] = value;
	}

	inline void Datum::Set(RTTI* value, const size_type index)
	{
		if (index >= _size)
		{
			throw std::invalid_argument("Cannot set value. Index out of range.");
		}

		_data.p[index] = value;
	}
#pragma endregion

#pragma region Getters
	inline int& Datum::GetInteger(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.i[index];
	}

	inline const int Datum::GetConstInteger(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.i[index];
	}

	inline float& Datum::GetFloat(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.f[index];
	}

	inline const float Datum::GetConstFloat(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.f[index];
	}

	inline std::string& Datum::GetString(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.s[index];
	}

	inline Scope& Datum::GetScope(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return *_data.table[index];
	}

	inline const std::string& Datum::GetConstString(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.s[index];
	}

	inline glm::vec4& Datum::GetVector(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.v[index];
	}

	inline const glm::vec4 Datum::GetConstVector(const size_type index) const
	{
		assert(index < _size && index >= 0);
		return _data.v[index];
	}
	inline glm::mat4& Datum::GetMatrix(const size_type index) const
	{
		assert(index < _size && index >= 0);

		return _data.m[index];
	}

	inline const glm::mat4 Datum::GetConstMatrix(const size_type index) const
	{
		assert(index < _size && index >= 0);

		return _data.m[index];
	}

	inline RTTI* Datum::GetPointer(const size_type index) const
	{
		assert(index < _size && index >= 0);

		return _data.p[index];
	}

#pragma endregion

#pragma region PopBack
	inline void Datum::PopBackInteger()
	{
		assert(_size != 0);
		--_size;
	}

	inline void Datum::PopBackFloat()
	{
		assert(_size != 0);
		--_size;
	}

	inline void Datum::PopBackString()
	{
		assert(_size != 0);
		--_size;
		_data.s[_size].~basic_string();
	}

	inline void Datum::PopBackVector()
	{
		assert(_size != 0);
		--_size;
	}

	inline void Datum::PopBackMatrix()
	{
		assert(_size != 0);
		--_size;
	}

	inline void Datum::PopBackPointer()
	{
		assert(_size != 0);
		--_size;
	}
#pragma endregion

#pragma region Front
	inline int& Datum::FrontInteger() const
	{
		assert(_size != 0);
		return _data.i[0];
	}

	inline float& Datum::FrontFloat() const
	{
		assert(_size != 0);
		return _data.f[0];
	}

	inline glm::vec4& Datum::FrontVector() const
	{
		assert(_size != 0);
		return _data.v[0];
	}

	inline std::string& Datum::FrontString() const
	{
		assert(_size != 0);
		return _data.s[0];
	}

	inline glm::mat4& Datum::FrontMatrix() const
	{
		assert(_size != 0);
		return _data.m[0];
	}

	inline RTTI* Datum::FrontPointer() const
	{
		assert(_size != 0);
		return _data.p[0];
	}
#pragma endregion

#pragma region Back
	inline int& Datum::BackInteger() const
	{
		assert(_size != 0);
		return _data.i[_size - 1];
	}

	inline float& Datum::BackFloat() const
	{
		assert(_size != 0);
		return _data.f[_size - 1];
	}

	inline std::string& Datum::BackString() const
	{
		assert(_size != 0);
		return _data.s[_size - 1];
	}

	inline glm::vec4& Datum::BackVector() const
	{
		assert(_size != 0);
		return _data.v[_size - 1];
	}

	inline glm::mat4& Datum::BackMatrix() const
	{
		assert(_size != 0);
		return _data.m[_size - 1];
	}

	inline RTTI* Datum::BackPointer() const
	{
		assert(_size != 0);
		return _data.p[_size - 1];
	}
#pragma endregion
#pragma region IndexOf
	inline Datum::size_type Datum::IndexOf(const int value)
	{
		return IndexOf(_data.i, value);
	}

	inline Datum::size_type Datum::IndexOf(const float value)
	{
		return IndexOf(_data.f, value);
	}

	inline Datum::size_type Datum::IndexOf(const std::string& value)
	{
		return IndexOf(_data.s, value);
	}

	inline Datum::size_type Datum::IndexOf(const glm::vec4 value)
	{
		return IndexOf(_data.v, value);
	}

	inline Datum::size_type Datum::IndexOf(const glm::mat4& value)
	{
		return IndexOf(_data.m, value);
	}

	inline Datum::size_type Datum::IndexOf(const RTTI* value)
	{
		return IndexOf(_data.p, value);
	}

	template<typename TData, typename TValue>
	inline Datum::size_type Datum::IndexOf(TData data, TValue& value)
	{
		for (size_type index = 0; index < _size; ++index)
		{
			if (data[index] == value)
			{
				return index;
			}
		}
		return _size;
	}
#pragma endregion
#pragma endregion
}
