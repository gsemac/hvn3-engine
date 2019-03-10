#include "hvn3/rooms/Scene.h"

namespace hvn3 {

	Scene::Scene(std::function<void(CreateEventArgs&)>&& onCreate) :
		_on_create(onCreate) {
	}
	void Scene::OnCreate(CreateEventArgs& e) {

		if (_on_create)
			_on_create(e);

	}

}