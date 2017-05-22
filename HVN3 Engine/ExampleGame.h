#pragma once
#include "HVN3.h"

namespace hvn3 {

	struct RotatingBox : Object {
		using Object::Object;

		void Draw() override {

			static int width = 64;
			static int thick = 5;
			static float speed = 0.5f;
			static bool sink = false;
			static float y_off = 0.0f;

			if (sink)
				if (y_off > 0.0f)
					y_off -= 0.1f;
				else
					sink = false;
			else
				if (y_off < 16.0f)
					y_off += 0.1f;
				else
					sink = true;

			//Graphics::Transform rotation;
			//rotation.Rotate(Point(X, Y), 45.0f);
			//Graphics::SetTransform(rotation);
			//Graphics::DrawRectangle(Rectangle(X, Y, width, width), Color::White, 5);
			//Graphics::ResetTransform();


			SetImageAngle(ImageAngle() + speed);
			Graphics::Transform trans;

			//trans.Rotate(Point(X + 48 + width / 2, Y + 48 + width / 2), ImageAngle());
			//Graphics::SetTransform(trans);
			//Graphics::DrawRectangle(Rectangle(X + 48, Y + 48, width, width), Color(Color::Red).Darken(0.1f), thick);
			//trans.Reset();

			// Draw the cube shadow.
			for (int i = 0; i < 30; ++i) {
				trans.Rotate(Point(X() + -y_off + i + 48 + width / 2, Y() + i + 48 + width / 2), ImageAngle());
				Graphics::SetTransform(trans);
				Graphics::DrawRectangle(Rectangle(X() + -y_off + i + 48, Y() + i + 48, width, width), Color(Color::Red).Darken(0.1f), thick);
				trans.Reset();
				Graphics::ResetTransform();
			}

			// Draw the cube itself.
			for (int i = 0; i < 30; ++i) {
				trans.Rotate(Point(X() + i + width / 2, Y() + y_off + i + width / 2), ImageAngle());
				Graphics::SetTransform(trans);
				Graphics::DrawRectangle(Rectangle(X() + i, Y() + y_off + i, width, width), Color::Black, thick);
				trans.Reset();
				Graphics::ResetTransform();
			}

			// Draw the upper edge of the cube.
			trans.Rotate(Point(X() + width / 2, Y() + y_off + width / 2), ImageAngle());
			Graphics::SetTransform(trans);
			Graphics::DrawRectangle(Rectangle(X(), Y() + y_off, width, width), Color::White, thick);
			Graphics::ResetTransform();

		}

	};

}