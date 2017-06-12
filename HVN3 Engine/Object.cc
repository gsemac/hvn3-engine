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

namespace hvn3 {

	Object::Object(ObjectId id) :
		Object(id, Point2d<float>(0.0f, 0.0f)) {}
	Object::Object(ObjectId id, const Point2d<float>& position) : ICollidable(position.X(), position.Y()) {

		SetDepth(0);

	}

	void Object::OnUpdate(UpdateEventArgs& e) {

		
		
	}
	void Object::OnDraw(DrawEventArgs& e) {



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

	//float Object::DistanceTo(const Object* other) {

	//	return Distance(AABB(), other->AABB());

	//}
	
}