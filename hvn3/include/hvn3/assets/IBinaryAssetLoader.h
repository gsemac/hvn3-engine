#pragma once 
#include "hvn3/assets/IAssetLoader.h"
#include <cstdint>
#include <string>

namespace hvn3 {
	
	class IBinaryAssetLoader : public IAssetLoader<uint8_t*, std::string> {
		
	public:
		virtual AssetLoaderResult<asset_type> LoadData(const asset_args_type& path) override = 0;
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) override = 0;
		virtual AssetLoaderResult<asset_type> NullData() override = 0;

	};

}