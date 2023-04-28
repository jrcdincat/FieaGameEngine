#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement);

	using DatumTypes = Datum::DatumTypes;

	ActionIncrement::ActionIncrement() : Action(TypeIdClass())
	{
	}

	void ActionIncrement::Update(const GameTime&)
	{
		Datum* target = Search(Target);
		assert(target != nullptr);
		int& t = target->GetInteger(TargetIndex);
		t += Step;
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> ActionIncrement::Signatures()
	{
		return Vector<Signature>
		{
			{ "Target", DatumTypes::String , 1, offsetof(ActionIncrement, Target) },
			{ "Step", DatumTypes::Integer, 1, offsetof(ActionIncrement, Step)},
			{ "TargetIndex", DatumTypes::Integer, 1, offsetof(ActionIncrement, TargetIndex) }
		};
	}
}