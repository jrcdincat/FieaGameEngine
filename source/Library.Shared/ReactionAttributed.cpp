#include "pch.h"
#include "ReactionAttributed.h"
#include "Datum.h"
#include "Event.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ReactionAttributed);
	using DatumTypes = Datum::DatumTypes;

	ReactionAttributed::ReactionAttributed() : Reaction(TypeIdClass())
	{
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	void ReactionAttributed::Notify(EventPublisher& attributedEvent)
	{
		Event<EventMessageAttributed>* eventAttributed = attributedEvent.As<Event<EventMessageAttributed>>();
		assert(eventAttributed != nullptr);
		EventMessageAttributed& message = eventAttributed->Message();

		if (SubType == message.GetSubType())
		{
			// Copy auxiliary attributes from event message attributed into this reaction attributed
			auto [first, last] = message.AuxiliaryAttributes();
			for (size_type index = first; index < last; ++index)
			{
				const std::string& key = message.GetKey(index);
				
				Datum& attribute = AppendAuxiliaryAttribute(key);
				Datum datum = message.At(key);
				attribute = std::move(datum);
			}
			ActionList::Update(GameState::Instance()->GetGameTime());
		}
	}

	void ReactionAttributed::Update(const GameTime&)
	{
	}

	void ReactionAttributed::SetSubType(const std::string& subType)
	{
		SubType = subType;
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> ReactionAttributed::Signatures()
	{
		return Vector<Signature>
		{
			{ "SubType", DatumTypes::String, 1, offsetof(ReactionAttributed, SubType) },
		};
	}
}