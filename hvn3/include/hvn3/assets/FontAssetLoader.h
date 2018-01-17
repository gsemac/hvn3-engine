#pragma once
#include "IAssetLoader.h"
#include "IBinaryAssetLoader.h"
#include "hvn3/fonts/Font.h"
#include <memory>

namespace hvn3 {

	struct FontAssetArgs {

		FontAssetArgs(const std::string& path, int size, FontFlags flags = static_cast<FontFlags>(0));

		std::string path;
		int size;
		FontFlags flags;

	};

	class FontAssetLoader : public IAssetLoader<Font, FontAssetArgs> {

	public:
		FontAssetLoader();
		FontAssetLoader(std::unique_ptr<IBinaryAssetLoader>& loader);
		FontAssetLoader(std::unique_ptr<IBinaryAssetLoader>&& loader);

		virtual AssetLoaderResult<asset_type> LoadData(const asset_args_type& args) override;
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) override;
		virtual AssetLoaderResult<asset_type> NullData() override;

	private:
		std::unique_ptr<IBinaryAssetLoader> _loader;

	};

}