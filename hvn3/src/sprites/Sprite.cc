#include "sprites/Sprite.h"
#include "exceptions/Exception.h"
#include "math/MathUtils.h"

namespace hvn3 {

	Sprite::Sprite() {

		_ox = 0;
		_oy = 0;
		_using_sprite_sheet = false;
		_strip_length = 0;

	}
	Sprite::Sprite(const Graphics::Bitmap& bitmap) :
		Sprite() {

		AddSubImage(Graphics::Bitmap(bitmap));

	}
	Sprite::Sprite(const Graphics::Bitmap& bitmap, const PointI& origin) :
		Sprite(bitmap) {

		SetOrigin(origin);

	}
	Sprite::Sprite(const Graphics::Bitmap& bitmap, int ox, int oy) :
		Sprite(bitmap, PointI(ox, oy)) {
	}
	Sprite::~Sprite() {
	}

	unsigned int Sprite::Width() const {

		if (_frames.size() <= 0)
			return 0;

		return _frames[0].Width();

	}
	unsigned int Sprite::Height() const {

		if (_frames.size() <= 0)
			return 0;

		return _frames[0].Height();

	}
	unsigned int Sprite::StripLength() const {

		return _strip_length > 0 ? _strip_length : static_cast<unsigned int>(_frames.size());

	}
	size_t Sprite::Length() const {

		return _frames.size();

	}

	PointI Sprite::Origin() const {

		return PointI(_ox, _oy);

	}
	void Sprite::SetOrigin(const PointI& origin) {

		SetOrigin(origin.X(), origin.Y());

	}
	void Sprite::SetOrigin(int ox, int oy) {

		_ox = ox;
		_oy = oy;

	}

	void Sprite::AddSubImage(const Graphics::Bitmap& sub_image) {

		_frames.push_back(Graphics::Bitmap(sub_image));

	}
	void Sprite::RemoveSubImage(int sub_image) {

		if (_frames.size() <= 0)
			return;

		_frames.erase(_frames.begin() + subImageToIndex(sub_image));

	}
	const Graphics::Bitmap& Sprite::SubImage(int sub_image) const {

		return _frames[subImageToIndex(sub_image)];

	}
	const Graphics::Bitmap& Sprite::operator[](int sub_image) const {

		return SubImage(sub_image);

	}

	Sprite Sprite::FromSpriteSheet(const Graphics::Bitmap& bitmap, int frame_width, int frame_height) {

		return FromSpriteSheet(bitmap, frame_width, frame_height, 0, 0);

	}
	Sprite Sprite::FromSpriteSheet(const Graphics::Bitmap& bitmap, int frame_width, int frame_height, int origin_x, int origin_y) {

		// Get the dimensions of the sheet, and determine the number of rows/columns.
		int sheet_width = bitmap.Width();
		int sheet_height = bitmap.Height();
		int columns = sheet_width / frame_width;
		int rows = sheet_height / frame_height;

		// Initialize the new Sprite. Note that the sheet must be maintained, because sub-bitmaps share its memory.
		// The sheet will be maintained as the first of the subimages; calls to the first subimage will return the second, and so on.
		Sprite sprite;
		sprite.SetOrigin(origin_x, origin_y);
		sprite._strip_length = columns;
		sprite._using_sprite_sheet = true;

		// Create sub-sprites from the sheet.
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				sprite.AddSubImage(Graphics::Bitmap(bitmap, RectangleI(frame_width * j, frame_height * i, frame_width, frame_height)));

		// Return the new sprite.
		return sprite;

	}
	Sprite Sprite::FromSpriteSheet(const Graphics::Bitmap& bitmap, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y) {

		throw System::NotImplementedException();

	}

			

	size_t Sprite::subImageToIndex(int sub_image) const {

		return Math::ModFloor(sub_image, _frames.size());

	}

}