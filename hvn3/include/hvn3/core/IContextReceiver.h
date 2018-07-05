#pragma once
#include "hvn3/core/Context.h"
#include "hvn3/events/EventArgs.h"

/*
Classes that inherit from IContextReceiver are able to receive a context from an IContextProvider.

This interface provides a means of setting the current context, and a callback that is called every time the context is changed.
*/

namespace hvn3 {

	class ContextChangedEventArgs : public System::EventArgs {
	public:
		ContextChangedEventArgs(Context context) : _context(context) {}
		hvn3::Context Context() {
			return _context;
		}
	private:
		hvn3::Context _context;
	};

	namespace System {

		class ContextProviderBase;

		// Classes that are given a context from an IContextProvider should inherit from this interface.
		class IContextReceiver {

			friend class ContextProviderBase;

		protected:
			// This function is called immediately after SetContext. The event args object contains the new context.
			virtual void OnContextChanged(ContextChangedEventArgs& e) = 0;

		private:
			// Sets the context held by this object to the given context.
			//virtual void SetContext(Context context) = 0;

		};

	}
}