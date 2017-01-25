#include <algorithm>
#include "CollisionManager.h"
#include "Vector2d.h"

// Helper Functions

// Returns true if two axis-aligned Sprites intersect at the specified positions.
bool Intersects(Sprite* s1, Sprite* s2, const Point& p1, const Point& p2) {

	// Get the Bitmaps corresponding to the Sprites.
	ALLEGRO_BITMAP* ba = s1->AlPtr();
	ALLEGRO_BITMAP* bb = s2->AlPtr();

	// Create bounding Rectangles from the Sprites.
	Rectangle a(p1.X() - s1->Origin().X(), p1.Y() - s1->Origin().Y(), s1->Width(), s1->Height());
	Rectangle b(p2.X() - s2->Origin().X(), p2.Y() - s2->Origin().Y(), s2->Width(), s2->Height());
	if (!Intersects(a, b)) return false;

	// Create Rectangle to represent the overlap area.
	Rectangle overlap(Point((std::max)(a.Left(), b.Left()), (std::max)(a.Top(), b.Top())),
		Point((std::min)(a.Right(), b.Right()), (std::min)(a.Bottom(), b.Bottom())));

	// Calculate overlap Rectangles for both bounding Rectangles.
	Rectangle overlap_a((std::max)(b.Left() - a.Left(), 0.0f), (std::max)(b.Top() - a.Top(), 0.0f), overlap.Width(), overlap.Height());
	Rectangle overlap_b((std::max)(a.Left() - b.Left(), 0.0f), (std::max)(a.Top() - b.Top(), 0.0f), overlap.Width(), overlap.Height());

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
// Returns true if the axis-aligned Sprite intersects with the Circle at the specific position.
bool Intersects(Sprite* sprite, const Circle& circle, const Point& pos) {

	/*
	Algorithm:
	- Find the closest edge of the Sprite to the Circle (left, right, top, or bottom).
	- Generate a Line from the pixels that exist on that edge.
	- Check the distance from the Line to the Circle.
	*/

	// Generate a bounding Rectangle for the Sprite.
	Rectangle bounds(pos.X() - sprite->Origin().X(), pos.Y() - sprite->Origin().Y(), sprite->Width(), sprite->Height());
	Point center = Point(bounds.X() + bounds.Width() / 2.0f, bounds.Y() + bounds.Height() / 2.0f);

	// Find the closest edge.
	float dist_x, dist_y;
	Direction edge;
	dist_x = (std::abs)(circle.X() - center.X());
	dist_y = (std::abs)(circle.Y() - center.Y());
	if (dist_x > dist_y)
		edge = (circle.X() < center.X()) ? LEFT : RIGHT;
	else
		edge = (circle.Y() < center.Y()) ? UP : DOWN;

	return false;

}
// Returns true if the SpriteMask intersects with the Circle at the specified position.
bool Intersects(const SpriteMask& mask, const Circle& circle, const Point& pos) {

	// Adjust Point of Circle to be relative to the (0, 0)-based SpriteMask.
	Point adj = Point(circle.X() - pos.X(), circle.Y() - pos.Y());
	Circle c = Circle(adj, circle.Radius());

	// Check for intersection.
	return mask.Intersects(c);

}

// Public methods

CollisionManager::CollisionManager(IBroadphase* broadphase) {

	__broadphase = broadphase;

}
bool CollisionManager::TestCollision(ICollidable* a, ICollidable* b) const {

	return TestCollision(a, a->X(), a->Y(), b, b->X(), b->Y());

}
bool CollisionManager::TestCollision(ICollidable* a, float ax, float ay, ICollidable* b, float bx, float by) const {
	
	// Get the masks for both Objects.
	const ICollisionMask& maska = a->Mask();
	const ICollisionMask& maskb = b->Mask();

	// Get AABBs for both Objects.
	Rectangle aabb_a = maska.AABB();
	Rectangle aabb_b = maskb.AABB();
	aabb_a.SetX(aabb_a.X() + ax);
	aabb_a.SetY(aabb_a.Y() + ay);
	aabb_b.SetX(aabb_a.X() + bx);
	aabb_b.SetY(aabb_a.Y() + by);

	// Check if the AABBs intersect.
	if (!Intersects(aabb_a, aabb_b))
		return false;
	if (maska.Type() == MaskType::Rectangle && maskb.Type() == MaskType::Rectangle)
		return true; // (Same as AABB)

					 // Perform more precise collision checking.
	//if (maska.IsCircle()) {
	//	if (maskb.IsRectangle())
	//		return Intersects(maskb.ToRectangle(bx, by), maska.ToCircle(ax, ay));
	//	if (maskb.IsSprite())
	//		return Intersects(maskb.ToSpriteMask(), maska.ToCircle(ax, ay), Point(bx, by));
	//}
	//else if (maska.IsRectangle()) {
	//	if (maskb.IsCircle())
	//		return Intersects(maska.ToRectangle(ax, ay), maskb.ToCircle(bx, by));
	//}

	// No valid mask comparison.
	return false;

}
IBroadphase& CollisionManager::Broadphase() {

	return *__broadphase;

}
void CollisionManager::Update() {

	// If the broadphase pointer is null, there is nothing to do.
	if (!__broadphase)
		return;

	// Update the state of the collision detection method.
	__broadphase->Update();

	// Process all collisions.
	ProcessCollisions(__broadphase->FindPairs());

}

// Private methods

void CollisionManager::ProcessCollisions(const std::vector<std::pair<ICollidable*, ICollidable*>>& pairs) const {
	return;
	// Test for a collision with each pair, and call the appropriate Collide function(s).
	for (auto i = pairs.begin(); i != pairs.end(); ++i) {
		ICollidable* a = i->first;
		ICollidable* b = i->second;
		if (!TestCollision(a, b))
			continue;
		if (a->CollidesWith(b))
			a->Collide(b);
		if (b->CollidesWith(a))
			b->Collide(a);
	}

}