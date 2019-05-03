#pragma once

#include "hvn3/ecs/Entity.h"
#include "hvn3/math/Rectangle.h"

namespace hvn3 {

	class View {

	public:
		View(float x, float y, float width, float height);
		View(float x, float y, float width, float height, float portX, float portY, float portWidth, float portHeight);
		View(const PointF& position, const SizeF& size);
		View(const PointF& viewPosition, const SizeF& viewSize, const PointF& portPosition, const SizeF& portSize);
		View(const RectangleF& view, const RectangleF& port);

		void SetTarget(const ecs::Entity& target);
		void SetTarget(const ecs::Entity& target, float borderWidth, float borderHeight);
		void SetTarget(const ecs::Entity& target, const SizeF& border);
		void ResetTarget();
		const ecs::Entity& Target() const;

		const RectangleF& Region() const;
		const RectangleF& Port() const;
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
		RectangleF _view;
		RectangleF _port;

		ecs::Entity _target;
		float _horizontal_border, _vertical_border;
		float _vspeed, _hspeed;
		float _angle;
		bool _enabled;
		bool _tracks_mouse;

	};

}