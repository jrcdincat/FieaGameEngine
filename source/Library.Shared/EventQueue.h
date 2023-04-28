#pragma once
#include "EventPublisher.h"
#include "GameTime.h"
#include <chrono>
#include <memory>

namespace FieaGameEngine
{
	using size_type = size_t;
	using clock = std::chrono::high_resolution_clock;
	using milliseconds = std::chrono::milliseconds;

	/// <summary>
	/// Queue of events
	/// </summary>
	class EventQueue
	{
	public:
		/// <summary>
		/// Entry that wraps an Event and time information
		/// </summary>
		struct QueueEntry
		{
			/// <summary>
			/// Event associated with this queue entry
			/// </summary>
			std::shared_ptr<EventPublisher> _event;

			/// <summary>
			/// Time point when event expires
			/// </summary>
			clock::time_point _expirationTime;

			/// <summary>
			/// Whether to delete entry after event is published
			/// </summary>
			bool _deleteAfterPublishing = false;

			/// <summary>
			/// Returns whether the event has expired
			/// </summary>
			/// <param name="gameTime"> - Game Time</param>
			/// <returns>True or False</returns>
			const bool IsExpired(const GameTime& gameTime) const;

			/// <summary>
			/// Constructor for queue entry for given event
			/// </summary>
			/// <param name="publisher"> - Event</param>
			QueueEntry(std::shared_ptr<EventPublisher> publisher, const GameTime& gameTime, milliseconds delay = milliseconds(0), bool deleteAfterPublishing = false) :
				_event{ publisher }, _deleteAfterPublishing(deleteAfterPublishing), _expirationTime{ gameTime.CurrentTime() + delay}
			{
			}
		};

		/// <summary>
		/// Sets Queue entry value and add the entry to the pending enqueue queue to later be added to the active queue
		/// </summary>
		/// <param name="gameTime"> - Game Time</param>
		/// <param name="entry"> - Queue entry</param>
		/// <param name="delay"> - delay until actually added to active queue</param>
		/// <param name="deleteAfterPublishing"> - whether to delete after publishing</param>
		void Enqueue(std::shared_ptr<QueueEntry> entry);

		/// <summary>
		/// Publish queued events that have expired. Do not call GameState::Update().
		/// </summary>
		/// <param name="gameTime"> - Game time</param>
		void Update(const GameTime& gameTime);

		/// <summary>
		/// Clears all queues
		/// </summary>
		void Clear();

		/// <summary>
		/// Return if queue of events is empty, excludes pending
		/// </summary>
		/// <returns>True or False</returns>
		const bool IsEmpty() const;

		/// <summary>
		/// Returns the size of the queue, excludes pending
		/// </summary>
		/// <returns>Size of queue</returns>
		const size_type Size() const;

	private:
		/// <summary>
		/// Queue actively being udpated
		/// </summary>
		Vector<std::shared_ptr<QueueEntry>> _queue;

		/// <summary>
		/// Pending queue awaiting to add to active queue
		/// </summary>
		Vector<std::shared_ptr<QueueEntry>> _pendingEnqueue;

		/// <summary>
		/// Pending queue awaiting to remove from active queue
		/// </summary>
		Vector<std::shared_ptr<QueueEntry>> _pendingDequeue;
	};
}