#pragma once
#include "hvn3/core/Context.h"

// Ideally, the "SetContext" method should not be public (only certain classes have business accessing it).

namespace hvn3 {
	namespace System {
	
		class IContextReceiver {

		public:
			virtual void SetContext(hvn3::Context context) = 0;

		};
	
	}
}