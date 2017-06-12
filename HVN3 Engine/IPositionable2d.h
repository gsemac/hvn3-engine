#pragma once
#include "Point2d.h"

namespace hvn3 {

	template <typename T>
	class IPositionable2d {

	public:
		IPositionable2d() :
			IPositionable2d(0, 0) {}
		IPositionable2d(T x, T y) :
			_position(x, y) {}

		T X() const {

			return _position.X();

		}
		T Y() const {

			return _position.Y();

		}
		virtual void SetX(T x) {

			_position.SetX(x);

		}
		virtual void SetY(T y) {

			_position.SetY(y);

		}

		virtual const Point2d<T>& Position() const {

			return _position;

		}
		virtual void SetPosition(const Point2d<T>& position) {

			_position = position;

		}
		virtual void SetPosition(T x, T y) {

			_position.SetX(x);
			_position.SetY(y);

		}

	private:
		Point2d<T> _position;

	};

}