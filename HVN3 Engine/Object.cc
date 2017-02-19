#include <allegro5/allegro.h>
#include <climits>
#include <cmath>
#include <algorithm>
#include "Object.h"
#include "Sprite.h"
#include "Helper.h"
#include "Graphics.h"
#include "DrawEventArgs.h"
#include "UpdateEventArgs.h"
#include "Room.h"

Object::Object() : Object(0.0f, 0.0f) {}
Object::Object(float x, float y) : ICollidable(x, y) {

	__image_index_timer = 0;
	__room = nullptr;
	SetDepth(0);

}

void Object::Update(UpdateEventArgs& e) {

	IncrementImageIndex();

	if (Velocity().Magnitude() > 0.0f)
		Translate(Velocity().X(), Velocity().Y());

}
void Object::Draw(DrawEventArgs& e) {

	// Draw sprite (if it exists).
	if (Sprite()) {

		e.Graphics().DrawSprite(
			X(),
			Y(),
			*Sprite(),
			ImageIndex(),
			ImageXScale(),
			ImageYScale(),
			ImageAngle(),
			Color::FromArgbf(1.0f, 1.0f, 1.0f, ImageAlpha())
			);

	}

}
int Object::Depth() const {

	return __depth;

}
void Object::SetDepth(int depth) {

	__depth = depth;

}
Vector2d& Object::Velocity() {

	return __velocity;

}
Room* Object::Room() {

	return __room;

}

float DistanceToObject(const std::shared_ptr<Object>& a, const std::shared_ptr<Object>& b) {

	return DistanceToObject(a.get(), b.get());

}
float DistanceToObject(const Object& a, const Object& b) {

	return DistanceToObject(&a, &b);

}
float DistanceToObject(const Object* a, const Object* b) {

	Rectangle ar = a->AABB();
	Rectangle br = b->AABB();

	return Distance(ar, br);

}

ObjectBase::ObjectBase() {

	__is_destroyed = false;
	__active = false;

}
void ObjectBase::Draw(DrawEventArgs& e) {}
void ObjectBase::Update(UpdateEventArgs& e) {}
void ObjectBase::Destroy() {

	__is_destroyed = true;

}
bool ObjectBase::IsDestroyed() {

	return __is_destroyed;

}
void ObjectBase::Deactivate() {

	__active = false;

}
bool ObjectBase::IsActive() {

	return __active;

}

void Object::IncrementImageIndex() {

	__image_index_timer += (std::fabs)(ImageSpeed());
	if (__image_index_timer >= 1.0f) {
		switch (Sign(ImageSpeed())) {
		case -1:
			if (ImageIndex() == 0)
				SetImageIndex(INT_MAX);
			else
				SetImageIndex(ImageIndex() - 1);
		case 1:
			if (ImageIndex() == INT_MAX)
				SetImageIndex(0);
			else
				SetImageIndex(ImageIndex() + 1);
		}
		__image_index_timer -= 1.0f;
	}

}

bool Object::PlaceFree(float x, float y, bool notme) {

	// If the object is not part of a Scene, it can't collide with anything, so return true.
	if (!Room())
		return true;

	std::vector<ICollidable*> v;
	Room()->CollisionManager().Broadphase().QueryRegion(AABB(), v, Filter().MaskBits());
	if (v.size() == 0 || (notme && v.size() == 1 && v[0] == this))
		return true;

	for (size_t i = 0; i < v.size(); ++i)
		if (Room()->CollisionManager().TestCollision(this, x, y, v[i], v[i]->X(), v[i]->Y()))
			return false;

	return true;

}
void Object::MoveContact(float direction, int max_distance) {

	for (int i = 0; i < max_distance; ++i) {
		Point new_position = PointInDirection(Point(X(), Y()), direction, 1);
		if (!PlaceFree(new_position.X(), new_position.Y(), true))
			break;
		SetXY(new_position.X(), new_position.Y());
	}

}