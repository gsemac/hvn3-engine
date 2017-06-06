#pragma once
#include <iostream>
#include "IPositionable.h"
#include "Hash.h"

namespace hvn3 {

	struct Point : public IPositionable {

	public:
		Point(float x = 0, float y = 0);

		bool IsEmpty() const;
		void Round();
		void Ceiling();
		void Truncate();

		friend bool operator==(const Point& a, const Point& b);
		friend bool operator!=(const Point& a, const Point& b);
		friend Point operator+(const Point& a, const Point& b);
		friend Point operator-(const Point& a, const Point& b);
		friend Point operator-(const Point& a);
		Point& operator+=(const Point& other);
		Point& operator-=(const Point& other);

	};

	std::ostream& operator<< (std::ostream& stream, const hvn3::Point& point);

}

namespace std {

	template <>
	struct hash<hvn3::Point> {

		std::size_t operator()(const hvn3::Point& point) const {

			size_t seed = 0;
			hash_combine(seed, point.X());
			hash_combine(seed, point.Y());
			return seed;

		}

	};

}