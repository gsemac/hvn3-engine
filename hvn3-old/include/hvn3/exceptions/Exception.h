#pragma once

#include "hvn3/exceptions/ExceptionBase.h"

namespace hvn3 {

	typedef ExceptionBase<> Exception;

	class ArgumentException :
		public ExceptionBase<std::invalid_argument> {

	public:
		using ExceptionBase::ExceptionBase;

		ArgumentException();

	};

	class InvalidOperationException :
		public Exception {


	public:
		using Exception::Exception;

		InvalidOperationException();

	};

	class NotSupportedException :
		public Exception {

	public:
		using Exception::Exception;

		NotSupportedException();
	};

	class NotImplementedException :
		public Exception {

	public:

		using Exception::Exception;

		NotImplementedException();

	};

	class FrameworkException : 
		public Exception {

	public:
		using Exception::Exception;

		FrameworkException();

	};

	class KeyNotFoundException : 
		public Exception {
	public:

		using Exception::Exception;

		KeyNotFoundException();

	};

	class NullPointerException : 
		public Exception {

	public:
		using Exception::Exception;

		NullPointerException();

	};

}