#pragma once

#include "hvn3/rooms/SceneBase.h"

#include <functional>

namespace hvn3 {

	class Scene :
		public SceneBase {

	public:
		Scene(std::function<void(CreateEventArgs&)>&& onCreate);
		
		void OnCreate(CreateEventArgs& e) override;

	private:
		std::function<void(CreateEventArgs&)> _on_create;

	};

}