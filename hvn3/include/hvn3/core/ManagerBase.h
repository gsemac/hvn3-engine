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

		void OnEvent(StartEventArgs& e) override {

			OnStart(e);

		}
		void OnEvent(EndEventArgs& e) override {

			OnEnd(e);

		}

	protected:
		virtual void OnStart(StartEventArgs& e) {}
		virtual void OnEnd(EndEventArgs& e) {}

	};

}