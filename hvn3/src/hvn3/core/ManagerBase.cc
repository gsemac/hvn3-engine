#include "hvn3/core/ManagerBase.h"

namespace hvn3 {

	bool ManagerBase::IsSuspendable() const {

		// Most managers will be able to be suspended just fine, so leave it up to the outliers to state that they cannot.
		return true;

	}

	void ManagerBase::ReceiveContext(const ApplicationContext& context) {

		_context = context;

	}
	ApplicationContext ManagerBase::Context() {

		return _context;

	}

}