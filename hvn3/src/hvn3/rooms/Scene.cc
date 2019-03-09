#include "hvn3/rooms/Scene.h"

namespace hvn3 {

	Scene::Scene(std::function<void(EnterEventArgs&)>&& onEnter) :
		_on_enter(onEnter) {
	}

}