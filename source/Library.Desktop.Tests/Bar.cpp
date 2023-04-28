#include "pch.h"
#include "Bar.h"

using namespace std;

namespace UnitTests
{
	RTTI_DEFINITIONS(Bar);

	Bar::Bar(int32_t data) :
		_data(new int32_t(data))
	{

	}

	Bar::Bar(const Bar& rhs) :
		_data(new int32_t(*rhs._data))
	{

	}

	Bar::Bar(Bar&& rhs) noexcept :
		_data(rhs._data)
	{
		rhs._data = nullptr;
	}

	Bar& Bar::operator=(const Bar& rhs)
	{
		if (this != &rhs)
		{
			*_data = *rhs._data;
		}
		return *this;
	}

	Bar& Bar::operator=(Bar&& rhs) noexcept
	{
		if (this != &rhs)
		{
			delete _data;
			_data = rhs._data;
			rhs._data = nullptr;
		}
		return *this;
	}

	Bar::~Bar()
	{
		delete _data;
	}

	bool Bar::operator==(const Bar& rhs) const
	{
		return *_data == *rhs._data;
	}

	bool Bar::operator!=(const Bar& rhs) const
	{
		return !operator == (rhs);
	}

	int32_t Bar::Data() const
	{
		return *_data;
	}

	void Bar::SetData(int32_t data)
	{
		*_data = data;
	}
}