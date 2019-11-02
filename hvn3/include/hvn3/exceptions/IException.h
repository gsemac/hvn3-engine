#pragma once

#include <string>

namespace hvn3 {

	class IException {

	public:
		virtual const char* what() const throw() = 0;
		virtual const std::string Message() const = 0;

	};

}