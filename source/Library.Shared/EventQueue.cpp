#include "pch.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
    void EventQueue::Enqueue(std::shared_ptr<QueueEntry> entry)
    {
        _pendingEnqueue.push_back(entry);
        // Enqueue after optional delay time
    }

    void EventQueue::Update(const GameTime& gameTime)
    {
        // Add to queue from pending enqueue 
        for (size_t index = 0; index < _pendingEnqueue.size(); ++index)
        {
            std::shared_ptr<QueueEntry> entry = _pendingEnqueue.back();
            if (gameTime.CurrentTime() >= entry->_expirationTime)
            {
                _queue.push_back(entry);
            }
        }
        _pendingEnqueue.Clear();

        // Publish events
        for (auto entry : _queue)
        {
            if (entry->IsExpired(gameTime))
            {
                entry->_event->Deliver();

                // Check whether event should be deleted after publishing
                if (entry->_deleteAfterPublishing)
                {
                    _pendingDequeue.push_back(entry);
                }
            }
        }

        for (size_t index = 0; index < _pendingDequeue.size(); ++index)
        {
            _queue.Remove(_pendingDequeue[index]);
        }

        // Dequeue from pending dequeue
        _pendingDequeue.Clear();
    }

    void EventQueue::Clear()
    {
        _pendingEnqueue.Clear();
        _pendingDequeue.Clear();
    }

    const bool EventQueue::IsEmpty() const
    {
        return _queue.IsEmpty();
    }

    const size_type EventQueue::Size() const
    {
        return _queue.size();
    }

    const bool EventQueue::QueueEntry::IsExpired(const GameTime& gameTime) const
    {   
        return gameTime.CurrentTime() >= _expirationTime;
    }
}

