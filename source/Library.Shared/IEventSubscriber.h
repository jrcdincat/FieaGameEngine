#pragma once


namespace FieaGameEngine
{
	class EventPublisher;

	/// <summary>
	/// Abstract Base Event Subscriber
	/// </summary>
	class IEventSubscriber
	{
	public:
		/// <summary>
		/// Notify to subscribers
		/// </summary>
		/// <param name=""> - Event</param>
		virtual void Notify(EventPublisher&) = 0;

		/// <summary>
		/// Defaulted virtual destructor
		/// </summary>
		virtual ~IEventSubscriber() = default;
	};
}