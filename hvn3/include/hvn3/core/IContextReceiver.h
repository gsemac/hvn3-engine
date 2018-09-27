#pragma once
#include "hvn3/events/EventArgs.h"

namespace hvn3 {

	class IContextProvider;
	class ContextChangedEventArgs;

	class IContextReceiver {

		friend class IContextProvider;

	public:
		virtual ~IContextReceiver() = default;

		virtual void OnContextChanged(ContextChangedEventArgs& e) {}

	};

}