#include "hvn3/core/SystemAssets.h"

namespace hvn3 {
	namespace System {

		std::string GetSystemAssetPath() {

			return HVN3_SYSTEM_ASSET_PATH;

		}
		std::string GetSystemAssetPath(SystemAssetType type) {

			switch (type) {
			case SystemAssetType::Fonts:
				return GetSystemAssetPath() + "Fonts/";
			default:
				return GetSystemAssetPath();
			}

		}

	}
}