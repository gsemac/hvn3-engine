#pragma once

#include "hvn3/exceptions/IException.h"

#include <stdexcept>

namespace hvn3 {

	template <typename BaseException = std::runtime_error>
	class ExceptionBase :
		public IException,
		public BaseException {

	public:

		ExceptionBase() : BaseException("") {}
		ExceptionBase(const char* message) : BaseException(message) {}
		ExceptionBase(const std::string& message) : BaseException(message.c_str()) {}

		virtual const char* what() const throw() override {
			return BaseException::what();
		}
		virtual const std::string Message() const override {
			return std::string(what());
		}
		const std::string ToString() const {

			return Message();

		}

	};

}