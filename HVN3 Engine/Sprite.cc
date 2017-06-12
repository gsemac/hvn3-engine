#include "Sprite.h"
#include "io/Directory.h"
#include "Exception.h"
#include "Framework.h"
#include <limits>
#include <cmath>

namespace hvn3 {

	Sprite::Sprite() {}
	Sprite::Sprite(const char* path) :
		Sprite(path, 0, 0) {
	}
	Sprite::Sprite(const char* path, int origin_x, int origin_y) {

		// Load the image from the filesystem.
		Drawing::Bitmap bmp(path);

		// If we failed to access the file, throw an error.
		if (!bmp)
			throw IO::IOException("Failed to access file '" + std::string(path) + "'.");

		// We are not using a sprite sheet, so set these values accordingly.
		_using_sprite_sheet = false;
		_strip_length = 0;

		// Add the image to the subimage collection.
		AddSubImage(bmp, true);

		// Set remaining members.
		_origin_x = origin_x;
		_origin_y = origin_y;

	}
	Sprite::Sprite(const char* path, int origin_x, int origin_y, const Color& alpha_color)
		: Sprite(path, origin_x, origin_y) {

		// If the first frame was added successfully, remove the background color.
		if (_frames[_using_sprite_sheet])
			_frames[_using_sprite_sheet].ConvertMaskToAlpha(alpha_color);

	}

	Sprite::Sprite(const std::string& path) :
		Sprite(path.c_str()) {
	}
	Sprite::Sprite(const std::string& path, int origin_x, int origin_y) :
		Sprite(path.c_str(), origin_x, origin_y) {
	}
	Sprite::Sprite(const std::string& path, int origin_x, int origin_y, const Color& alpha_color)
		: Sprite(path.c_str(), origin_x, origin_y, alpha_color) {
	}

	Sprite::Sprite(Sprite&& other) {

		// Move all frames to the other Sprite.
		for (size_t i = 0; i < other._frames.size(); ++i)
			_frames.push_back(std::move(other._frames[i]));

		// Copy member variables to the new object.
		_origin_x = other._origin_x;
		_origin_y = other._origin_y;
		_strip_length = other._strip_length;
		_using_sprite_sheet = other._using_sprite_sheet;

		// Reset the member variables of the object.
		other._origin_x = 0;
		other._origin_y = 0;
		other._strip_length = 0;
		other._using_sprite_sheet = false;

	}

	Sprite::~Sprite() {

		// If the framework has been shut down before freeing resources, do nothing (or else we'd get an error), but alert the user.
		if (!Framework::Initialized()) {
			std::cerr << "Warning: Framework was shutdown before this resource could be freed. Free all resources before shutting down the framework.\n";
			return;
		}

		// Dispose of all frames.
		_frames.clear();

	}

	unsigned int Sprite::Width() const {

		return (_frames[_using_sprite_sheet] ? _frames[_using_sprite_sheet].Width() : 0);

	}
	unsigned int Sprite::Height() const {

		return (_frames[_using_sprite_sheet] ? _frames[_using_sprite_sheet].Height() : 0);

	}

	unsigned int Sprite::StripLength() const {

		return _strip_length > 0 ? _strip_length : (int)_frames.size();

	}
	unsigned int Sprite::Length() const {

		return (unsigned int)_frames.size();

	}

	Point2d<int> Sprite::Origin() const {

		return Point2d<int>(_origin_x, _origin_y);

	}
	void Sprite::SetOrigin(int origin_x, int origin_y) {

		_origin_x = origin_x;
		_origin_y = origin_y;

	}

	void Sprite::AddSubImage(Drawing::Bitmap& subimage, bool owner) {

		if (owner)
			_frames.push_back(std::move(subimage));
		else
			_frames.push_back(subimage);

	}
	void Sprite::AddSubImage(const char* path) {

		AddSubImage(Drawing::Bitmap(path), true);

	}

	const Drawing::Bitmap& Sprite::SubImage(int subimage) const {

		int index = (std::abs)(subimage) % _frames.size();
		index += _using_sprite_sheet;

		return _frames[index];

	}
	const Drawing::Bitmap& Sprite::operator[] (int subimage) const {

		return SubImage(subimage);

	}

	SpritePtr Sprite::CreateFromSpriteSheet(const char* path, int frame_width, int frame_height) {

		return CreateFromSpriteSheet(path, frame_width, frame_height, 0, 0);

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y) {

		// Load the sprite sheet from the filesystem.
		Drawing::Bitmap sheet(path);

		// If we failed to access the file, throw an error.
		if (!sheet)
			throw IO::IOException("Failed to access file '" + std::string(path) + "'.");

		// Get the dimensions of the sheet, and determine the number of rows/columns.
		int sheet_width = sheet.Width();
		int sheet_height = sheet.Height();
		int columns = sheet_width / frame_width;
		int rows = sheet_height / frame_height;

		// Initialize the new Sprite. Note that the sheet must be maintained, because sub-bitmaps share its memory.
		// The sheet will be maintained as the first of the subimages; calls to the first subimage will return the second, and so on.
		SpritePtr sprite = Sprite::Create(new Sprite);
		sprite->_origin_x = origin_x;
		sprite->_origin_y = origin_y;
		sprite->_strip_length = columns;
		sprite->_using_sprite_sheet = true;

		// Add the sheet as the first subimage.
		sprite->AddSubImage(sheet);

		// Create sub-sprites from the sheet.
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				sprite->AddSubImage(Drawing::Bitmap(sprite->_frames[0], RectangleI(frame_width * j, frame_height * i, frame_width, frame_height)));

		// Return the new sprite.
		return sprite;

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color) {

		SpritePtr sprite = CreateFromSpriteSheet(path, frame_width, frame_height, origin_x, origin_y);
		sprite->_frames[0].ConvertMaskToAlpha(alpha_color);

		return sprite;

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y) {

		throw NotImplementedException();

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color) {

		throw NotImplementedException();

	}

	SpritePtr Sprite::CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height) {

		return CreateFromSpriteSheet(path.c_str(), frame_width, frame_height, 0, 0);

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y) {

		return CreateFromSpriteSheet(path.c_str(), frame_width, frame_height, origin_x, origin_y);

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color) {

		return CreateFromSpriteSheet(path.c_str(), frame_width, frame_height, origin_x, origin_y, alpha_color);

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y) {

		return CreateFromSpriteSheet(path.c_str(), frame_width, frame_height, frame_x_offset, frame_y_offset, frame_x_separation, frame_y_separation, frame_number, origin_x, origin_y);

	}
	SpritePtr Sprite::CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color) {

		return CreateFromSpriteSheet(path.c_str(), frame_width, frame_height, frame_x_offset, frame_y_offset, frame_x_separation, frame_y_separation, frame_number, origin_x, origin_y, alpha_color);

	}
	
}