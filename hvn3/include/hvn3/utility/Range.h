#pragma once
#include <algorithm>
#include <cassert>

namespace hvn3 {

	template <typename T>
	class Range {

	public:
		Range() :
			Range(true) {
		}
		Range(bool inclusive) {

			_start = 0;
			_end = _start;
			_inclusive = inclusive;

		}
		Range(T start, T end, bool inclusive = true) {

			Resize(start, end);
			_inclusive = inclusive;

		}

		T Start() const {
			return _start;
		}
		T End() const {
			return _end;
		}
		T Min() {

			return (_start < _end) ? _start : _end;

		}
		T Max() {

			return (_end > _start) ? _end : _start;

		}
		int Length() const {
			return (std::abs)(_end - _start);
		}

		bool Overlaps(const Range<T>& other) const {

			return (other._start >= _start && other._start <= _end + 1) ||
				(other._start <= _start && other._end + 1 >= _start);

		}
		Range<T>& Merge(const Range<T>& other) {

			_start = (std::min)(_start, other._start);
			_end = (std::max)(_end, other._end);

			return *this;

		}

		void Set(T start, T end) {
			_start = start;
			_end = end;
		}
		void SetLower(T value) {

			if (_end < _start)
				_end = value;
			else
				_start = value;

		}
		void SetUpper(T value) {

			if (_end > _start)
				_end = value;
			else
				_end = value;

		}
		void SetStart(T value) {

			_start = value;

		}
		void SetEnd(T value) {

			_end = value;

		}

		bool IsEmpty() const {

			return Length() == 0;

		}
		bool IsAscending() const {

			return _start < _end;

		}
		bool IsDescending() const {

			return _start > _end;

		}
		bool IsInclusive() const {
			return _inclusive;
		}
		bool ContainsValue(const T& value) {

			return (value >= Min() + !_inclusive && value <= Max() - !_inclusive);

		}
		bool ContainsRange(const Range<T>& other) {

			return ContainsValue(other.Min()) && ContainsValue(other.Max());

		}

		void MakeAscending() {

			if (_start > _end)
				std::swap(_start, _end);

		}
		void MakeDescending() {

			if (_start < _end)
				std::swap(_start, _end);

		}

	private:
		T _start, _end;
		bool _inclusive;

	};

}