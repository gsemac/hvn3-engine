#pragma once
#include <stdexcept>
#include <string>

namespace hvn3 {
	namespace System {

		class IException {
		public:
			virtual const char* what() const throw() = 0;
			virtual std::string Message() const = 0;
		};

		template <typename STD_EXCEPTION>
		class ExceptionBase : public IException, public STD_EXCEPTION {
		public:
			ExceptionBase() : STD_EXCEPTION("") {}
			ExceptionBase(const char* message) : STD_EXCEPTION(message) {}
			ExceptionBase(const std::string& message) : STD_EXCEPTION(message.c_str()) {}
			virtual const char* what() const throw() override {
				return STD_EXCEPTION::what();
			}
			virtual std::string Message() const override {
				return std::string(what());
			}
		};

		typedef ExceptionBase<std::runtime_error> Exception;

		class ArgumentException : public ExceptionBase<std::invalid_argument> {
		public:
			using ExceptionBase::ExceptionBase;
			ArgumentException();
		};

		class InvalidOperationException : public Exception {
		public:
			using Exception::Exception;
			InvalidOperationException();
		};

		class NotSupportedException : public Exception {
		public:
			using Exception::Exception;
			NotSupportedException();
		};

		class NotImplementedException : public Exception {
		public:
			using Exception::Exception;
			NotImplementedException();
		};

		class FrameworkException : public Exception {
		public:
			using Exception::Exception;
			FrameworkException();
		};

		class KeyNotFoundException : public Exception {
		public:
			using Exception::Exception;
			KeyNotFoundException();
		};

		class NullPointerException : public Exception {
		public:
			using Exception::Exception;
			NullPointerException();
		};

		class NullReferenceException : public Exception {
		public:
			using Exception::Exception;
			NullReferenceException();
		};

		namespace IO {

			class IOException : public Exception {
			public:
				using Exception::Exception;
				IOException();
			};

			class FileNotFoundException : public IOException {
			public:
				using IOException::IOException;
				FileNotFoundException();
			};

		}

	}
}