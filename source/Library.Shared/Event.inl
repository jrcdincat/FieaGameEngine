#pragma once
#include "Event.h"

namespace FieaGameEngine
{
	template<typename T>
	RTTI_DEFINITIONS(Event<T>);

	template<typename T>
	inline Event<T>::Event(T message) : EventPublisher{_subscribers}, _message { std::move(message) }
	{
	}

	template<typename T>
	inline void Event<T>::Subscribe(IEventSubscriber& eventSubscriber)
	{
		_subscribers.push_back(&eventSubscriber);
	}

	template<typename T>
	inline void Event<T>::UnSubscribe(IEventSubscriber& eventSubscriber)
	{
		_subscribers.Remove(&eventSubscriber);
	}

	template<typename T>
	inline void Event<T>::UnSubscribeAll()
	{
		_subscribers.Clear();
	}
	template<typename T>
	inline T& Event<T>::Message()
	{
		return _message;
	}

	template<typename T>
	inline const Event<T>::size_type Event<T>::Size() const
	{
		return _subscribers.size();
	}
}