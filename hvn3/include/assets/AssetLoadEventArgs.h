#include "events/EventArgs.h"

namespace hvn3 {

	class AssetLoadEventArgs : public System::EventArgs {

	public:
		AssetLoadEventArgs(unsigned int assets_loaded, unsigned int total_assets);

		unsigned int AssetsLoaded() const;
		unsigned int TotalAssets() const;
		float Percentage() const;

	private:
		unsigned int _assets_loaded;
		unsigned int _total_assets;

	};

}