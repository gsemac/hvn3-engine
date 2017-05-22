#pragma once

namespace hvn3 {

	class DrawEventArgs;

	class IDrawable {

	public:
		virtual void Draw(DrawEventArgs& e) = 0;

	};

}