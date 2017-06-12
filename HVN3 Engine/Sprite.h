#pragma once
#include <vector>
#include <string>
#include "Point2d.h"
#include "Color.h"
#include "Bitmap.h"
#include "UniqueCreateableBase.h"

namespace hvn3 {

	class Sprite;

	typedef std::unique_ptr<Sprite> SpritePtr;

	class Sprite : public UniqueCreateableBase<Sprite> {

	public:
		Sprite(const char* path);
		Sprite(const char* path, int origin_x, int origin_y);
		Sprite(const char* path, int origin_x, int origin_y, const Color& alpha_color);

		Sprite(const std::string& path);
		Sprite(const std::string& path, int origin_x, int origin_y);
		Sprite(const std::string& path, int origin_x, int origin_y, const Color& alpha_color);

		Sprite(Sprite&& other);

		~Sprite();

		// Returns the width of the first subimage of the sprite, or 0 of no subimages exist.
		unsigned int Width() const;
		// Returns the height of the first subimage of the sprite, or 0 of no subimages exist.
		unsigned int Height() const;

		// If loaded from a sprite sheet, returns the length of each strip. Otherwise, returns the number of subimages.
		unsigned int StripLength() const;
		// Returns the number of subimages.
		size_t Length() const;

		// Returns a point corresponding to the origin (offset) of the sprite.
		Point2d<int> Origin() const;
		// Sets the origin (offset) of the sprite to specified values.
		void SetOrigin(int origin_x, int origin_y);

		// Adds a new subimage to the sprite as a bitmap.
		void AddSubImage(Drawing::Bitmap& subimage, bool owner = true);
		// Adds a new subimage o the sprite from a file.
		void AddSubImage(const char* path);

		// Returns the subimage at the given index.
		const Drawing::Bitmap& SubImage(int subimage) const;
		// Returns the subimage at the given index.
		const Drawing::Bitmap& operator[](int subimage) const;

		// Static methods

		static SpritePtr CreateFromSpriteSheet(const char* path, int frame_width, int frame_height);
		static SpritePtr CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y);
		static SpritePtr CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color);
		static SpritePtr CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y);
		static SpritePtr CreateFromSpriteSheet(const char* path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color);
		
		static SpritePtr CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height);
		static SpritePtr CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y);
		static SpritePtr CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int origin_x, int origin_y, const Color& alpha_color);
		static SpritePtr CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y);
		static SpritePtr CreateFromSpriteSheet(const std::string& path, int frame_width, int frame_height, int frame_x_offset, int frame_y_offset, int frame_x_separation, int frame_y_separation, int frame_number, int origin_x, int origin_y, const Color& alpha_color);
		
	private:
		std::vector<Drawing::Bitmap> _frames;
		int _origin_x;
		int _origin_y;
		bool _using_sprite_sheet;
		unsigned int _strip_length;

		Sprite();

	};

}