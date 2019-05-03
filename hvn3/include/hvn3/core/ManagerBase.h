#pragma once

#include "hvn3/core/ApplicationContext.h"
#include "hvn3/core/IManager.h"

namespace hvn3 {

	template<typename InterfaceType = void>
	class ManagerBase :
		public IManager,
		public InterfaceType {

	public:
		typedef InterfaceType interface;

		void OnStart(StartEventArgs& e) override {}
		void OnEnd(EndEventArgs& e) override {}

	};

}