#pragma once
#include "Factory.h"
#include <string>
#include <cstddef>
#include <memory>

// Concrete factory macro that defines a class that derives from a factory type.
#define CONCRETE_FACTORY(ConcreteProductType, AbstractProductType)																					\
	class ConcreteProductType##Factory : public FieaGameEngine::Factory<AbstractProductType>														\
	{																																				\
		public:																																		\
			[[nodiscard]] const std::string ClassName() const override { return std::string(#ConcreteProductType); }								\
			gsl::owner<ConcreteProductType*> Create() const override { return gsl::owner<ConcreteProductType*>(new ConcreteProductType); }			\
	}																																				\

