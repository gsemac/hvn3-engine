#pragma once
#include <utility>

namespace hvn3 {

	// Moves matching elements to the ends of both containers and returns a pair of iterators one past the last elements. Not guaranteed to preserve element ordering.
	template<typename Iter>
	std::pair<Iter, Iter> RemoveSameUnordered(Iter begin_1, Iter end_1, Iter begin_2, Iter end_2) {

		bool found = false;

		for (auto i = begin_1; i != end_1;) {

			for (auto j = begin_2; j != end_2; ++j)
				if (*i == *j) {
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

}