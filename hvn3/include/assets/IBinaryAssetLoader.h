#pragma once 
#include "IAssetLoader.h"
#include <utility>
#include <cstdint>

namespace hvn3 {

	class IBinaryAssetLoader : public IAssetLoader<uint8_t*> {
		
	public:
		virtual AssetLoaderResult<asset_type> LoadData(const std::string& path) override = 0;
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) override = 0;
		virtual AssetLoaderResult<asset_type> GetNull() = 0;

	};

}