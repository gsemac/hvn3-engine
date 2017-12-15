#include <allegro5/allegro.h>
#include <climits>
#include <cmath>
#include <algorithm>
#include "hvn3/objects/Object.h"
#include "hvn3/sprites/Sprite.h"
#include "hvn3/utility/Utility.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/rooms/RoomBase.h"

namespace hvn3 {

	Object::Object(ObjectId id) :
		Object(id, PointF(0.0f, 0.0f)) {
	}
	Object::Object(ObjectId id, const PointF& position) :
		Positionable2dBase(position) {

		_id = id;

		SetDepth(0);

	}

	void Object::OnCreate(CreateEventArgs& e) {}
	void Object::OnDestroy(DestroyEventArgs& e) {}
	void Object::OnUpdate(UpdateEventArgs& e) {
	}
	void Object::OnDraw(DrawEventArgs& e) {
	}
	void Object::OnCollision(CollisionEventArgs& e) {
	}

	ObjectId Object::Id() const {

		return _id;

	}

	int Object::Depth() const {

		return _depth;

	}
	void Object::SetDepth(int depth) {

		_depth = depth;

	}

}