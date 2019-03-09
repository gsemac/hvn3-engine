#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/core/IManager.h"

namespace hvn3 {

	class ManagerBase :
		public IManager {

	public:
		bool IsSuspendable() const override;

	protected:
		void ReceiveContext(const ApplicationContext& context) override;
		ApplicationContext Context() override;

	private:
		ApplicationContext _context;

	};

}