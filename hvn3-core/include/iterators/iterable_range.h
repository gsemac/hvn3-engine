#pragma once

namespace hvn3::iterators {

	template<typename Iter>
	class IterableRange {

	public:
		template<typename Iter>
		IterableRange(Iter begin, Iter end) :
			_begin(begin),
			_end(end) {
		}

		Iter begin() {

			return _begin;

		}
		Iter end() {

			return _end;

		}

	private:
		Iter _begin;
		Iter _end;

	};

	template<typename Iter>
	IterableRange<Iter> make_iterable_range(Iter begin, Iter end) {

		return IterableRange<Iter>(begin, end);

	}

}