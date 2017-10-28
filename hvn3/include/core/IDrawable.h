#pragma once

namespace hvn3 {

	class DrawEventArgs;

	class IDrawable {

	public:
		virtual void OnDraw(DrawEventArgs& e) = 0;

	};

}