#include "assets/BitmapAssetLoader.h"
#include "assets/FileAssetLoader.h"
#include "graphics/Imaging.h"
#include "io/Path.h"

namespace hvn3 {

	BitmapAssetLoader::BitmapAssetLoader() :
		BitmapAssetLoader(std::make_unique<FileAssetLoader>()) {}
	BitmapAssetLoader::BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader) :
		_loader(std::move(loader)) {}
	BitmapAssetLoader::BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader) :
		_loader(std::move(loader)) {}

	BitmapAssetLoader::asset_type BitmapAssetLoader::LoadData(const std::string& path) {
		
		// Load the raw binary data using the binary asset loader.
		IBinaryAssetLoader::asset_type data = _loader->LoadData(path);
		
		// Create a new asset from that data.
		Imaging::ImageFormat format = Imaging::FileExtensionToImageFormat(IO::Path::GetExtension(path));
		Drawing::Bitmap asset(data.first, data.second, format);
	
		// We don't need the data buffer anymore, so it can be freed.
		_loader->UnloadData(data);

		return asset;

	}
	void BitmapAssetLoader::UnloadData(asset_type& asset) {

		// Calling the destructor will free the resource and set its data pointer to null.
		asset.~Bitmap();

	}
	BitmapAssetLoader::asset_type BitmapAssetLoader::LoadNull() {
	
		return Drawing::Bitmap((ALLEGRO_BITMAP*)nullptr, false);

	}

}