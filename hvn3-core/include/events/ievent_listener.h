#pragma once

namespace hvn3::events {

	class IEventDispatcher;

	class IEventListener {

	public:
		virtual ~IEventListener() = default;

		virtual void RegisterSubscription(IEventDispatcher& eventDispatcher) = 0;
		virtual void UnregisterSubscription(IEventDispatcher& eventDispatcher) = 0;

	};

}