#pragma once

#include "hvn3/iterators/transform_iterator.h"

#include <utility>

namespace hvn3 {

	template<typename Iter, typename UnaryFunction>
	class StashingTransformIterator :
		public TransformIterator<Iter, UnaryFunction> {

		using stashed_type = typename std::remove_reference<reference>::type;

	public:
		using TransformIterator::TransformIterator;

		typename stashed_type* operator->() {

			stashed = **this;

			return &stashed;

		}
		const typename stashed_type* operator->() const {

			stashed = **this;

			return &stashed;

		}

	private:
		typename stashed_type stashed;

	};

	template<typename Iter, typename UnaryFunction>
	StashingTransformIterator<Iter, UnaryFunction> make_stashing_transform_iterator(Iter iterator, UnaryFunction transformFunction) {

		return StashingTransformIterator<Iter, UnaryFunction>(std::forward<Iter>(iterator), std::forward<UnaryFunction>(transformFunction));

	}

}