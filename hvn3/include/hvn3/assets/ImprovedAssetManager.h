#pragma once

/*
The following classes are meant to be an improvement over the current AssetManager and related classes.
The goal is to simplify the interface and the implementation of different asset loaders.

#todo Eventually the classes here will replace the existing ones altogether once the functionality is fully duplicated.
*/

#include "hvn3/core/CoreDefs.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/graphics/Imaging.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/sprites/Sprite.h"

#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace hvn3 {

	namespace Preview {

		typedef uint8_t Byte;

		typedef int AssetId;
		typedef AssetId AssetGroupId;

		enum : AssetId {
			NULL_ASSET_ID = -1
		};

		class AssetLoaderResult {

		public:
			Byte* begin() {
				return _data.data();
			}
			Byte* end() {
				return _data.data() + _data.size();
			}
			const Byte* begin() const {
				return _data.data();
			}
			const Byte* end() const {
				return _data.data() + _data.size();
			}
			size_t size() const {
				return _data.size();
			}

			void Resize(size_t bytes) {
				_data.resize(bytes);
			}
			bool IsEmpty() const {
				return size() <= 0;
			}

		private:
			std::vector<Byte> _data;

		};

		template<typename KeyType>
		class IAssetLoader {

		public:
			virtual AssetLoaderResult Load(const KeyType& key) = 0;

		};

		class FileSystemAssetLoader :
			IAssetLoader<std::string> {

		public:
			AssetLoaderResult Load(const std::string& key) override {

				AssetLoaderResult result;

				// Seek to the end of the file immediately so we can get the file size.
				std::ifstream file(key, std::ios::binary | std::ios::ate);
				std::streamsize size = file.tellg();

				// Seek back to the beginning of the file to read it into the buffer.
				file.seekg(0, std::ios::beg);

				// If the file was empty or couldn't be read, return null data.
				if (size <= 0)
					return result;

				// Read contents of file into the buffer.
				result.Resize(static_cast<size_t>(size));
				file.read(reinterpret_cast<char*>(result.begin()), size);

				return result;

			}

		private:

		};

		template<typename AssetType, typename KeyType = AssetId, typename PathType = std::string>
		class AssetCollection {

		public:
			typedef AssetType asset_type;
			typedef KeyType key_type;
			typedef PathType path_type;

		private:
			struct AssetGroup {
				std::unordered_map<key_type, path_type> paths;
				std::unordered_map<key_type, asset_type> assets;
			};

			typedef AssetGroup asset_group_type;
			typedef key_type asset_group_key_type;
			typedef std::function<asset_type(const path_type&, const AssetLoaderResult&)> callback_type;

			template<typename AssetType, typename PathType>
			void _setDefaultAssetLoaderAndCallback(AssetCollection<AssetType, key_type, PathType>& asset_collection) {}
			template<>
			void _setDefaultAssetLoaderAndCallback(AssetCollection<hvn3::Graphics::Bitmap, key_type, std::string>& asset_collection) {

				SetAssetLoader(std::unique_ptr<IAssetLoader<std::string>>(new FileSystemAssetLoader));
				SetAssetLoaderCallback([this](const std::string& key, const AssetLoaderResult& result) {

					hvn3::Imaging::ImageFormat format = hvn3::Imaging::FileExtensionToImageFormat(hvn3::IO::Path::GetExtension(key));
					hvn3::Graphics::Bitmap asset = hvn3::Graphics::Bitmap::FromBuffer(result.begin(), result.size(), format);

					return asset;

				});

			}

		public:
			AssetCollection() {

				_setDefaultAssetLoaderAndCallback<AssetType, PathType>(*this);
				_deferred_loading_enabled = true;
				_byte_count = 0;

			}

			void Add(const key_type& key, const path_type& path) {

				_registerAssetIfNotExists(_default_group, key, path);

				if (!_deferred_loading_enabled)
					_loadAssetIfNotLoaded(_default_group, key, path);

			}
			void Add(const key_type& key, const asset_type& asset) {
				_addAssetIfNotExists(_default_group, key, asset);
			}
			void Add(const asset_group_key_type& group, const key_type& key, const path_type& path) {

				auto g = _createGroupIfNotExists(group);

				_registerAssetIfNotExists(*g, key, path);

			}
			void Add(const asset_group_key_type& group, const key_type& key, const asset_type& asset) {

				auto g = _createGroupIfNotExists(group);

				_addAssetIfNotExists(*g, key, asset);

			}

			asset_type& Get(const key_type& key) {

				_loadAssetIfNotLoaded(_default_group, key);

				return _default_group.assets[key];

			}
			asset_type& Get(const asset_group_key_type& group, const key_type& key) {

				auto g = _createGroupIfNotExists(group);

				_loadAssetIfNotLoaded(*g, key);

				return g->assets[key];

			}

			void SetAssetLoaderCallback(callback_type&& callback) {

				_callback = std::move(callback);

			}
			void SetAssetLoader(std::unique_ptr<IAssetLoader<path_type>>& asset_loader) {

				_loader = std::move(asset_loader);

			}
			void SetAssetLoader(std::unique_ptr<IAssetLoader<path_type>>& asset_loader, callback_type&& callback) {

				SetAssetLoader(asset_loader);
				SetAssetLoaderCallback(std::move(callback));

			}

			bool DeferredLoadingEnabled() const {
				return _deferred_loading_enabled;
			}
			void SetDeferredLoadingEnabled(bool value) {
				_deferred_loading_enabled = value;
			}

			size_t AssetCount() const {

				size_t count = _default_group.assets.size();

				for (auto i = _groups.begin(); i != _groups.end(); ++i)
					count += i->second.assets.size();

				return count;

			}
			size_t GroupCount() const {
				return _groups.size() + 1;
			}

			void Clear() {

				_default_group.assets.clear();
				_default_group.paths.clear();

				_groups.clear();

			}

			asset_type& operator[](const key_type& key) {
				return Get(key);
			}

		private:
			typedef std::unordered_map<asset_group_key_type, asset_group_type> asset_group_map_type;

			asset_group_type _default_group;
			asset_group_map_type _groups;
			std::unique_ptr<IAssetLoader<path_type>> _loader;
			callback_type _callback;
			bool _deferred_loading_enabled;
			size_t _byte_count;

			asset_group_type* _createGroupIfNotExists(const asset_group_key_type& group) {

				auto it = _groups.find(group);

				if (it == _groups.end())
					return &_groups.insert(std::make_pair(group, asset_group_type())).first->second;

				return &it->second;

			}
			bool _isAssetRegistered(asset_group_type& group, const key_type& key) {

				// Has this asset already been loaded? If so, do nothing.
				if (group.assets.count(key) > 0)
					return true;

				// Has this asset already been registered? If so, do nothing.
				if (group.paths.count(key) > 0)
					return true;

				// The asset has not been loaded or registered.
				return false;

			}
			void _loadAssetIfNotLoaded(asset_group_type& group, const key_type& key) {

				// If the asset has already been loaded, do nothing.

				auto loaded_it = group.assets.find(key);

				if (loaded_it != group.assets.end())
					return;

				// Get the path for the asset.

				auto path_it = group.paths.find(key);

				HVN3_ASSERT(path_it != group.paths.end(), "No asset with the given key has been registered.");

				// Load the asset.

				_loadAssetIfNotLoaded(group, key, path_it->second);

			}
			void _loadAssetIfNotLoaded(asset_group_type& group, const key_type& key, const path_type& path) {

				// Register this asset if it hasn't already been registered.
				_registerAssetIfNotExists(group, key, path);

				// If the asset has already been loaded, do nothing.

				auto loaded_it = group.assets.find(key);

				if (loaded_it != group.assets.end())
					return;

				// Load the asset.

				HVN3_ASSERT(static_cast<bool>(_loader), "Asset loader was null.");

				auto asset_data = _loader->Load(path);

				HVN3_ASSERT(asset_data.size() > 0, "Asset could not be loaded.");
				HVN3_ASSERT(static_cast<bool>(_callback), "Asset loader callback was null.");

				auto asset = _callback(path, asset_data);

				group.assets[key] = std::move(asset);

			}
			void _registerAssetIfNotExists(asset_group_type& group, const key_type& key, const path_type& path) {

				if (_isAssetRegistered(group, key))
					return;

				group.paths[key] = path;

			}
			void _addAssetIfNotExists(asset_group_type& group, const key_type& key, const asset_type& asset) {

				if (_isAssetRegistered(group, key))
					return;

				group.assets[key] = asset;

			}

		};

		class AssetManager :
			public hvn3::IManager {

		public:
			AssetManager() = default;

			AssetCollection<hvn3::Graphics::Bitmap>& Bitmaps() {
				return _bitmaps;
			}
			hvn3::Graphics::Bitmap& Bitmaps(AssetId id) {
				return _bitmaps.Get(id);
			}
			AssetCollection<hvn3::Sprite>& Sprites() {
				return _sprites;
			}
			hvn3::Sprite& Sprites(AssetId id) {
				return _sprites.Get(id);
			}

			void Clear() {

				_bitmaps.Clear();
				_sprites.Clear();

			}

		private:
			AssetCollection<hvn3::Graphics::Bitmap> _bitmaps;
			AssetCollection<hvn3::Sprite> _sprites;

		};

	}

	template<>
	struct ManagerIdTraits<ASSET_MANAGER> {
		typedef Preview::AssetManager type;
	};

}