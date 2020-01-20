#pragma once

namespace hvn3 {

	template<typename Iter>
	class IteratorRange {

	public:
		template<typename Iter>
		IteratorRange(Iter begin, Iter end) :
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
	IteratorRange<Iter> make_iterator_range(Iter begin, Iter end) {

		return IteratorRange<Iter>(begin, end);

	}

}