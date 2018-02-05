#pragma once
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
namespace hvn3 {

	// Moves matching elements to the ends of both containers and returns a pair of iterators one past the last elements. Not guaranteed to preserve element ordering.
	template<typename Iter1, typename Iter2>
	std::pair<Iter1, Iter2> RemoveSame(Iter1 begin_1, Iter1 end_1, Iter2 begin_2, Iter2 end_2, std::function<bool(typename const Iter1::value_type&, typename const Iter2::value_type&)> comparer) {

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

		return RemoveSame(begin_1, end_1, begin_2, end_2, [](typename const Iter1::value_type& x, typename const Iter2::value_type& y) {return x == y; });
		
	}

	
}