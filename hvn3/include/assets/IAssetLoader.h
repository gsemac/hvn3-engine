#pragma once
#include <string>

namespace hvn3 {

	template <typename asset_type>
	struct AssetLoaderResult {
		AssetLoaderResult(asset_type& data, unsigned int size, bool success) :
		Data(std::move(data)) {
			Size = size;
			Success = success;
		}
		asset_type Data;
		unsigned int Size;
		bool Success;
	};

	template <typename asset_type>
	class IAssetLoader {

	public:
		typedef asset_type asset_type;

		virtual AssetLoaderResult<asset_type> LoadData(const std::string& path) = 0;
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) = 0;
		virtual AssetLoaderResult<asset_type> GetNull() = 0;

	};

}