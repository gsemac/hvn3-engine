#pragma once

#include <algorithm>
#include <iostream>

namespace hvn3 {

	class Scale;

	template <typename T>
	class Size {

	public:
		typedef T value_type;

		Size() :
			Size(0, 0) {
		}
		Size(T width, T height) {
			SetSize(width, height);
		}

		T Width() const {
			return width;
		}
		T Height() const {
			return height;
		}
		void SetWidth(T value) {
			SetSize(value, height);
		}
		void SetHeight(T value) {
			SetSize(width, value);
		}
		void SetSize(T width, T height) {

			this->width = width;
			this->height = height;

		}
		void SetSize(const Size<T>& value) {
			SetSize(value.width, value.height);
		}

		template <typename U>
		explicit operator Size<U>() const {
			return Size<U>(static_cast<U>(Width()), static_cast<U>(Height()));
		}

		static Size<T> Round(const Size<T>& value) {

			Size<T> out = value;

			out.width = (std::round)(out.width);
			out.height = (std::round)(out.height);

			return out;

		}
		static Size<T> Floor(const Size<T>& value) {

			Size<T> out = value;

			out.width = (std::floor)(out.width);
			out.height = (std::floor)(out.height);

			return out;

		}
		static Size<T> Ceiling(const Size<T>& value) {

			Size<T> out = value;

			out.width = (std::ceil)(out.width);
			out.height = (std::ceil)(out.height);

			return out;

		}

		T width;
		T height;

	};

	typedef Size<int> SizeI;
	typedef Size<float> SizeF;

	template <typename T>
	bool operator<(const Size<T>& a, const Size<T>& b) {

		return (a.Width() * b.Height() < b.Width() * b.Height());

	}
	template <typename T>
	bool operator>(const Size<T>& a, const Size<T>& b) {

		return (a.Width() * b.Height() > b.Width()* b.Height());

	}
	template <typename T>
	bool operator<=(const Size<T>& a, const Size<T>& b) {

		return (a < b || a == b);

	}
	template <typename T>
	bool operator>=(const Size<T>& a, const Size<T>& b) {

		return (a > b || a == b);

	}
	template <typename T>
	bool operator==(const Size<T>& a, const Size<T>& b) {

		return (a.Width() == b.Width() && a.Height() == b.Height());

	}
	template <typename T>
	bool operator!=(const Size<T>& lhs, const Size<T>& rhs) {
		return !(lhs == rhs);
	}

	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const Size<T>& size) {

		stream << size.Width() << 'x' << size.Height();

		return stream;

	}

}