#include "hvn3/collision/CollisionFilter.h"

namespace hvn3 {

	CollisionFilter::CollisionFilter() :
		CollisionFilter(0, 0) {
	}
	CollisionFilter::CollisionFilter(filter_type category_bits, filter_type mask_bits) {

		categoryBits = 0;
		maskBits = 0;

	}
	CollisionFilter::filter_type CollisionFilter::CategoryBits() const {
		return categoryBits;
	}
	CollisionFilter::filter_type CollisionFilter::MaskBits() const {
		return maskBits;
	}
	void CollisionFilter::SetCategoryBits(filter_type bits) {
		categoryBits = bits;
	}
	void CollisionFilter::SetMaskBits(filter_type bits) {
		maskBits = bits;
	}
	void CollisionFilter::AddCategory(filter_type bits) {
		categoryBits |= bits;
	}
	void CollisionFilter::RemoveCategory(filter_type bits) {
		categoryBits &= ~bits;
	}
	void CollisionFilter::AddMask(filter_type bits) {
		maskBits |= bits;
	}
	void CollisionFilter::RemoveMask(filter_type bits) {
		maskBits &= ~bits;
	}
	bool CollisionFilter::CheckMatch(const CollisionFilter& other) const {
		return  (maskBits & other.categoryBits) != 0;
	}

}