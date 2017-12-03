#pragma once
#include "hvn3/assets/AssetTypeDefs.h"

namespace hvn3 {

	template <typename asset_type>
	class AssetHandle {

	public:
		AssetHandle() :
			AssetHandle(nullptr) {
		}
		AssetHandle(asset_type* resource) :
			_resource(resource) {
		}

		AssetId Id() const {

			return _id;

		}
		bool IsNull() const {

			return _resource == nullptr;

		}

		explicit operator bool() const {

			return !IsNull();

		}
		operator asset_type*() const {

			return _resource;

		}

		asset_type* operator->() const {

			return _resource;

		}
		asset_type& operator*() const {

			return *_resource;

		}

		bool operator==(const AssetHandle<asset_type> other) const {

			return _resource == other._resource;

		}
		bool operator!=(const AssetHandle<asset_type> other) const {

			return !(*this == other);

		}

	private:
		asset_type* _resource;
		AssetId _id;

	};

}