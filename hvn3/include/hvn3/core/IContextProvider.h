#pragma once
#include "hvn3/core/ApplicationContext.h"
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/core/ManagerDefs.h"

namespace hvn3 {

	class ApplicationContext;

	class IContextProvider {

	protected:
		virtual void GiveContext(IContextReceiver& receiver, const ApplicationContext& context) {

			ContextChangedEventArgs args(context);

			receiver.OnContextChanged(args);

		}

	};

}