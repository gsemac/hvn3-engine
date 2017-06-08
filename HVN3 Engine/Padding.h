#pragma once

namespace hvn3 {

	class Padding {

	public:
		Padding();
		Padding(float all);
		Padding(float left, float right, float top, float bottom);

		float Left() const;
		float Right() const;
		float Top() const;
		float Bottom() const;

	private:
		float _left;
		float _right;
		float _top;
		float _bottom;

	};

}