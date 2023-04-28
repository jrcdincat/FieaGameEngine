#include "pch.h"
#include "AttributedBar.h"
#include "Datum.h"


using namespace FieaGameEngine;
using DatumTypes = Datum::DatumTypes;

namespace UnitTests
{
	RTTI_DEFINITIONS(AttributedBar);

	AttributedBar::AttributedBar() :
		AttributedFoo(AttributedBar::TypeIdClass())
	{
	}

	Scope* AttributedBar::Clone() const
	{
		return gsl::owner<AttributedBar*>(new AttributedBar(*this));
	}

	Vector<Signature> AttributedBar::Signatures()
	{
		return Vector<Signature>
		{
			{ "ExternalBarInteger", DatumTypes::Integer, 1, offsetof(AttributedBar, ExternalBarInteger) },
			{ "ExternalBarFloat",  DatumTypes::Float, 1, offsetof(AttributedBar, ExternalBarFloat) },
			{ "ExternalBarString", DatumTypes::String, 1, offsetof(AttributedBar, ExternalBarString) },
		};
	}
}