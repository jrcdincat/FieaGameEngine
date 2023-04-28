#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction);

	Reaction::Reaction(RTTI::IdType typeId) : ActionList(typeId)
	{
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> Reaction::Signatures()
	{
		return Vector<Signature>
		{
		};
	}
}