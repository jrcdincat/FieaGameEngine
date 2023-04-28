#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "IEventSubscriber.h"
#include <chrono>

namespace FieaGameEngine
{
	/// <summary>
	/// Publisher that publishes the event message to subscribers
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);
	public:

		/// <summary>
		/// Defaulted constructor for Game State
		/// </summary>
		EventPublisher() = delete;

		/// <summary>
		/// Defaulted copy constructor
		/// </summary>
		/// <param name="">EventPublisher to copy</param>
		EventPublisher(const EventPublisher&) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="">EventPublisher to move</param>
		/// <returns>Moved EventPublisher</returns>
		EventPublisher(EventPublisher&&) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="">EventPublisher to copy</param>
		/// <returns>Reference to EventPublisher copy</returns>
		EventPublisher& operator=(const EventPublisher&) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="">EventPublisher to move</param>
		/// <returns>Reference to moved EventPublisher</returns>
		EventPublisher& operator=(EventPublisher&&) noexcept = default;

		/// <summary>
		/// Constructor that sets vector of subscribers
		/// </summary>
		/// <param name="subscribers"> - reference to vector of subscribers</param>
		EventPublisher(Vector<IEventSubscriber*>& subscribers);
		
		bool Equals(const RTTI*) const;

		/// <summary>
		/// Calls notify on all subscribers
		/// </summary>
		void Deliver();
		
		/// <summary>
		/// Defaulted virtual destructor
		/// </summary>
		virtual ~EventPublisher() = default;

	private: 
		/// <summary>
		/// Subscribers
		/// </summary>
		Vector<IEventSubscriber*>* _subscribers;
	};
}