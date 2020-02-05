#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/io/Path.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/tiles/Tileset.h"
#include "hvn3/xml/xml_document.h"

#include <cassert>
#include <sstream>

namespace hvn3::tiles {

	// Public members

	Tileset::Tile::Tile(Graphics::Bitmap&& image) :
		image(std::move(image)) {
	}
	const Graphics::Bitmap& Tileset::Tile::Image() const {

		return image;

	}

	Tileset::Tileset(const Graphics::Bitmap& image, const SizeI& tileSize) :
		Tileset(image, tileSize, PointI(0, 0), PointI(0, 0)) {
	}
	Tileset::Tileset(const Graphics::Bitmap& image, const SizeI& tileSize, const PointI& offset, const PointI& separation) :
		image(image),
		tileSize(tileSize) {

		// Initialize column count.

		rows = 0;

		// Get width/height as unsigned integers.

		int w = tileSize.Width();
		int h = tileSize.Height();

		assert(w > 0);
		assert(w > 0);

		// Generate a vector of sub-bitmaps to represent each tile.

		for (int y = offset.Y(); y < image.Height(); y += h + separation.Y()) {

			// Insert the next row of tiles.

			for (int x = offset.X(); x < image.Width(); x += w + separation.X())
				tiles.push_back(Tile(Graphics::Bitmap(image, RectangleI(x, y, Math::Min(w, image.Width() - x), Math::Min(h, image.Height() - y)))));

			// Increment the number of rows.

			++rows;

		}

	}
	Tileset::Tileset(const Graphics::Bitmap& image, int tileWidth, int tileHeight) :
		Tileset(image, SizeI(tileWidth, tileHeight)) {
	}

	const Tileset::Tile& Tileset::GetTile(int x, int y) const {

		assert(x >= 0);
		assert(y >= 0);

		return tiles[y * Columns() + x];

	}
	const Tileset::Tile& Tileset::GetTile(int n) const {

		assert(n >= 0);

		return tiles[n];

	}

	Tileset::size_type Tileset::Rows() const {

		return rows;

	}
	Tileset::size_type Tileset::Columns() const {

		return tiles.size() / rows;

	}

	Tileset::size_type Tileset::Count() const {

		return tiles.size();

	}

	const Graphics::Bitmap& Tileset::Image() const {

		return image;

	}

	const SizeI& Tileset::Size() const {

		return SizeI(Rows(), Columns());

	}
	const SizeI& Tileset::TileSize() const {

		return tileSize;

	}

	/*Tileset Tileset::FromFile(const std::string& file) {

		return FromFile(file, SizeI(0, 0));

	}
	Tileset Tileset::FromFile(const std::string& file, const SizeI& tile_size) {

		Graphics::Bitmap image = Graphics::Bitmap::FromFile(file);
		std::string meta_path = IO::Path::SetExtension(file, ".xml");

		if (tile_size.width == 0 && tile_size.height == 0)
			assert(IO::File::Exists(meta_path));

		if (IO::File::Exists(meta_path)) {

			xml::XmlDocument meta = xml::XmlDocument::FromFile(IO::Path::SetExtension(file, ".xml"));

			int tile_width = StringUtils::Parse<int>(meta.Root().GetAttribute("tile_width"));
			int tile_height = StringUtils::Parse<int>(meta.Root().GetAttribute("tile_height"));

			Tileset tileset(image, SizeI(tile_width, tile_height));

			std::stringstream flags;
			flags << meta.Root().GetChild("flags").Text();

			for (size_t i = 0; i < tileset.Count(); ++i) {
				flags >> tileset.TileAt(i).flag;
				flags.ignore(1);
			}

			return tileset;

		}

		return Tileset(image, tile_size);

	}*/

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