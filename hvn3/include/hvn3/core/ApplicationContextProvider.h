#pragma once

#include "hvn3/core/IApplicationContextReceiver.h"
#include "hvn3/core/ManagerDefs.h"

namespace hvn3 {

	class ApplicationContext;

	class ApplicationContextProvider {

	protected:
		virtual void ProvideContext(IApplicationContextReceiver* receiver, const ApplicationContext& context) {

			receiver->ReceiveContext(context);

		}

	};

}