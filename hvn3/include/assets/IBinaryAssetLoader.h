#pragma once 
#include "IAssetLoader.h"
#include <utility>
#include <cstdint>

namespace hvn3 {

	class IBinaryAssetLoader : public IAssetLoader<std::pair<uint8_t*, size_t> > {
		
	public:
		virtual asset_type LoadData(const std::string& path) override = 0;
		virtual void FreeData(asset_type& asset) override = 0;
		virtual asset_type GetNull() = 0;

	};

}