#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/io/Path.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/tiles/Tileset.h"
#include "hvn3/xml/XmlDocument.h"

#include <cassert>
#include <sstream>

namespace hvn3 {

	Tileset::Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size) :
		Tileset(tileset, tile_size, PointI(0, 0), PointI(0, 0)) {
	}
	Tileset::Tileset(const Graphics::Bitmap& tileset, const SizeI& tile_size, const PointI& offset, const PointI& separation) :
		_bitmap(tileset),
		_tile_size(tile_size) {

		// Initialize column count.
		_rows = 0;

		// Get width/height as unsigned integers.
		int w = tile_size.Width();
		int h = tile_size.Height();

		assert(w > 0);
		assert(w > 0);

		// Generate a vector of sub-bitmaps to represent each tile.
		for (int y = offset.Y(); y < _bitmap.Height(); y += h + separation.Y()) {

			// Insert the next row of tiles.
			for (int x = offset.X(); x < _bitmap.Width(); x += w + separation.X())
				_tiles.push_back(Tile(std::move(Graphics::Bitmap(_bitmap, RectangleI(x, y, Math::Min(w, _bitmap.Width() - x), Math::Min(h, _bitmap.Height() - y))))));

			// Increment the number of rows.
			++_rows;

		}

	}
	const Tileset::Tile& Tileset::At(unsigned int x, unsigned int y) const {
		return _tiles[y * Columns() + x];
	}
	const Tileset::Tile& Tileset::At(unsigned int n) const {
		return _tiles[n];
	}
	Tileset::Tile& Tileset::GetAt(unsigned int x, unsigned int y) {
		return _tiles[y * Columns() + x];
	}
	Tileset::Tile& Tileset::GetAt(unsigned int n) {
		return _tiles[n];
	}
	size_t Tileset::Count() const {

		return _tiles.size();

	}
	size_t Tileset::Rows() const {
		return _rows;
	}
	size_t Tileset::Columns() const {
		return _tiles.size() / _rows;
	}
	const Graphics::Bitmap& Tileset::Bitmap() const {

		return _bitmap;

	}
	const SizeI& Tileset::GridSize() const {

		return _tile_size;

	}
	const SizeI& Tileset::TileSize() const {
		return _tile_size;
	}

	Tileset Tileset::FromFile(const std::string& file) {

		return FromFile(file, SizeI(0, 0));

	}
	Tileset Tileset::FromFile(const std::string& file, const SizeI& tile_size) {

		Graphics::Bitmap image = Graphics::Bitmap::FromFile(file);
		std::string meta_path = IO::Path::SetExtension(file, ".xml");

		if (tile_size.width == 0 && tile_size.height == 0)
			assert(IO::File::Exists(meta_path));

		if (IO::File::Exists(meta_path)) {

			Xml::XmlDocument meta = Xml::XmlDocument::Open(IO::Path::SetExtension(file, ".xml"));

			int tile_width = StringUtils::Parse<int>(meta.Root().GetAttribute("tile_width"));
			int tile_height = StringUtils::Parse<int>(meta.Root().GetAttribute("tile_height"));
			
			Tileset tileset(image, SizeI(tile_width, tile_height));

			std::stringstream flags;
			flags << meta.Root().GetChild("flags").Text();
			
			for (size_t i = 0; i < tileset.Count(); ++i) {
				flags >> tileset.GetAt(i).flag;
				flags.ignore(1);
			}

			return tileset;

		}

		return Tileset(image, tile_size);

	}


	Tileset::Tile::Tile(Graphics::Bitmap&& bitmap) :
		bitmap(std::move(bitmap)) {

		this->flag = 0;

	}

	//Tileset Tileset::GenerateAutoTiles(const Tileset& autotiles) {

	//	//// Each autotile set is composed of 2 columns of 3 tiles (6 tiles total).
	//	//// There will be 48 combinations generated for each autotile.

	//	//// If the dimensions of the tileset don't match, through an exception.
	//	//if (autotiles.Rows() % 2 != 0 || autotiles.Columns() % 3 != 0 || (autotiles.TileSize().width != autotiles.TileSize().height))
	//	//	throw ArgumentException("The width of the tileset must be a multiple of 2 and the height a multiple of 3, and tiles must be square.");

	//	//// We will store the 48 generated tiles in 1 1/2 rows of 32 tiles (width of 1024px for 32px tiles).
	//	//int autotile_count = (autotiles.Rows() / 2) * (autotiles.Columns() / 3);
	//	//int bmp_w = autotiles.TileSize().width * 32;
	//	//int bmp_h = static_cast<int>(Math::Ceiling(1.5f * autotile_count)) * autotiles.TileSize().height;
	//	//Graphics::Bitmap bmp(bmp_w, bmp_h);

	//	//Graphics::Graphics gfx(bmp);
	//	//gfx.DrawBitmap(0, 0, autotiles.TileAt(0));
	//	//
	//	//bmp.Save("test.png");

	//	//return Tileset(bmp, SizeI(32, 32));

	//}

}