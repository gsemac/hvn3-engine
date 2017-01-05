#include <allegro5/allegro.h>
#include <climits>
#include <cmath>
#include <algorithm>
#include "Object.h"
#include "Sprite.h"
#include "Helper.h"
#include "Graphics.h"

Object::Object() : Object(0.0f, 0.0f) {}
Object::Object(float x, float y) : ICollidable(x, y) {

	__image_index_timer = 0;
	Depth = 0;

}

void Object::Update(float dt) {

	IncrementImageIndex();

	if (Velocity.Magnitude() > 0.0f) {
		X += Velocity.X();
		Y += Velocity.Y();
	}

}
void Object::Draw() {

	// Draw sprite (if it exists).
	if (Sprite()) {

		Graphics::DrawSprite(
			*Sprite(), 
			ImageIndex(), 
			X, 
			Y,
			Color::FromArgbf(1.0f, 1.0f, 1.0f, ImageAlpha()),
			ImageXScale(),
			ImageYScale(),
			ImageAngle()
		);
		
	}

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
void ObjectBase::Draw() {}
void ObjectBase::Update() {}
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