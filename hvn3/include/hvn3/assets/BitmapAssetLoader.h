#pragma once
#include "IAssetLoader.h"
#include "IBinaryAssetLoader.h"
#include "hvn3/graphics/Bitmap.h"
#include <memory>

namespace hvn3 {

	struct BitmapAssetArgs {

		BitmapAssetArgs(const std::string& path);
		BitmapAssetArgs(const char* path);

		std::string path;

	};

	class BitmapAssetLoader : public IAssetLoader<Graphics::Bitmap, BitmapAssetArgs> {

	public:
		BitmapAssetLoader();
		BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader);
		BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader);
		
		virtual AssetLoaderResult<asset_type> LoadData(const asset_args_type& args) override;
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) override;
		virtual AssetLoaderResult<asset_type> NullData() override;

	private:
		std::unique_ptr<IBinaryAssetLoader> _loader;

	};

}