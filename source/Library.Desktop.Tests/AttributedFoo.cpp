#include "pch.h"
#include "AttributedFoo.h"
#include "Datum.h"


using namespace FieaGameEngine;
using DatumTypes = Datum::DatumTypes;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	Scope* AttributedFoo::Clone() const
	{
		return gsl::owner<AttributedFoo*>(new AttributedFoo(*this));
	}

	bool AttributedFoo::Equals(const RTTI* rhs) const
	{
		const auto rhsFoo = rhs->As<AttributedFoo>();
		if (rhsFoo == nullptr)
		{
			return false;
		}

		return ExternalInteger == rhsFoo->ExternalInteger &&
			ExternalFloat == rhsFoo->ExternalFloat &&
			ExternalString == rhsFoo->ExternalString &&
			ExternalVector == rhsFoo->ExternalVector &&
			ExternalMatrix == rhsFoo->ExternalMatrix &&
			std::equal(std::begin(ExternalIntegerArray), std::end(ExternalIntegerArray), std::begin(rhsFoo->ExternalIntegerArray)) &&
			std::equal(std::begin(ExternalFloatArray), std::end(ExternalFloatArray), std::begin(rhsFoo->ExternalFloatArray)) &&
			std::equal(std::begin(ExternalStringArray), std::end(ExternalStringArray), std::begin(rhsFoo->ExternalStringArray)) &&
			std::equal(std::begin(ExternalVectorArray), std::end(ExternalVectorArray), std::begin(rhsFoo->ExternalVectorArray)) &&
			std::equal(std::begin(ExternalMatrixArray), std::end(ExternalMatrixArray), std::begin(rhsFoo->ExternalMatrixArray)) &&
			(*this).At("NestedScope"s) == (*rhsFoo).At("NestedScope"s);
	}


	Vector<Signature> AttributedFoo::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat",  DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) }, 
			{ "ExternalVector", DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector) }, 
			{ "ExternalMatrix", DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", DatumTypes::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) }, 
			{ "ExternalFloatArray", DatumTypes::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) }, 
			{ "ExternalStringArray", DatumTypes::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray)},
			{ "ExternalVectorArray", DatumTypes::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) }, 
			{ "ExternalMatrixArray", DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) }, 
			{ "NestedScope", DatumTypes::Table, 0, 0 },
			{ "NestedScopeArray", DatumTypes::Table, ArraySize, 0 }
		};
	}
}