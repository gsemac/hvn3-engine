#include "CollisionFilter.h"

namespace hvn3 {

	CollisionFilter::CollisionFilter() {

		__category_bits = 0;
		__mask_bits = 0;

	}
	int CollisionFilter::CategoryBits() const {

		return __category_bits;

	}
	int CollisionFilter::MaskBits() const {

		return __mask_bits;

	}
	void CollisionFilter::SetCategoryBits(int bits) {

		__category_bits = bits;

	}
	void CollisionFilter::SetMaskBits(int bits) {

		__mask_bits = bits;

	}
	void CollisionFilter::AddCategory(int bits) {

		__category_bits |= bits;

	}
	void CollisionFilter::RemoveCategory(int bits) {

		__category_bits &= ~bits;

	}
	void CollisionFilter::AddMask(int bits) {

		__mask_bits |= bits;

	}
	void CollisionFilter::RemoveMask(int bits) {

		__mask_bits &= ~bits;

	}
	bool CollisionFilter::CheckMatch(const CollisionFilter& other) const {

		return  (__mask_bits & other.__category_bits) != 0;

	}

}