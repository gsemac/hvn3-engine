#pragma once
#include <string>

namespace hvn3 {
	namespace System {

		enum class SystemAssetType {
			Fonts,
			Graphics,
			Icons
		};

		static const char* HVN3_SYSTEM_ASSET_PATH = "bin/system/";
		
		std::string GetSystemAssetPath();
		std::string GetSystemAssetPath(SystemAssetType type);

	}
}