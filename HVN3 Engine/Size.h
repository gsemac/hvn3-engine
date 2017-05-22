#pragma once
#include <iostream>

namespace hvn3 {

	struct Size {

	private:
		float __width, __height;

	public:
		Size(float width, float height);

		float Width() const;
		float Height() const;
		void SetWidth(float value);
		void SetHeight(float value);
		void Resize(float width, float height);

	};

	bool operator<(const Size& a, const Size& b);
	bool operator>(const Size& a, const Size& b);
	bool operator<=(const Size& a, const Size& b);
	bool operator>=(const Size& a, const Size& b);
	bool operator==(const Size& a, const Size& b);
	std::ostream& operator<< (std::ostream& stream, const hvn3::Size& size);

}