#pragma once
#include <string>

namespace hvn3 {

	template <typename asset_type>
	class IAssetLoader {

	public:
		typedef asset_type asset_type;

		virtual asset_type LoadData(const std::string& path) = 0;
		virtual void UnloadData(asset_type& asset) = 0;
		virtual asset_type LoadNull() = 0;

	};

}