#pragma once

namespace hvn3 {

	class CollisionFilter {

	public:
		typedef int filter_type;

		filter_type categoryBits;
		filter_type maskBits;

		CollisionFilter();
		CollisionFilter(filter_type category_bits, filter_type mask_bits);

		// Returns the category bits flag associated with the collider. This represents what the collider "is".
		filter_type CategoryBits() const;
		// Returns the mask bits flag assoated with the collider. This represents what the collider can collide with.
		filter_type MaskBits() const;

		// Sets the category bits flag associated with the collider. This represents what the collider "is".
		void SetCategoryBits(filter_type bits);
		// Sets the mask bits flag assoated with the collider. This represents what the collider can collide with.
		void SetMaskBits(filter_type bits);

		// Adds a flag to the collider's category bits.
		void AddCategory(filter_type bits);
		// Removes a flag from the collider's category bits.
		void RemoveCategory(filter_type bits);
		// Adds a flag to the collider's mask bits.
		void AddMask(filter_type bits);
		// Removes a flag from the collider's mask bits.
		void RemoveMask(filter_type bits);

		// Returns true if the other filter's category bits are contained in this filter's mask bits. Otherwise, returns false.
		bool CheckMatch(const CollisionFilter& other) const;

	};

}