#pragma once
#include "Attributed.h"
#include "ConcreteFactory.h"
#include "Factory.h"

namespace UnitTests
{
	class AttributedFoo : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);

	public:
		static const std::size_t ArraySize = 5; 

		AttributedFoo();
		AttributedFoo(const AttributedFoo&) = default;
		AttributedFoo(AttributedFoo&&) = default;
		AttributedFoo& operator=(const AttributedFoo&) = default;
		AttributedFoo& operator=(AttributedFoo&&) = default;
		virtual ~AttributedFoo() = default;

		int ExternalInteger = 0;
		float ExternalFloat = 0.0f; 
		std::string ExternalString = ""s;
		glm::vec4 ExternalVector = glm::vec4(0);
		glm::mat4 ExternalMatrix = glm::mat4(0);

		int ExternalIntegerArray[ArraySize] = { 0,0,0,0,0 };
		float ExternalFloatArray[ArraySize] = { 0.0f,0.0f,0.0f,0.0f, 0.0f };
		std::string ExternalStringArray[ArraySize] = {"a"s, "b"s, "c"s, "d"s, "e"s};
		glm::vec4 ExternalVectorArray[ArraySize] = { glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0), glm::vec4(0) };
		glm::mat4 ExternalMatrixArray[ArraySize] = { glm::mat4(0), glm::mat4(0), glm::mat4(0), glm::mat4(0), glm::mat4(0) };

		Scope* Clone() const override;

		bool Equals(const RTTI* rhs) const;
		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	protected:
		AttributedFoo(RTTI::IdType typeId) : Attributed(typeId) { }; // Forwarding call
	};

	CONCRETE_FACTORY(AttributedFoo, FieaGameEngine::Scope);
}