#pragma once
#include "hvn3/objects/ObjectBase.h"

namespace hvn3 {

	class Camera2d : 
		public ObjectBase {

	public:
		Camera2d();
		Camera2d(float x, float y);
		Camera2d(const PointF& position);
		Camera2d(IObject* target);
		Camera2d(ObjectId target_id);

		void OnUpdate(UpdateEventArgs& e) override;

		void SetTarget(IObject* target);
		void SetTarget(ObjectId target_id);

	private:
		PointF _position;
		IObject* _target;
		ObjectId _target_id;

	};

}