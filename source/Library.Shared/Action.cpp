#include "pch.h"
#include "Action.h"
#include "Datum.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);

	using DatumTypes = Datum::DatumTypes;

	const std::string Action::Name() const
	{
		return _name;
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}

	Vector<Signature> Action::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", DatumTypes::String, 1, offsetof(Action, _name) },
		};
	}
}