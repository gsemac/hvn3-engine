#include "assets/AssetLoadEventArgs.h"

namespace hvn3 {

	AssetLoadEventArgs::AssetLoadEventArgs(unsigned int assets_loaded, unsigned int total_assets) {

		_assets_loaded = assets_loaded;
		_total_assets = total_assets;

	}

	unsigned int AssetLoadEventArgs::AssetsLoaded() const {

		return _assets_loaded;

	}
	unsigned int AssetLoadEventArgs::TotalAssets() const {

		return _total_assets;

	}
	float AssetLoadEventArgs::Percentage() const {

		// This way we can reliably checked for exactly 100% without worrying about rounding error.
		if (_assets_loaded == _total_assets)
			return 100.0f;

		return (static_cast<float>(_assets_loaded) / _total_assets) * 100.0f;

	}

}