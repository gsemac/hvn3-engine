#pragma once
#include "hvn3/core/IContextReceiver.h"

namespace hvn3 {

	class IManager :
		public IContextReceiver {

	public:
		virtual ~IManager() = default;

	};

}