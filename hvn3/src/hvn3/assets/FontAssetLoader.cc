#include "hvn3/assets/FontAssetLoader.h"
#include "hvn3/assets/FileAssetLoader.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/io/Path.h"

namespace hvn3 {

	FontAssetArgs::FontAssetArgs(const std::string& path, int size, FontOptions options) :
		path(path),
		size(size),
		options(options) {
	}



	FontAssetLoader::FontAssetLoader() :
		FontAssetLoader(std::make_unique<FileAssetLoader>()) {
	}
	FontAssetLoader::FontAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader) :
		_loader(std::move(loader)) {
	}
	FontAssetLoader::FontAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader) :
		_loader(std::move(loader)) {
	}

	AssetLoaderResult<FontAssetLoader::asset_type> FontAssetLoader::LoadData(const asset_args_type& args) {

		// Load the raw binary data using the binary asset loader.
		auto binary_data = _loader->LoadData(args.path);
		bool success = binary_data.success;

		// Create a new asset from that data.
		Font asset(binary_data.data, binary_data.size, args.size, FontFormat::TrueTypeFont);

		// We don't need the data buffer anymore, so it can be freed.
		_loader->FreeData(binary_data);

		return AssetLoaderResult<asset_type>(asset, binary_data.size, success);

	}
	void FontAssetLoader::FreeData(AssetLoaderResult<asset_type>& asset) {

		// Calling the destructor will free the resource and set its data pointer to null.
		asset.data.~Font();

		asset.size = 0;

	}
	AssetLoaderResult<FontAssetLoader::asset_type> FontAssetLoader::NullData() {

		return AssetLoaderResult<asset_type>(Font(), 0, false);

	}

}