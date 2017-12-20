#include "hvn3/collision/CategoryFilter.h"

namespace hvn3 {

		CategoryFilter::CategoryFilter() {

			_category_bits = 0;
			_mask_bits = 0;

		}
		int CategoryFilter::CategoryBits() const {

			return _category_bits;

		}
		int CategoryFilter::MaskBits() const {

			return _mask_bits;

		}
		void CategoryFilter::SetCategoryBits(int bits) {

			_category_bits = bits;

		}
		void CategoryFilter::SetMaskBits(int bits) {

			_mask_bits = bits;

		}
		void CategoryFilter::AddCategory(int bits) {

			_category_bits |= bits;

		}
		void CategoryFilter::RemoveCategory(int bits) {

			_category_bits &= ~bits;

		}
		void CategoryFilter::AddMask(int bits) {

			_mask_bits |= bits;

		}
		void CategoryFilter::RemoveMask(int bits) {

			_mask_bits &= ~bits;

		}
		bool CategoryFilter::CheckHit(const CategoryFilter& other) const {

			return  (_mask_bits & other._category_bits) != 0;

		}

}