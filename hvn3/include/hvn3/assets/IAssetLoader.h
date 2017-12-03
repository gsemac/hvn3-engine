#pragma once

namespace hvn3 {

	template <typename asset_type>
	struct AssetLoaderResult {

		AssetLoaderResult(asset_type& data, unsigned int size, bool success) :
			data(std::move(data)),
			size(size),
			success(success) {
		}

		asset_type data;
		unsigned int size;
		bool success;

	};

	template <typename asset_type, typename asset_args_type>
	class IAssetLoader {

	public:
		typedef asset_type asset_type;
		typedef asset_args_type asset_args_type;

		// Loads an asset using the given asset arguments and returns the result.
		virtual AssetLoaderResult<asset_type> LoadData(const asset_args_type& args) = 0;
		// Frees the data associated with the given asset.
		virtual void FreeData(AssetLoaderResult<asset_type>& asset) = 0;
		// Returns a null asset that does not have any data associated with it.
		virtual AssetLoaderResult<asset_type> NullData() = 0;

	};

}