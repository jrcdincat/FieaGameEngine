#pragma once
#include "IEventSubscriber.h"
#include "EventPublisher.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Event that inherits from EventPublisher
	/// </summary>
	/// <typeparam name="T"> - Message type</typeparam>
	template <typename T>
	class Event final : public EventPublisher
	{
		using size_type = size_t;

		RTTI_DECLARATIONS(Event, EventPublisher);
	public:
		/// <summary>
		/// Delete default constructor
		/// </summary>
		Event() = delete;

		/// <summary>
		/// Constructor that takes in a templated message
		/// </summary>
		/// <param name="message"> - message</param>
		Event(T message);

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="">Event to copy</param>
		Event(const Event&) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="">Event to move</param>
		/// <returns>Moved Event</returns>
		Event(Event&&) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="">Event to copy</param>
		/// <returns>Reference to Event copy</returns>
		Event& operator=(const Event&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="">Event to move</param>
		/// <returns>Reference to moved Event</returns>
		Event& operator=(Event&&) noexcept = default;

		/// <summary>
		/// Defaulted Event destructor
		/// </summary>
		~Event() override = default;

		/// <summary>
		/// Subscribe Event to subscriber
		/// </summary>
		/// <param name="">Subscriber for this event</param>
		inline static void Subscribe(IEventSubscriber&);
		
		/// <summary>
		/// Unsubscribe from subscriber
		/// </summary>
		/// <param name="">Subscriber to unsubscribe from</param>
		inline static void UnSubscribe(IEventSubscriber&);
		
		/// <summary>
		/// Unsubscribes all subscribers
		/// </summary>
		inline static void UnSubscribeAll();
		
		/// <summary>
		/// Returns the message
		/// </summary>
		/// <returns>Message reference</returns>
		T& Message();
	
		/// <summary>
		/// Returns the number of subscribers this event is subscribed to
		/// </summary>
		/// <returns>Number of subscribers</returns>
		inline const size_type Size() const;

	private:


		/// <summary>
		/// Message event sends
		/// </summary>
		T _message;

		/// <summary>
		/// List of subscribers this type of event is subscribed to.
		/// </summary>
		inline static Vector<IEventSubscriber*> _subscribers{ 100 };

		/// <summary>
		/// Constructor to pass RTTI typeId
		/// </summary>
		/// <param name="typeId">- TypeId</param>
		explicit Event(RTTI::IdType typeId) : EventPublisher(typeId) { };
	};
}
#include "Event.inl"