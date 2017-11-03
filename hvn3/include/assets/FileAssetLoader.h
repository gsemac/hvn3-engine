#pragma once
#include "IBinaryAssetLoader.h"
#include <cstdint>
#include <vector>

namespace hvn3 {

	class FileAssetLoader : public IBinaryAssetLoader {

	public:
		asset_type LoadData(const std::string& path) override;
		void FreeData(asset_type& asset) override;
		asset_type GetNull() override;

	};

}