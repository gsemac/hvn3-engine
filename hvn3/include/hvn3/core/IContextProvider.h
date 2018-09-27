#pragma once
#include "hvn3/core/Context.h"
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/core/ManagerDefs.h"

namespace hvn3 {

	class Context;

	class IContextProvider {

	protected:
		virtual void GiveContext(IContextReceiver& receiver, const Context& context) {

			ContextChangedEventArgs args(context);

			receiver.OnContextChanged(args);

		}

	};

}