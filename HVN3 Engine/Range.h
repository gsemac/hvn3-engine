#ifndef __RANGE_H
#define __RANGE_H
#include <cassert>
#include <algorithm>

namespace hvn3 {

	template <typename T>
	class Range {

	private:
		T __start, __end;
		bool __inclusive;

	public:
		Range(T start, T end, bool inclusive = true) {

			Resize(start, end);
			__inclusive = inclusive;

		}

		T Start() {

			return __start;

		}
		T End() {

			return __end;

		}
		bool Overlaps(const Range<T>& other) {

			return (other.__start >= __start && other.__start <= __end + 1) ||
				(other.__start <= __start && other.__end + 1 >= __start);

		}
		Range<T>& Merge(const Range<T>& other) {

			__start = (std::min)(__start, other.__start);
			__end = (std::max)(__end, other.__end);

			return *this;

		}
		void Resize(T start, T end) {

			// Set member variables.
			__start = start;
			__end = end;

		}
		void AssignToMin(T value) {

			if (__end < __start)
				__end = value;
			else
				__start = value;

		}
		void AssignToMax(T value) {

			if (__end > __start)
				__end = value;
			else
				__end = value;

		}
		void SetStart(T value) {

			__start = value;

		}
		void SetEnd(T value) {

			__end = value;

		}
		int Length() {

			return (std::abs)(__end - __start);

		}
		bool IsEmpty() {

			return Length() == 0;

		}
		bool IsAscending() {

			return __start < __end;

		}
		bool IsDescending() {

			return __start __end;

		}
		bool IsInclusive() {

			return __inclusive;

		}
		void Sort() {

			if (__start > __end)
				std::swap(__start, __end);

		}
		T Min() {

			return (__start < __end) ? __start : __end;

		}
		T Max() {

			return (__end > __start) ? __end : __start;

		}
		bool ContainsValue(const T& value) {

			return (value >= Min() + !__inclusive && value <= Max() - !__inclusive);

		}
		bool ContainsRange(const Range<T>& other) {

			return ContainsValue(other.Min()) && ContainsValue(other.Max());

		}

	};

}

#endif