#pragma once

#include "hvn3/core/IDrawable.h"

namespace hvn3 {

	class DrawableBase :
		public IDrawable {

	public:
		DrawableBase();

		void OnDraw(DrawEventArgs& e) override;

		depth_type Depth() const override;
		void SetDepth(depth_type value) override;

	private:
		depth_type _depth;

	};

}