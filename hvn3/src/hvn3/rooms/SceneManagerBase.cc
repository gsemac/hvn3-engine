#include "hvn3/rooms/SceneManagerBase.h"
#include "hvn3/rooms/SceneTransitionFade.h"

#include <memory>

namespace hvn3 {

	// SceneManagerBase

	// Public methods

	SceneManagerBase::SceneManagerBase() :
		_depth(0) {}
	bool SceneManagerBase::IsSuspendable() const {

		// The scene manager itself should not be suspended when saving a scene state.
		return false;

	}

	// Protected methods

	void SceneManagerBase::ReceiveContext(const ApplicationContext& context) {

		_context = context;

	}
	SceneManagerBase::depth_type SceneManagerBase::SceneManagerBase::Depth() const {

		return _depth;

	}
	void SceneManagerBase::SetDepth(depth_type value) {

		_depth = value;

	}
	void SceneManagerBase::OnDraw(DrawEventArgs& e) {}
	void SceneManagerBase::OnUpdate(UpdateEventArgs& e) {}
	ApplicationContext SceneManagerBase::Context() {

		return _context;

	}


}