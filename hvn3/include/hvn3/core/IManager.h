#pragma once

#include "hvn3/core/IApplicationContextReceiver.h"

namespace hvn3 {

	class IManager :
		public IApplicationContextReceiver {

	public:
		virtual ~IManager() = default;

		// Returns true if the manager state is able to be suspended.
		virtual bool IsSuspendable() const = 0;

	protected:
		// Returns the application context assigned to this object.
		virtual ApplicationContext Context() = 0;

	};

}