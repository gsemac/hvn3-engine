#pragma once

#include "hvn3/exceptions/Exception.h"

namespace hvn3 {
	namespace xml {

		class XmlException :
			public Exception {

			using Exception::Exception;

		};

	}
}