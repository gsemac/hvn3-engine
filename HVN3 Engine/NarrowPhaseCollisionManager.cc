#include "NarrowPhaseCollisionManager.h"
#include "Sprite.h"
#include "Vector2d.h"
#include "ICollisionBody.h"
#include "Direction8.h"
#include <allegro5/allegro.h>
#include <algorithm>

namespace hvn3 {

	bool NarrowPhaseCollisionManager::TestCollision(ICollisionBody* a, ICollisionBody* b) const {

		return TestCollision(a, a->Position(), b, b->Position());

	}
	bool NarrowPhaseCollisionManager::TestCollision(ICollisionBody* a, const PointF& position_a, ICollisionBody* b, const PointF& position_b) const {

		// Get the masks for both colliders.
		HitMaskPtr& a_mask = a->HitMask();
		HitMaskPtr& b_mask = b->HitMask();

		// Offset both colliders by the given offsets.
		a_mask->SetOffset(PointF(a_mask->Offset().X() + position_a.X(), a_mask->Offset().Y() + position_a.Y()));
		b_mask->SetOffset(PointF(b_mask->Offset().X() + position_b.X(), b_mask->Offset().Y() + position_b.Y()));

		bool hit = false;

		// If the bounding boxes do not intersect, return false.
		if (!hvn3::TestIntersection(a_mask->AABB(), b_mask->AABB()))
			hit = false;

		// Test to see if the two masks intersect one another.
		else
			hit = a_mask->TestIntersection(b_mask);

		// Reset the offsets.
		a_mask->SetOffset(PointF(a_mask->Offset().X() - position_a.X(), a_mask->Offset().Y() - position_a.Y()));
		b_mask->SetOffset(PointF(b_mask->Offset().X() - position_b.X(), b_mask->Offset().Y() - position_b.Y()));

		// Return the result.
		return hit;

	}

	bool NarrowPhaseCollisionManager::TestIntersection(Sprite* s1, Sprite* s2, const PointF& p1, const PointF& p2) const {

		// Get the Bitmaps corresponding to the Sprites.
		ALLEGRO_BITMAP* ba = s1->SubImage(0).AlPtr();
		ALLEGRO_BITMAP* bb = s2->SubImage(0).AlPtr();

		// Create bounding Rectangles from the Sprites.
		RectangleF a(p1.X() - s1->Origin().X(), p1.Y() - s1->Origin().Y(), s1->Width(), s1->Height());
		RectangleF b(p2.X() - s2->Origin().X(), p2.Y() - s2->Origin().Y(), s2->Width(), s2->Height());
		if (!hvn3::TestIntersection(a, b)) return false;

		// Create Rectangle to represent the overlap area.
		RectangleF overlap(PointF((std::max)(a.Left(), b.Left()), (std::max)(a.Top(), b.Top())),
			PointF((std::min)(a.Right(), b.Right()), (std::min)(a.Bottom(), b.Bottom())));

		// Calculate overlap Rectangles for both bounding Rectangles.
		RectangleF overlap_a((std::max)(b.Left() - a.Left(), 0.0f), (std::max)(b.Top() - a.Top(), 0.0f), overlap.Width(), overlap.Height());
		RectangleF overlap_b((std::max)(a.Left() - b.Left(), 0.0f), (std::max)(a.Top() - b.Top(), 0.0f), overlap.Width(), overlap.Height());

		// Lock Bitmaps in system memory.
		al_lock_bitmap(ba, al_get_bitmap_format(ba), ALLEGRO_LOCK_READONLY);
		al_lock_bitmap(bb, al_get_bitmap_format(bb), ALLEGRO_LOCK_READONLY);

		// Check for any non-transparent overlapping pixels.
		bool collided = false;
		for (int i = 0; i < overlap_a.Width(); ++i) {
			for (int j = 0; j < overlap_a.Height(); ++j) {
				ALLEGRO_COLOR pixel_a = al_get_pixel(ba, overlap_a.X() + i, overlap_a.Y() + j);
				ALLEGRO_COLOR pixel_b = al_get_pixel(bb, overlap_b.X() + i, overlap_b.Y() + j);
				if (pixel_a.a > 0.0f && pixel_b.a > 0.0f) {
					collided = true;
					break;
				}
			}
			if (collided)
				break;
		}

		// Unlock the Bitmaps.
		al_unlock_bitmap(ba);
		al_unlock_bitmap(bb);

		// Return the result.
		return collided;

	}
	bool NarrowPhaseCollisionManager::TestIntersection(Sprite* sprite, const CircleF& circle, const PointF& pos) const {

		/*
		Algorithm:
		- Find the closest edge of the Sprite to the Circle (left, right, top, or bottom).
		- Generate a Line from the pixels that exist on that edge.
		- Check the distance from the Line to the Circle.
		*/

		// Generate a bounding Rectangle for the Sprite.
		RectangleF bounds(pos.X() - sprite->Origin().X(), pos.Y() - sprite->Origin().Y(), sprite->Width(), sprite->Height());
		PointF center = PointF(bounds.X() + bounds.Width() / 2.0f, bounds.Y() + bounds.Height() / 2.0f);

		// Find the closest edge.
		float dist_x, dist_y;
		Direction8 edge;
		dist_x = (std::abs)(circle.X() - center.X());
		dist_y = (std::abs)(circle.Y() - center.Y());
		if (dist_x > dist_y)
			edge = (circle.X() < center.X()) ? DIRECTION_LEFT : DIRECTION_RIGHT;
		else
			edge = (circle.Y() < center.Y()) ? DIRECTION_UP : DIRECTION_DOWN;

		return false;

	}
	//bool NarrowPhaseCollisionManager::TestIntersection(const SpriteMask& mask, const CircleF& circle, const PointF& pos) const {

	//	// Adjust Point of Circle to be relative to the (0, 0)-based SpriteMask.
	//	PointF adj = PointF(circle.X() - pos.X(), circle.Y() - pos.Y());
	//	CircleF c = CircleF(adj, circle.Radius());

	//	// Check for intersection.
	//	return mask.Intersects(c);

	//}

}