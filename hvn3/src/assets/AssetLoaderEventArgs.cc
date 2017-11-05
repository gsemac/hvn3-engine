#include "assets/AssetLoaderEventArgs.h"

namespace hvn3 {

	AssetLoaderEventArgs::AssetLoaderEventArgs(unsigned int assets_loaded, unsigned int total_assets, unsigned int size, ASSET_LOADER_EVENT_STATUS status, AssetId id, AssetGroupId group) {

		_assets_loaded = assets_loaded;
		_total_assets = total_assets;
		_size = size;
		_status = status;
		_asset_id = id;
		_group_id = group;

	}

	unsigned int AssetLoaderEventArgs::LoadedAssets() const {

		return _assets_loaded;

	}
	unsigned int AssetLoaderEventArgs::TotalAssets() const {

		return _total_assets;

	}
	float AssetLoaderEventArgs::Progress() const {

		if (_total_assets <= 0)
			return 0.0f;

		// This way we can reliably checked for exactly 100% without worrying about rounding error.
		if (_assets_loaded == _total_assets)
			return 100.0f;

		return (static_cast<float>(_assets_loaded) / _total_assets) * 100.0f;

	}
	bool AssetLoaderEventArgs::Complete() const {

		return Progress() == 100.0f;

	}

	unsigned int AssetLoaderEventArgs::Size() const {

		return _size;

	}
	ASSET_LOADER_EVENT_STATUS AssetLoaderEventArgs::Status() const {

		return _status;

	}
	AssetId AssetLoaderEventArgs::Id() const {

		return _asset_id;

	}
	AssetGroupId AssetLoaderEventArgs::Group() const {

		return _group_id;

	}

}