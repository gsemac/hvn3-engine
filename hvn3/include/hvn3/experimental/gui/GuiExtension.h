#pragma once
#include "IExtension.h"

namespace hvn3 {

	namespace Gui {

		class GUIExtension : public IExtension {

		public:
			void Initialize() override;
			void Update(UpdateEventArgs& e) override;
			void Shutdown() override;

		private:

		};

	}

}