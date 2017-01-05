#pragma once
#include "Engine.h"

struct RotatingBox : Object {
	using Object::Object;

	void Draw() override {

		static int width = 64;
		static int thick = 5;
		static float speed = 0.5f;

		SetImageAngle(ImageAngle() + speed);
		Graphics::Transform trans;
		Graphics::Transform old = Graphics::GetTransform();

		trans.Rotate(Point(X + 48 + width / 2, Y + 48 + width / 2), ImageAngle());
		Graphics::SetTransform(trans);
		Graphics::DrawRectangle(Rectangle(X + 48, Y + 48, width, width), Color(Color::Red).Darken(0.1f), thick);
		trans.Reset();
		for (int i = 0; i < 30; ++i) {
			trans.Rotate(Point(X + i + 48 + width / 2, Y + i + 48 + width / 2), ImageAngle());
			Graphics::SetTransform(trans);
			Graphics::DrawRectangle(Rectangle(X + i + 48, Y + i + 48, width, width), Color(Color::Red).Darken(0.1f), thick);
			trans.Reset();
		}

		for (int i = 0; i < 30; ++i) {
			trans.Rotate(Point(X + i + width / 2, Y + i + width / 2), ImageAngle());
			Graphics::SetTransform(trans);
			Graphics::DrawRectangle(Rectangle(X + i, Y + i, width, width), Color::Black, thick);
			trans.Reset();
		}

		trans.Reset();
		trans.Rotate(Point(X + width / 2, Y + width / 2), ImageAngle());
		Graphics::SetTransform(trans);
		Graphics::DrawRectangle(Rectangle(X, Y, width, width), Color::White, thick);
		Graphics::SetTransform(old);

	}

};