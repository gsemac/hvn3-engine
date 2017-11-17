#include "collision/CollisionFilter.h"

namespace hvn3 {

		CollisionFilter::CollisionFilter() {

			_category_bits = 0;
			_mask_bits = 0;

		}
		int CollisionFilter::CategoryBits() const {

			return _category_bits;

		}
		int CollisionFilter::MaskBits() const {

			return _mask_bits;

		}
		void CollisionFilter::SetCategoryBits(int bits) {

			_category_bits = bits;

		}
		void CollisionFilter::SetMaskBits(int bits) {

			_mask_bits = bits;

		}
		void CollisionFilter::AddCategory(int bits) {

			_category_bits |= bits;

		}
		void CollisionFilter::RemoveCategory(int bits) {

			_category_bits &= ~bits;

		}
		void CollisionFilter::AddMask(int bits) {

			_mask_bits |= bits;

		}
		void CollisionFilter::RemoveMask(int bits) {

			_mask_bits &= ~bits;

		}
		bool CollisionFilter::CheckHit(const CollisionFilter& other) const {

			return  (_mask_bits & other._category_bits) != 0;

		}

}