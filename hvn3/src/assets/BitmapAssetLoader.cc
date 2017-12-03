#include "assets/BitmapAssetLoader.h"
#include "assets/FileAssetLoader.h"
#include "graphics/Imaging.h"
#include "io/Path.h"

namespace hvn3 {

	BitmapAssetArgs::BitmapAssetArgs(const std::string& path) :
		path(path) {
	}
	BitmapAssetArgs::BitmapAssetArgs(const char* path) :
		BitmapAssetArgs(path) {
	}



	BitmapAssetLoader::BitmapAssetLoader() :
		BitmapAssetLoader(std::make_unique<FileAssetLoader>()) {
	}
	BitmapAssetLoader::BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader) :
		_loader(std::move(loader)) {
	}
	BitmapAssetLoader::BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader) :
		_loader(std::move(loader)) {
	}

	AssetLoaderResult<BitmapAssetLoader::asset_type> BitmapAssetLoader::LoadData(const asset_args_type& args) {

		// Load the raw binary data using the binary asset loader.
		auto binary_data = _loader->LoadData(args.path);
		bool success = binary_data.success;

		// Create a new asset from that data.
		Imaging::ImageFormat format = Imaging::FileExtensionToImageFormat(IO::Path::GetExtension(args.path));
		Graphics::Bitmap asset(binary_data.data, binary_data.size, format);

		// We don't need the data buffer anymore, so it can be freed.
		_loader->FreeData(binary_data);

		return AssetLoaderResult<asset_type>(asset, binary_data.size, success);

	}
	void BitmapAssetLoader::FreeData(AssetLoaderResult<asset_type>& asset) {

		// Calling the destructor will free the resource and set its data pointer to null.
		asset.data.~Bitmap();

		asset.size = 0;

	}
	AssetLoaderResult<BitmapAssetLoader::asset_type> BitmapAssetLoader::NullData() {

		return AssetLoaderResult<asset_type>(Graphics::Bitmap(nullptr, false), 0, false);

	}

}