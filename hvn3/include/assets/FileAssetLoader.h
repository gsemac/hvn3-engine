#pragma once
#include "IBinaryAssetLoader.h"
#include <cstdint>
#include <vector>

namespace hvn3 {

	class FileAssetLoader : public IBinaryAssetLoader {

	public:
		AssetLoaderResult<asset_type> LoadData(const asset_args_type& path) override;
		void FreeData(AssetLoaderResult<asset_type>& asset) override;
		AssetLoaderResult<asset_type> NullData() override;

	};

}