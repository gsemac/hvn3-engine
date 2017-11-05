#include "events/EventArgs.h"
#include "AssetTypeDefs.h"

namespace hvn3 {

	enum ASSET_LOADER_EVENT_STATUS {
		// The asset manager has begin loading the asset.
		ASSET_LOADER_EVENT_STATUS_STARTED,
		// The asset manager has finished loading the asset.
		ASSET_LOADER_EVENT_STATUS_FINISHED,
		// The asset manager failed to load the asset.
		ASSET_LOADER_EVENT_STATUS_FAILED,
		// The asset was removed from the asset manager.
		ASSET_LOADER_EVENT_STATUS_REMOVED,
		// The asset was already loaded.
		ASSET_LOADER_EVENT_STATUS_ALREADY_LOADED
	};

	class AssetLoaderEventArgs : public System::EventArgs {

	public:
		AssetLoaderEventArgs(unsigned int assets_loaded, unsigned int total_assets, unsigned int size, ASSET_LOADER_EVENT_STATUS status, AssetId id, AssetGroupId group);

		// Returns the number of assets loaded so far.
		unsigned int LoadedAssets() const;
		// Returns the total number of assets.
		unsigned int TotalAssets() const;

		// Returns a percentage representing the total completion.
		float Progress() const;
		// Returns true if the asset loading process is complete.
		bool Complete() const;
		// Returns the size of the loaded asset in bytes.
		unsigned int Size() const;
		// Returns the status of the asset for which the event was generated.
		ASSET_LOADER_EVENT_STATUS Status() const;

		// Returns the asset id of the loaded asset.
		AssetId Id() const;
		// Returns the group id of the loaded asset.
		AssetGroupId Group() const;

	private:
		unsigned int _assets_loaded;
		unsigned int _total_assets;
		unsigned int _size;
		ASSET_LOADER_EVENT_STATUS _status;
		AssetId _asset_id;
		AssetGroupId _group_id;

	};

}