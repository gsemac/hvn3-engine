#include "GUIExtension.h"
#include <iostream>

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