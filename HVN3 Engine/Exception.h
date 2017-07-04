#pragma once
#include <stdexcept>
#include <string>

namespace hvn3 {

	class Exception : public std::runtime_error {

	public:
		Exception();
		Exception(const char* message);
		Exception(const std::string& message);

		virtual const char* what() const throw();
		virtual std::string Message() const;

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