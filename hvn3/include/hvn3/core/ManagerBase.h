#pragma once

#include "hvn3/core/IManager.h"

#include <type_traits>

namespace hvn3 {

	namespace implementation {

		class ManagerBaseNullInterfaceType {};

	}

	template<typename InterfaceType = implementation::ManagerBaseNullInterfaceType>
	class ManagerBase :
		public IManager,
		public InterfaceType {

	public:
		typedef InterfaceType interface;
		typedef implementation::ManagerBaseNullInterfaceType null_interface;

		void OnStart(StartEventArgs& e) override {}
		void OnEnd(EndEventArgs& e) override {}

	};

}