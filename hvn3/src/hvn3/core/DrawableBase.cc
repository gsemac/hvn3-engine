#include "hvn3/core/DrawableBase.h"

namespace hvn3 {

	// DrawableBase

	// Public methods

	DrawableBase::DrawableBase() :
		_depth(0) {}
	void DrawableBase::OnDraw(DrawEventArgs& e) {}
	DrawableBase::depth_type DrawableBase::Depth() const {

		return _depth;

	}
	void DrawableBase::SetDepth(depth_type value) {

		_depth = value;

	}

}