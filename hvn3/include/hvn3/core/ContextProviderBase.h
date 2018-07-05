#include "hvn3/core/IContextProvider.h"
#include "hvn3/core/IContextReceiver.h"

namespace hvn3 {
	namespace System {

		class ContextProviderBase :
			public IContextProvider {

		public:
			void ProvideContext(IContextReceiver& receiver) override {
				
				hvn3::Context ctx = Context();

				//receiver.SetContext(ctx);
				receiver.OnContextChanged(ContextChangedEventArgs(ctx));

			}

		};

	}
}