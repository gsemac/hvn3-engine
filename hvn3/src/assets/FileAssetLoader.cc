#include "assets/FileAssetLoader.h"
#include <fstream>

namespace hvn3 {

	FileAssetLoader::asset_type FileAssetLoader::LoadData(const std::string& path) {
		
		asset_type data;
		data.first = nullptr;
		data.second = 0;

		// Seek to the end of the file immediately so we can get the file size.
		std::ifstream file(path, std::ios::binary | std::ios::ate);
		std::streamsize size = file.tellg();

		// Seek back to the beginning of the file to read it into the buffer.
		file.seekg(0, std::ios::beg);
		
		// If the file was empty or couldn't be read, return null data.
		if (size <= 0)
			return data;

		// Otherwise, allocate a buffer for storing the data from the file.
		data.first = new uint8_t[size];
		data.second = (size_t)size;

		// Read contents of file into the buffer.
		file.read((char*)data.first, size);
		
		return data;

	}
	void FileAssetLoader::UnloadData(asset_type& asset) {

		delete[] asset.first;

		asset.first = nullptr;
		asset.second = 0;

	}
	FileAssetLoader::asset_type FileAssetLoader::LoadNull() {

		asset_type null;
		null.first = nullptr;
		null.second = 0;

		return null;

	}
}