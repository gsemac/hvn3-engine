#include "hvn3/assets/FileAssetLoader.h"
#include <fstream>

namespace hvn3 {

	AssetLoaderResult<FileAssetLoader::asset_type> FileAssetLoader::LoadData(const asset_args_type& path) {

		asset_type data;

		// Seek to the end of the file immediately so we can get the file size.
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();

		// Seek back to the beginning of the file to read it into the buffer.
		file.seekg(0, std::ios::beg);

		// If the file was empty or couldn't be read, return null data.
		if (size <= 0)
			return AssetLoaderResult<asset_type>(data, static_cast<unsigned int>(size), false);

		// Otherwise, allocate a buffer for storing the data from the file.
		data = new uint8_t[size];

		// Read contents of file into the buffer.
		file.read((char*)data, size);

		return AssetLoaderResult<asset_type>(data, static_cast<unsigned int>(size), true);

	}
	void FileAssetLoader::FreeData(AssetLoaderResult<asset_type>& asset) {

		if (asset.data != nullptr)
			delete[] asset.data;

		asset.data = nullptr;
		asset.size = 0;
		asset.success = false;

	}
	AssetLoaderResult<FileAssetLoader::asset_type> FileAssetLoader::NullData() {

		asset_type data = nullptr;

		return AssetLoaderResult<asset_type>(data, 0, false);

	}
}