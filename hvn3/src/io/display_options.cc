#include "io/display_options.h"

namespace hvn3::io {

	// Public members

	DisplayOptions::DisplayOptions() :
		width(640),
		height(480),
		x(-1),
		y(-1),
		windowOptions(WindowOptions::None) {
	}

	int DisplayOptions::Width() const {

		return width;

	}
	void DisplayOptions::SetWidth(int width) {

		this->width = width;

	}
	int DisplayOptions::Height() const {

		return height;

	}
	void DisplayOptions::SetHeight(int height) {

		this->height = height;

	}
	SizeI DisplayOptions::Size() const {

		return SizeI(Width(), Height());

	}
	void DisplayOptions::SetSize(int width, int height) {

		SetWidth(width);
		SetHeight(height);

	}
	void DisplayOptions::SetSize(const SizeI& size) {

		SetWidth(size.Width());
		SetHeight(size.Height());

	}
	int DisplayOptions::X() const {

		return x;

	}
	void DisplayOptions::SetX(int x) {

		this->x = x;

	}
	int DisplayOptions::Y() const {

		return y;

	}
	void DisplayOptions::SetY(int y) {

		this->y = y;

	}
	math::PointI DisplayOptions::Position() const {

		return math::PointI(X(), Y());

	}
	void DisplayOptions::SetPosition(int x, int y) {

		SetX(x);
		SetY(y);

	}
	void DisplayOptions::SetPosition(const math::PointI& position) {

		SetPosition(position.X(), position.Y());

	}
	const String& DisplayOptions::Title() const {

		return title;

	}
	void DisplayOptions::SetTitle(const String& title) {

		this->title = title;

	}
	WindowOptions DisplayOptions::WindowOptions() const {

		return windowOptions;

	}
	void DisplayOptions::SetWindowOptions(enum WindowOptions windowOptions) {

		this->windowOptions = windowOptions;

	}

}