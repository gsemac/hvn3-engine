#pragma once
#include <string>

namespace hvn3 {
	namespace System {

		enum class SystemAssetType {
			Fonts
		};

		static const char* HVN3_SYSTEM_ASSET_PATH = "data/System/";
		
		std::string GetSystemAssetPath();
		std::string GetSystemAssetPath(SystemAssetType type);

	}
}