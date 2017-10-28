#include "GuiExtension.h"
#include <iostream>

namespace hvn3 {

	namespace Gui {

		void GUIExtension::Initialize() {

			std::cout << "extension initialized!\n";

		}
		void GUIExtension::Update(UpdateEventArgs& e) {



		}
		void GUIExtension::Shutdown() {

			std::cout << "extension updating!\n";


		}

	}

}