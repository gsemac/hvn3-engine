#pragma once

namespace hvn3 {

	class ApplicationContext;
	class ApplicationContextProvider;

	class IApplicationContextReceiver {

		friend class ApplicationContextProvider;

	public:
		virtual ~IApplicationContextReceiver() = default;

	protected:
		// Called by an IApplicationContextProvider to set the context for this object.
		virtual void ReceiveContext(const ApplicationContext& context) = 0;

	};

}