#pragma once
#include "AttributedFoo.h"

namespace UnitTests
{
	class AttributedBar : public AttributedFoo
	{
		RTTI_DECLARATIONS(AttributedBar, AttributedFoo);

	public:
		static const std::size_t ArraySize = 5;

		AttributedBar();
		AttributedBar(const AttributedBar&) = default;
		AttributedBar(AttributedBar&&) = default;
		AttributedBar& operator=(const AttributedBar&) = default;
		AttributedBar& operator=(AttributedBar&&) = default;
		~AttributedBar() = default;

		int ExternalBarInteger = 0;
		int ExternalBarFloat = 0;
		std::string ExternalBarString;

		Scope* Clone() const override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		AttributedBar(RTTI::IdType typeId) : AttributedFoo(typeId) { }; // Forwarding call
	};
}