#pragma once

#include "hvn3/rooms/SceneBase.h"

#include <functional>

namespace hvn3 {

	class Scene :
		public SceneBase {

	public:
		Scene(std::function<void(EnterEventArgs&)>&& onEnter);

	private:
		std::function<void(EnterEventArgs&)> _on_enter;

	};

}