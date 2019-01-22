#pragma once

#include "hvn3/graphics/Transform.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/objects/IObject.h"
#include "hvn3/utility/Scale.h"

namespace hvn3 {

	class View {

	public:
		View(float x, float y, float width, float height);
		View(float x, float y, float width, float height, float port_x, float port_y, float port_width, float port_height);
		View(const PointF& position, const SizeF& size);
		View(const PointF& position, const SizeF& size, const PointF& port_position, const SizeF& port_size);
		View(const PointF& view_position, const SizeF& view_size, const PointF& port_position, const SizeF& port_size, IObject* following, const SizeF& border);

		void SetFollowing(IObject* following);
		void SetFollowing(IObject* following, float border_width, float border_height);
		void SetFollowing(IObject* following, const SizeF& border);
		const IObject* GetFollowing() const;
		IObject* GetFollowing();

		RectangleF Region() const;
		RectangleF Port() const;
		SizeF Size() const;
		float X() const;
		float Y() const;
		const PointF& Position() const;
		void SetPosition(float x, float y);
		void SetPosition(const PointF& position);
		void SetX(float x);
		void SetY(float y);
		Scale Scale() const;

		float HorizontalBorder() const;
		float VerticalBorder() const;

		float Angle() const;
		void SetAngle(float value);

		void SetEnabled(bool value);
		bool Enabled() const;

		bool MouseTrackingEnabled() const;
		void SetMouseTrackingEnabled(bool value);

		Graphics::Transform GetTransform() const;

	private:
		PointF _position;
		SizeF _size;
		PointF _port_position;
		SizeF _port_size;

		IObject* _following;
		float _horizontal_border, _vertical_border;
		float _vspeed, _hspeed;
		float _angle;
		bool _enabled;
		bool _tracks_mouse;

	};

}