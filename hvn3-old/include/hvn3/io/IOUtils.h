#pragma once

#include "hvn3/events/EventSource.h"
#include "hvn3/io/IODefs.h"

#include <string>

namespace hvn3 {
	namespace io {

		class IOUtils {

		public:

			// Converts a file into a byte string that can be directly embedded in source code.
			static std::string FileToEmbeddableString(const std::string& filePath);

			static bool IsPrintableChar(int keyCode);
			static bool IsPrintableChar(Key key);
			static bool IsNumeric(Key key);

			static EventSource MouseEventSource();
			static EventSource KeyboardEventSource();

		};



	}
}