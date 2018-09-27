#pragma once
#include "hvn3/graphics/Tween.h"
#include "hvn3/objects/ObjectBase.h"

namespace hvn3 {

	class View;

	class Camera2d : 
		public ObjectBase {

	public:
		enum class PanMode {
			Immediate,
			Linear,
			EaseOut
		};
		
		Camera2d();
		Camera2d(float x, float y);
		Camera2d(const PointF& position);
		Camera2d(IObject* target);
		Camera2d(ObjectId target_id);

		void OnCreate(CreateEventArgs& e) override;
		void OnUpdate(UpdateEventArgs& e) override;
		void OnContextChanged(ContextChangedEventArgs& e) override;

		void SetPosition(const PointF& position) override;

		void SnapToTarget();
		void SetFollowing(IObject* target);
		void SetFollowing(ObjectId target_id);
		void SetOffset(float offset_x, float offset_y);
		void SetOffset(const PointF& offset);
		void SetPanMode(PanMode value);
		void SetTiltEnabled(bool value);

	private:
		PointF _offset;
		IObject* _target;
		ObjectId _target_id;
		PanMode _pan_mode;
		bool _tilt_enabled;
		Context _context;

		PointF _getTargetPosition();
		bool _hasFollowingTarget();
		View* _getView();

	};

}