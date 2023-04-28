#pragma once
#include <cstdint>
#include <cassert>
#include "RTTI.h"

namespace UnitTests
{

	class Foo final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, FieaGameEngine::RTTI);
	public:
		explicit Foo(std::int32_t data = 0);
		Foo(const Foo& rhs); 
		Foo(Foo&& rhs) noexcept;
		Foo& operator=(const Foo& rhs);
		Foo& operator=(Foo&& rhs) noexcept;
		virtual ~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;

		bool Equals(const RTTI* rhs) const override;

		std::string ToString() const override;

		std::int32_t Data() const;
		void SetData(std::int32_t data);

	private:
		std::int32_t* _data;
	};
}

