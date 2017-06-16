#pragma once

namespace hvn3 {

	class CollisionFilter {

	public:
		CollisionFilter();

		// Returns the category bits flag associated with the collider. This represents what the collider "is".
		int CategoryBits() const;
		// Returns the mask bits flag assoated with the collider. This represents what the collider can collide with.
		int MaskBits() const;

		// Sets the category bits flag associated with the collider. This represents what the collider "is".
		void SetCategoryBits(int bits);
		// Sets the mask bits flag assoated with the collider. This represents what the collider can collide with.
		void SetMaskBits(int bits);

		// Adds a flag to the collider's category bits.
		void AddCategory(int bits);
		// Removes a flag from the collider's category bits.
		void RemoveCategory(int bits);
		// Adds a flag to the collider's mask bits.
		void AddMask(int bits);
		// Removes a flag from the collider's mask bits.
		void RemoveMask(int bits);

		// Returns true if the other filter's category bits are contained in this filter's mask bits. Otherwise, returns false.
		bool CheckHit(const CollisionFilter& other) const;

	private:
		int _category_bits;
		int _mask_bits;

	};

}