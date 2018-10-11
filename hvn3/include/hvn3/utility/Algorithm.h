#pragma once

#include "hvn3/math/Line.h"

#include <functional>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <vector>

namespace hvn3 {

	// Moves matching elements to the ends of both containers and returns a pair of iterators one past the last elements. Not guaranteed to preserve element ordering.
	template<typename Iter1, typename Iter2>
	std::pair<Iter1, Iter2> RemoveSame(Iter1 begin_1, Iter1 end_1, Iter2 begin_2, Iter2 end_2, std::function<bool(const typename std::iterator_traits<Iter1>::value_type&, const typename std::iterator_traits<Iter1>::value_type&)> comparer) {

		bool found = false;

		for (auto i = begin_1; i != end_1;) {

			for (auto j = begin_2; j != end_2; ++j)
				if (comparer(*i, *j)) {
					*i = std::move(*--end_1);
					*j = std::move(*--end_2);
					found = true;
					break;
				}

			if (begin_2 == end_2)
				break;

			if (found) {
				found = false;
				continue;
			}

			++i;

		}

		return std::make_pair(end_1, end_2);

	}

	// Moves matching elements to the ends of both containers and returns a pair of iterators one past the last elements. Not guaranteed to preserve element ordering.
	template<typename Iter1, typename Iter2>
	std::pair<Iter1, Iter2> RemoveSame(Iter1 begin_1, Iter1 end_1, Iter2 begin_2, Iter2 end_2) {

		return RemoveSame(begin_1, end_1, begin_2, end_2, [](const typename Iter1::value_type& x, const typename Iter2::value_type& y) {return x == y; });

	}

	// Sorts a collection according to the order of elements in the second collection.
	template <typename IterBegin, typename IterEnd, typename IterOrderBegin, typename IterOrderEnd>
	void RelativeSort(IterBegin in_begin, IterEnd in_end, IterOrderBegin order_begin, IterOrderEnd order_end) {

		using key_type = typename IterBegin::value_type;

		std::unordered_map<key_type, size_t> order_map;
		size_t i = 0;

		for (auto it = order_begin; it != order_end; ++it, ++i)
			order_map[*it] = i;

		std::sort(in_begin, in_end, [&](key_type lhs, key_type rhs) {
			auto it_lhs = order_map.find(lhs);
			if (it_lhs == order_map.end())
				it_lhs = order_map.emplace(std::make_pair(lhs, ++i)).first;
			auto it_rhs = order_map.find(rhs);
			if (it_rhs == order_map.end())
				it_rhs = order_map.emplace(std::make_pair(rhs, ++i)).first;
			return it_lhs->second < it_rhs->second;
		});

	}

	// Increments the given value, wrapping around to a minimal value if enabled, and returns the result.
	template <typename T>
	T NextIncrement(T value, bool wrap) {

		if (value < std::numeric_limits<T>::max())
			// If we haven't reached the maximum value yet, increment.
			return ++value;
		else if (wrap)
			// If wrapping is allowed, wrap back to the minimum value.
			return std::numeric_limits<T>::min();
		else
			// Otherwise, the value cannot be incremented.
			return value;

	}

	// Increments the given value until the condition returns true, wrapping around to a minimal value if enabled, and returns the result. If the condition cannot be satisfied, returns the starting value.
	template <typename T, typename ConditionialFunc>
	T NextIncrementIf(T value, bool wrap, const ConditionialFunc& condition) {

		// Get the next value.
		T out = NextIncrement(value, wrap);

		// If the new value meets the condition, exit here.
		if (condition(out))
			return out;

		// Otherwise, continue incrementing the value until it meets the condition or maxes out.
		while (++out <= std::numeric_limits<T>::max())
			if (condition(out))
				return out;

		// If wrapping is allowed, we can wrap around and test all values < out to see if any meet the condition.
		if (wrap) {
			T max = out;
			out = std::numeric_limits<T>::min();
			while (out++ < max)
				if (condition(out))
					return out;
		}

		// The condition could not be satisfied, so return the input value.
		return value;

	}

	// Uses Bresenham's line algorithm to plot a line using the given callback. Points are ordered from the start of the line to the end.
	void BresenhamLineAlgorithm(float x1, float y1, float x2, float y2, const std::function<void(int, int)>& plot) {

		bool swap = false;

		// Because the loop uses x, if x doesn't vary, the loop won't execute even when y does vary.
		// As well, if the line is steep (change in y is greater than the change in x), we need use y for the loop instead to get all points.
		// To solve this, swap the components, and plot (x, y) as (y, x) instead.

		if (std::abs(x2 - x1) < 1.0f || (std::abs(y2 - y1) > std::abs(x2 - x1))) {

			std::swap(x1, y1);
			std::swap(x2, y2);

			swap = true;

		}

		float delta_x = x2 - x1;
		float delta_y = y2 - y1;
		float delta_err = std::abs(delta_y / delta_x);
		float err = 0.0f;

		int y = static_cast<int>(std::round(y1));

		// Notice that the loop goes in reverse by decrementing x if x2 < x1.
		// Some implementations would instead trace the line in the opposite direction, which plots the same points, but in the opposite order.
		// This approach is used instead to keep the points in the correct order.

		int x_inc = x2 < x1 ? -1 : 1;

		for (int x = static_cast<int>(x1); x_inc > 0 ? (x <= x2) : (x >= x2); x += x_inc) {

			if (swap)
				plot(y, x);
			else
				plot(x, y);

			err += delta_err;

			if (err >= 0.5f) {

				y += delta_y < 0.0f ? -1 : 1;
				err -= 1.0f;

			}

		}

	}
	void BresenhamLineAlgorithm(const LineF& line, const std::function<void(int, int)>& plot) {
		BresenhamLineAlgorithm(line.X1(), line.Y1(), line.X2(), line.Y2(), plot);
	}
	void BresenhamLineAlgorithm(const PointF& start, const PointF& end, const std::function<void(int, int)>& plot) {
		BresenhamLineAlgorithm(start.x, start.y, end.x, end.y, plot);
	}


}