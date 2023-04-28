#include "pch.h"
#include "ActionEvent.h"
#include "Datum.h"
#include <memory>
#include "Event.h"
#include "EventMessageAttributed.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
	using DatumTypes = Datum::DatumTypes;
	using QueueEntry = FieaGameEngine::EventQueue::QueueEntry;

	RTTI_DEFINITIONS(ActionEvent);
	ActionEvent::ActionEvent() : Action(TypeIdClass())
	{
	}

	void ActionEvent::Update(const GameTime& gameTime)
	{
		// Create an attributed event, assign its world and subtype, copy all
		// the auxiliary attributes into the event and queue the event with the given 
		// delay
		std::shared_ptr<EventMessageAttributed> message = std::make_shared<EventMessageAttributed>();
		std::shared_ptr<Event<EventMessageAttributed>> attributedEvent = std::make_shared<Event<EventMessageAttributed>>(*message);
		EventMessageAttributed& attributedMessage = attributedEvent->Message();

		attributedMessage.SetGameState(*GameState::Instance());
		attributedMessage.SetSubType(SubType);
		auto [first, last] = AuxiliaryAttributes();
		for (size_type index = first; index < last; ++index)
		{
			const std::string& key = GetKey(index);
			Datum& attribute = attributedMessage.AppendAuxiliaryAttribute(key);
			Datum datum = At(key);
			attribute = std::move(datum);
		}

		std::shared_ptr<QueueEntry>queueEntry = std::make_shared<QueueEntry>(attributedEvent, gameTime, milliseconds(Delay));
		GameState::Instance()->GetEventQueue()->Enqueue(queueEntry);
	}

	void ActionEvent::SetSubtype(const std::string& subType)
	{
		SubType = subType;
	
	}

	const uint32_t ActionEvent::GetDelay()
	{
		return Delay;
	}

	Vector<Signature> ActionEvent::Signatures()
	{
		return Vector<Signature>
		{
			{ "SubType", DatumTypes::String, 1, offsetof(ActionEvent, SubType) },
			{ "Delay", DatumTypes::Integer, 1, offsetof(ActionEvent, Delay) },
		};
	}
}