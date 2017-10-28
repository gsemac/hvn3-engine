#pragma once
#include "core/IPositionable2d.h"

namespace hvn3 {

	template <typename T>
	class Positionable2dBase : public IPositionable2d<T> {

	public:
		Positionable2dBase() :
			Positionable2dBase(0, 0) {}
		Positionable2dBase(T x, T y) :
			_position(x, y) {}
		Positionable2dBase(const Point2d<T>& position) :
			_position(position) {}

		T X() const override {

			return _position.X();

		}

		T Y() const override {

			return _position.Y();

		}
		virtual void SetX(T x) override {

			_position.SetX(x);

		}
		virtual void SetY(T y) override {

			_position.SetY(y);

		}

		virtual Point2d<T> Position() const override {

			return _position;

		}
		virtual void SetPosition(const Point2d<T>& position) override {

			_position = position;

		}
		virtual void SetPosition(T x, T y) override {

			_position.SetX(x);
			_position.SetY(y);

		}

	private:
		Point2d<T> _position;

	};

}