#include <allegro5/allegro.h>
#include <climits>
#include <cmath>
#include <algorithm>
#include "Object.h"
#include "Sprite.h"
#include "Utility.h"
#include "Graphics.h"
#include "DrawEventArgs.h"
#include "UpdateEventArgs.h"
#include "RoomBase.h"
#include "NarrowPhaseCollisionManager.h"

Object::Object() : Object(0.0f, 0.0f) {}
Object::Object(float x, float y) : ICollidable(x, y) {

	__image_index_timer = 0;
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
ObjectId Object::Id() const {

	return noone;

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

float Object::DistanceTo(const Object* other) {

	return Distance(AABB(), other->AABB());

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