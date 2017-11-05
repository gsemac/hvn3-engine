#include "assets/BitmapAssetLoader.h"
#include "assets/FileAssetLoader.h"
#include "graphics/Imaging.h"
#include "io/Path.h"

namespace hvn3 {

	BitmapAssetLoader::BitmapAssetLoader() :
		BitmapAssetLoader(std::make_unique<FileAssetLoader>()) {
	}
	BitmapAssetLoader::BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader) :
		_loader(std::move(loader)) {
	}
	BitmapAssetLoader::BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader) :
		_loader(std::move(loader)) {
	}

	AssetLoaderResult<BitmapAssetLoader::asset_type> BitmapAssetLoader::LoadData(const std::string& path) {

		// Load the raw binary data using the binary asset loader.
		auto binary_data = _loader->LoadData(path);
		bool success = binary_data.Success;

		// Create a new asset from that data.
		Imaging::ImageFormat format = Imaging::FileExtensionToImageFormat(IO::Path::GetExtension(path));
		Drawing::Bitmap asset(binary_data.Data, binary_data.Size, format);

		// We don't need the data buffer anymore, so it can be freed.
		_loader->FreeData(binary_data);

		return AssetLoaderResult<asset_type>(asset, binary_data.Size, success);

	}
	void BitmapAssetLoader::FreeData(AssetLoaderResult<asset_type>& asset) {

		// Calling the destructor will free the resource and set its data pointer to null.
		asset.Data.~Bitmap();

		asset.Size = 0;

	}
	AssetLoaderResult<BitmapAssetLoader::asset_type> BitmapAssetLoader::GetNull() {

		return AssetLoaderResult<asset_type>(Drawing::Bitmap((ALLEGRO_BITMAP*)nullptr, false), 0, false);

	}

}