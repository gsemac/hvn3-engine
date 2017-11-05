#pragma once
#include "IAssetLoader.h"
#include "graphics/Bitmap.h"
#include <memory>

namespace hvn3 {

	class IBinaryAssetLoader;

	class BitmapAssetLoader : public IAssetLoader<Drawing::Bitmap> {

	public:
		BitmapAssetLoader();
		BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader);
		BitmapAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader);
		
		virtual AssetLoaderResult<asset_type> LoadData(const std::string& path) override;
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) override;
		virtual AssetLoaderResult<asset_type> GetNull() override;

	private:
		std::unique_ptr<IBinaryAssetLoader> _loader;

	};

}