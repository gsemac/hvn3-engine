#include "hvn3/assets/SystemAssets.h"

namespace hvn3 {
	namespace System {

		std::string GetSystemAssetPath() {

			return HVN3_SYSTEM_ASSET_PATH;

		}
		std::string GetSystemAssetPath(SystemAssetType type) {

			switch (type) {
			case SystemAssetType::Fonts:
				return GetSystemAssetPath() + "fonts/";
			case SystemAssetType::Graphics:
				return GetSystemAssetPath() + "graphics/";
			case SystemAssetType::Icons:
				return GetSystemAssetPath() + "icons/";
			default:
				return GetSystemAssetPath();
			}

		}

	}
}