#pragma once
#include "objects/Object.h"
#include "utility/Scale.h"
#include "graphics/Transform.h"

namespace hvn3 {

	class View {

	public:
		View(const Point2d<float>& view_position, const Size<float>& view_size, const Point2d<float>& port_position, const Size<float>& port_size);
		View(const Point2d<float>& view_position, const Size<float>& view_size, const Point2d<float>& port_position, const Size<float>& port_size, Object* follow_obj, float horizontal_border, float vertical_border);

		void SetFollowing(Object* obj);
		Object* GetFollowing();

		Rectangle<float> Region() const;
		Rectangle<float> Port() const;
		const Point2d<float>& Position() const;
		void SetPosition(float x, float y);
		void SetPosition(const Point2d<float>& position);
		float ViewX() const;
		float ViewY() const;
		Scale Scale() const;

		float HorizontalBorder() const;
		float VerticalBorder() const;

		float Angle() const;
		void SetAngle(float angle);

		void Enable();
		void Disable();
		bool Enabled() const;

		bool TracksMouse() const;
		void TracksMouse(bool tracks);

		Drawing::Transform GetTransform() const;

	private:
		Point2d<float> __view_pos;
		Size<float> __view_size;
		Point2d<float> __port_pos;
		Size<float> __port_size;

		Object* __follow_obj;
		float __hbor, __vbor, __vspd, __hspd;
		float __angle;
		bool __enabled;
		bool __tracks_mouse;

	};

}