#include "pch.h"
#include "Datum.h"
#include <string>

using namespace std::string_literals;

namespace FieaGameEngine
{
	const HashMap<Datum::DatumTypes, std::string> Datum::DatumTypesStringMap =
	{
		{ DatumTypes::Unknown, "unknown"s },
		{ DatumTypes::Integer, "integer"s },
		{ DatumTypes::Float, "float"s },
		{ DatumTypes::Vector, "vector"s },
		{ DatumTypes::Matrix, "matrix"s },
		{ DatumTypes::Table, "table"s },
		{ DatumTypes::String, "string"s },
		{ DatumTypes::Pointer, "pointer"s },
		{ DatumTypes::Byte, "byte"s }
	};

	const HashMap<std::string, Datum::DatumTypes> Datum::StringDatumTypesMap =
	{
		{ "unknown"s , DatumTypes::Unknown},
		{ "integer"s, DatumTypes::Integer },
		{ "float"s, DatumTypes::Float, },
		{ "vector"s, DatumTypes::Vector },
		{ "matrix"s, DatumTypes::Matrix },
		{ "table"s, DatumTypes::Table },
		{ "string"s, DatumTypes::String },
		{ "pointer"s, DatumTypes::Pointer },
		{ "byte"s, DatumTypes::Byte }
	};

	Datum::Datum(DatumTypes type)
	{
		_type = type;
	}

	Datum::Datum(DatumTypes type, size_type capacity)
	{
		if (type == DatumTypes::Unknown && capacity > 0)
		{
			throw std::runtime_error("Cannot allocate memory for unknown data type.");
		}

		_type = type;
		Reserve(capacity);
	}

	Datum::Datum(int value)
	{
		_type = DatumTypes::Integer;
		PushBack(value);
	}

	Datum::Datum(float value)
	{
		_type = DatumTypes::Float;
		PushBack(value);
	}

	Datum::Datum(const std::string& value)
	{
		_type = DatumTypes::String;
		PushBack(value);
	}

	Datum::Datum(Scope* value)
	{
		_type = DatumTypes::Table;
		PushBack(*value);
	}

	Datum::Datum(const glm::vec4 value)
	{
		_type = DatumTypes::Vector;
		PushBack(value);
	}

	Datum::Datum(const glm::mat4& value)
	{
		_type = DatumTypes::Matrix;
		PushBack(value);
	}

	Datum::Datum(RTTI* value)
	{
		_type = DatumTypes::Pointer;
		PushBack(value);
	}

	Datum::Datum(const Datum& other)
	{
		SetType(other._type);
		Reserve(other._capacity);

		switch (other._type)
		{
		case DatumTypes::Integer:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetInteger(index));
			}
			break;
		case DatumTypes::Float:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetFloat(index));
			}
			break;
		case DatumTypes::String:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetString(index));
			}
			break;
		case DatumTypes::Table:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetScope(index));
			}
			break;
		case DatumTypes::Vector:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetVector(index));
			}
			break;
		case DatumTypes::Matrix:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetMatrix(index));
			}
			break;
		case DatumTypes::Pointer:
			for (size_type index = 0; index < other._size; ++index)
			{
				PushBack(other.GetPointer(index));
			}
			break;
		}
	}

	Datum::Datum(Datum&& other) noexcept :
		_data{ other._data }, _type{ other._type }, _capacity{ other._capacity }, _size{ other._size }, _isExternal{ other._isExternal }
	{
		other._size = 0;
		other._capacity = 0;
		other._data.vp = nullptr;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		_data = rhs._data;
		_type = rhs._type;
		_capacity = rhs._capacity;
		_size = rhs._capacity;
		_isExternal = rhs._isExternal;

		rhs._size = 0;
		rhs._capacity = 0;
		rhs._data.vp = nullptr;
		return *this;
	}

	Datum::~Datum()
	{
		if (!_isExternal)
		{
			Clear();
			free(_data.vp);
		}
	}

#pragma region Initializer Lists
	Datum::Datum(std::initializer_list<int> list)
	{
		SetType(DatumTypes::Integer);
		PushBackInitializerList(list);
	}

	Datum::Datum(std::initializer_list<float> list)
	{
		SetType(DatumTypes::Float);
		PushBackInitializerList(list);
	}

	Datum::Datum(std::initializer_list<std::string> list)
	{
		SetType(DatumTypes::String);
		PushBackInitializerList(list);
	}

	Datum::Datum(std::initializer_list<glm::vec4> list)
	{
		SetType(DatumTypes::Vector);
		PushBackInitializerList(list);
	}

	Datum::Datum(std::initializer_list<glm::mat4> list)
	{
		SetType(DatumTypes::Matrix);
		PushBackInitializerList(list);
	}

	Datum::Datum(std::initializer_list<RTTI*> list)
	{
		SetType(DatumTypes::Pointer);
		PushBackInitializerList(list);
	}

#pragma endregion

#pragma region Comparison Operators
	const bool Datum::operator==(const Datum& rhs) const
	{
		if (_type != rhs._type || _size != rhs._size || _capacity != rhs._capacity)
		{
			return false;
		}

		switch (rhs._type)
		{
		case DatumTypes::String:
			for (size_type index = 0; index < _size; ++index)
			{
				if (_data.s[index] != rhs._data.s[index])
				{
					return false;
				}
			}
			break;
		case DatumTypes::Table:
		case DatumTypes::Pointer:
			for (size_type index = 0; index < _size; ++index)
			{
				if (_data.p[index]->ToString() != rhs._data.p[index]->ToString())
				{
					return false;
				}
			}
			break;
		default:
			int zeroIfEqual = memcmp(_data.vp, rhs._data.vp, _typeSizes[static_cast<int>(_type)]);
			if (zeroIfEqual != 0)
			{
				return false;
			}
			break;
		}

		return true;
	}

	const bool Datum::operator!=(const Datum& rhs) const
	{
		return !operator==(rhs);
	}

	const bool Datum::operator==(const int rhs) const
	{
		if (_size == 0)
		{
			return false;
		}

		return FrontInteger() == rhs;
	}

	const bool Datum::operator!=(const int rhs) const
	{
		return !operator==(rhs);
	}

	const bool Datum::operator==(const float rhs) const
	{
		if (_size == 0)
		{
			return false;
		}

		return FrontFloat() == rhs;
	}

	const bool Datum::operator!=(const float rhs) const
	{
		return !operator==(rhs);
	}

	const bool Datum::operator==(const std::string& rhs) const
	{
		if (_size == 0)
		{
			return false;
		}

		return FrontString() == rhs;
	}

	const bool Datum::operator!=(const std::string& rhs) const
	{
		return !operator==(rhs);
	}

	const bool Datum::operator==(const glm::vec4 rhs) const
	{
		if (_size == 0)
		{
			return false;
		}

		return FrontVector() == rhs;
	}

	const bool Datum::operator!=(const glm::vec4 rhs) const
	{
		return !operator==(rhs);
	}

	const bool Datum::operator==(const glm::mat4 rhs) const
	{
		if (_size == 0)
		{
			return false;
		}

		return FrontMatrix() == rhs;
	}

	const bool Datum::operator!=(const glm::mat4 rhs) const
	{
		return !operator==(rhs);
	}

	const bool Datum::operator==(const RTTI* rhs) const
	{
		if (_size == 0)
		{
			return false;
		}

		return rhs->Equals(FrontPointer());
	}

	const bool Datum::operator!=(const RTTI* rhs) const
	{
		return !operator==(rhs);
	}
#pragma endregion

#pragma region Assignment Operators
	Datum& Datum::operator=(Datum& rhs)
	{
		if (_isExternal)
		{
			return *this;
		}

		Clear();
		ShrinkToFit();

		_type = rhs._type;
		_isExternal = rhs._isExternal;

		if (!rhs._isExternal)
		{
			Reserve(rhs._capacity);
			for (size_type index = 0; index < rhs._size; ++index)
			{
				switch (_type)
				{
				case DatumTypes::Integer:
					PushBack(rhs.GetInteger(index));
					break;
				case DatumTypes::Float:
					PushBack(rhs.GetFloat(index));
					break;
				case DatumTypes::String:
					PushBack(rhs.GetString(index));
					break;
				case DatumTypes::Vector:
					PushBack(rhs.GetVector(index));
					break;
				case DatumTypes::Matrix:
					PushBack(rhs.GetMatrix(index));
					break;
				case DatumTypes::Pointer:
					PushBack(rhs.GetPointer(index));
					break;
				}
			}
		}
		else
		{
			_size = rhs._size;
			_capacity = rhs._capacity;
			_data = rhs._data;
			rhs._data.vp = nullptr;
			rhs._size = 0;
			rhs._capacity = 0;
		}

		return *this;
	}

	Datum& Datum::operator=(int rhs)
	{
		assert(_type == DatumTypes::Unknown || _type == DatumTypes::Integer);
		_type = DatumTypes::Integer;
		PushBack(rhs);
		return *this;
	}

	Datum& Datum::operator=(float rhs)
	{
		assert(_type == DatumTypes::Unknown || _type == DatumTypes::Float);
		_type = DatumTypes::Float;
		PushBack(rhs);
		return *this;
	}

	Datum& Datum::operator=(const std::string& rhs)
	{
		assert(_type == DatumTypes::Unknown || _type == DatumTypes::String);
		_type = DatumTypes::String;
		PushBack(rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4 rhs)
	{
		assert(_type == DatumTypes::Unknown || _type == DatumTypes::Vector);
		_type = DatumTypes::Vector;
		PushBack(rhs);
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& rhs)
	{
		assert(_type == DatumTypes::Unknown || _type == DatumTypes::Matrix);
		_type = DatumTypes::Matrix;
		PushBack(rhs);
		return *this;
	}

	Datum& Datum::operator=(RTTI* rhs)
	{
		assert(_type == DatumTypes::Unknown || _type == DatumTypes::Pointer);
		_type = DatumTypes::Pointer;
		PushBack(rhs);
		return *this;
	}
#pragma endregion

	void Datum::Reserve(const size_type capacity)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot reserve memory of external storage.");
		}

		if (capacity > _capacity)
		{
			size_t size = _typeSizes[static_cast<int>(_type)];
			void* data = realloc(_data.vp, capacity * size);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}

	void Datum::Clear()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot reserve memory of external storage.");
		}
		else if (_type == DatumTypes::String)
		{
			for (size_type index = 0; index < _size; ++index)
			{
				_data.s[index].~basic_string();
			}
		}

		_size = 0;
	}

	void Datum::Resize(const size_type size)
	{
		if (size > _capacity)
		{
			size_type capacity = _capacity;
			Reserve(size);

			switch (_type)
			{
			case DatumTypes::Integer:
				PushBackDefault(size - capacity, int{});
				break;
			case DatumTypes::Float:
				PushBackDefault(size - capacity, float{});
				break;
			case DatumTypes::String:
				PushBackDefault(size - capacity, std::string{});
				break;
			case DatumTypes::Vector:
				PushBackDefault(size - capacity, glm::vec4{});
				break;
			case DatumTypes::Matrix:
				PushBackDefault(size - capacity, glm::mat4{});
				break;
			case DatumTypes::Pointer:
				PushBackDefault(size - capacity, nullptr);
				break;
			}
		}
		else if (size > _size && size < _capacity)
		{
			switch (_type)
			{
			case DatumTypes::Integer:
				PushBackDefault(_capacity - size, int{});
				break;
			case DatumTypes::Float:
				PushBackDefault(_capacity - size, float{});
				break;
			case DatumTypes::String:
				PushBackDefault(_capacity - size, std::string{});
				break;
			case DatumTypes::Vector:
				PushBackDefault(_capacity - size, glm::vec4{});
				break;
			case DatumTypes::Matrix:
				PushBackDefault(_capacity - size, glm::mat4{});
				break;
			case DatumTypes::Pointer:
				PushBackDefault(_capacity - size, nullptr);
				break;
			}
			ShrinkToFit();
		}
		else
		{
			ShrinkToFit();
		}
	}
	
	void Datum::ShrinkToFit()
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot reserve memory of external storage.");
		}
		
		if (_size == 0)
		{
			free(_data.vp);
		}
		
		if (_capacity != _size)
		{
			Reserve(_size);
		}

		_capacity = _size;
	}

#pragma region Set Storage
	void Datum::SetStorage(int* intArray, const size_type arraySize)
	{
		SetStorage(DatumTypes::Integer, intArray, arraySize);
	}

	void Datum::SetStorage(float* floatArray, const size_type arraySize)
	{
		SetStorage(DatumTypes::Float, floatArray, arraySize);
	}

	void Datum::SetStorage(std::string* stringArray, const size_type arraySize)
	{
		SetStorage(DatumTypes::String, stringArray, arraySize);
	}

	void Datum::SetStorage(glm::vec4* vectorArray, const size_type arraySize)
	{
		SetStorage(DatumTypes::Vector, vectorArray, arraySize);
	}

	void Datum::SetStorage(glm::mat4* matrixArray, const size_type arraySize)
	{
		SetStorage(DatumTypes::Matrix, matrixArray, arraySize);
	}

	void Datum::SetStorage(RTTI** pointerArray, const size_type arraySize)
	{
		SetStorage(DatumTypes::Pointer, pointerArray, arraySize);
	}

	void Datum::SetStorage(DatumTypes type, void* data, size_type size)
	{
		assert(data != nullptr);
		if (size == 0)
		{
			throw std::invalid_argument("External storage size must be greater than 0.");
		}

		if (_type == DatumTypes::Unknown)
		{
			SetType(type);
		}
		else if (_type != type)
		{
			throw std::runtime_error("Invalid type.");
		}

		if (_isExternal == false && _capacity > 0)
		{
			throw std::runtime_error("Unable to change interal storage to external.");
		}

		_isExternal = true;
		_data.vp = data;
		_size = size;
		_capacity = size;
	}
#pragma endregion

	void Datum::SetFromString(const std::string& string, const size_type index)
	{
		assert(_type != DatumTypes::Unknown && _type != DatumTypes::Pointer);
		glm::vec4 vector;
		glm::mat4 matrix;

		switch (_type)
		{
		case DatumTypes::Integer:
			Set(stoi(string), index);
			break;
		case DatumTypes::Float:
			Set(stof(string), index);
			break;
		case DatumTypes::String:
			Set(string, index);
			break;
		case DatumTypes::Vector:
			sscanf_s(string.c_str(), "vec4( %f, %f, %f, %f)", &vector.x, &vector.y, &vector.z, &vector.a);
			Set(vector, index);
			break;
		case DatumTypes::Matrix:
			sscanf_s(string.c_str(), "mat4(vec4( %f, %f, %f, %f ), vec4( %f, %f, %f, %f ), vec4( %f, %f, %f, %f ), vec4( %f, %f, %f, %f )))", &matrix[0].x, &matrix[0].y, &matrix[0].z, &matrix[0].a, &matrix[1].x, &matrix[1].y, &matrix[1].z, &matrix[1].a, &matrix[2].x, &matrix[2].y, &matrix[2].z, &matrix[2].a, &matrix[3].x, &matrix[3].y, &matrix[3].z, &matrix[3].a);
			Set(matrix, index);
			break;
		}
	}

	std::string Datum::ToString(const size_type index)
	{
		std::string string;
		if (index >= _size)
		{
			throw std::invalid_argument("Index out of bounds.");
		}

		assert(_type != DatumTypes::Unknown);
		switch (_type)
		{
		case FieaGameEngine::Datum::DatumTypes::Integer:
			string = std::to_string(_data.i[index]);
			break;
		case FieaGameEngine::Datum::DatumTypes::Float:
			string = std::to_string(_data.f[index]);
			break;
		case FieaGameEngine::Datum::DatumTypes::String:
			return _data.s[index];
			break;
		case FieaGameEngine::Datum::DatumTypes::Vector:
			string = glm::to_string(_data.v[index]);
			break;
		case FieaGameEngine::Datum::DatumTypes::Matrix:
			string = glm::to_string(_data.m[index]);
			break;
		case FieaGameEngine::Datum::DatumTypes::Pointer:
			string = _data.p[index]->ToString();
			break;
		}
		return string;
	}


#pragma region PushBack
	void Datum::PushBack(const int value)
	{
		SetType(DatumTypes::Integer);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}

		new (_data.i + _size) int{ value };
		++_size;
	}

	void Datum::PushBack(const float value)
	{
		SetType(DatumTypes::Float);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}

		new (_data.f + _size) float{ value };
		++_size;
	}

	void Datum::PushBack(const std::string& value)
	{
		SetType(DatumTypes::String);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}
		new (_data.s + _size) std::string{ value };
		++_size;
	}

	void Datum::PushBack(std::string&& value)
	{
		SetType(DatumTypes::String);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}
		new (_data.s + _size) std::string{ value };
		++_size;;
	}

	void Datum::PushBack(Scope& value)
	{
		SetType(DatumTypes::Table);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}
		new (_data.table + _size) Scope* { &value };
		++_size;
	}

	void Datum::PushBack(const glm::vec4 value)
	{
		SetType(DatumTypes::Vector);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}

		new (_data.v + _size) glm::vec4{ value };
		++_size;
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		SetType(DatumTypes::Matrix);
		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}

		new (_data.m + _size) glm::mat4{ value };
		++_size;
	}

	void Datum::PushBack(RTTI* value)
	{
		if (_type != DatumTypes::Table)
		{
			SetType(DatumTypes::Pointer);
		}

		if (_size == _capacity)
		{
			size_type newCapacity = _capacity + 1;
			Reserve(newCapacity);
		}

		new (_data.p + _size) RTTI* { value };
		++_size;
	}
#pragma endregion

#pragma region Remove
	bool Datum::Remove(int value)
	{
		return RemoveAt(IndexOf(value));
	}

	bool Datum::Remove(float value)
	{
		return RemoveAt(IndexOf(value));
	}

	bool Datum::Remove(const std::string& value)
	{
		return RemoveAt(IndexOf(value));
	}

	bool Datum::Remove(glm::vec4 value)
	{
		return RemoveAt(IndexOf(value));
	}

	bool Datum::Remove(glm::mat4 value)
	{
		return RemoveAt(IndexOf(value));
	}

	bool Datum::Remove(RTTI* value)
	{
		return RemoveAt(IndexOf(value));
	}

	bool Datum::RemoveAt(size_type index)
	{
		if (_isExternal)
		{
			throw std::runtime_error("Cannot remove from external datum.");
		}

		if (index >= _size)
		{
			return false;
		}

		if (_type == DatumTypes::String)
		{
			_data.s[index].~basic_string();
		}

		const size_type elementCount = _size - index - 1; 
		const auto elementSize = _typeSizes[static_cast<size_type>(_type)];
		const auto size = elementCount * elementSize;
		if (size > 0)
		{
			std::byte* destination = _data.b + index * elementSize;
			std::byte* source = destination + elementSize;
			memmove(destination, source, size);
		}

		--_size;

		return true;
	}
#pragma endregion
}
