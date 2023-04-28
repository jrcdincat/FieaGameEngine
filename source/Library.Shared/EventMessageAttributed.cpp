#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed);

	EventMessageAttributed::EventMessageAttributed() : Attributed(TypeIdClass())
	{

	}

	void EventMessageAttributed::SetGameState(GameState& gameState)
	{
		_root = &gameState;
	}

	const std::string& EventMessageAttributed::GetSubType() const
	{
		return _subType;
	}

	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> EventMessageAttributed::Signatures()
	{
		return Vector<Signature>
		{
		};
	}
}