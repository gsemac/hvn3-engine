#pragma once
#include "AssetHandle.h"
#include "IAssetLoader.h"
#include "AssetLoaderEventArgs.h"
#include "utility/ThreadPool.h"
#include <functional>
#include <limits>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>

namespace hvn3 {

	template <typename asset_type>
	class AssetManager {

	private:
		struct Asset {

			Asset(std::string Path, AssetLoaderResult<asset_type>& Data, bool IsLoaded) :
				Path(Path),
				Data(std::move(Data)),
				IsLoaded(IsLoaded) {
			}

			AssetLoaderResult<asset_type> Data;
			std::string Path;
			bool IsLoaded;

		};

		typedef std::unordered_map<AssetId, Asset> asset_map_type;
		typedef std::unordered_map<AssetGroupId, asset_map_type> group_map_type;

	public:
		typedef asset_type asset_type;
		static const AssetGroupId DefaultGroupId = 0;

		AssetManager(std::unique_ptr<IAssetLoader<asset_type> >& loader) :
			_loader(std::move(loader)) {

			_next_id = 0;

		}
		~AssetManager() {

			RemoveAll();

		}

		AssetId Add(const std::string& path, bool auto_load = true) {

			AssetId id = _getNextFreeId();

			Add(id, path, auto_load);

			return id;

		}
		bool Add(AssetId id, const std::string& path, bool auto_load = true) {

			return Add(DefaultGroupId, id, path, auto_load);

		}
		bool Add(AssetGroupId group, AssetId id, const std::string& path, bool auto_load = true) {

			auto lock = _writeLock();

			return _add(group, id, path, auto_load);

		}

		AssetHandle<asset_type> Get(AssetId id, bool auto_load = false) {

			return Get(DefaultGroupId, id, auto_load);

		}
		AssetHandle<asset_type> Get(AssetGroupId group, AssetId id, bool auto_load = false) {

			auto lock = _readLock();

			Asset* asset = _findAsset(group, id);

			if(asset == nullptr)
				if (auto_load) {
					_load(group, id);
					asset = _findAsset(group, id);
				}
				else
					return nullptr;

			return &(asset->Data.Data);

		}

		bool Remove(AssetId asset) {

			return Remove(DefaultGroupId, asset);

		}
		bool Remove(AssetGroupId group, AssetId asset) {

			auto lock = _writeLock();

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			group_iter->second.erase(asset_iter);

			return true;

		}
		bool RemoveAll() {

			auto lock = _writeLock();

			_groups.clear();

			return true;

		}
		bool RemoveAll(AssetGroupId group) {

			auto lock = _writeLock();

			auto iter = _groups.find(group);
			if (iter == _groups.end())
				return false;

			iter->second.clear();

			return true;

		}

		bool Load(AssetId asset) {

			return Load(DefaultGroupId, asset);

		}
		bool Load(AssetGroupId group, AssetId asset) {

			auto lock = _writeLock();

			return _load(group, asset);

		}
		void LoadAll() {

			auto lock = _writeLock();

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				_loadAll(iter->first);
			
		}
		void LoadAll(AssetGroupId group) {

			auto lock = _writeLock();

			_loadAll(group);

		}
		void LoadAll(AssetGroupId group, unsigned int number_of_threads, std::function<void(AssetLoaderEventArgs&)> callback) {

			_loadAll(group, number_of_threads, callback);

		}

		bool Unload(AssetId asset) {

			return Unload(DefaultGroupId, asset);

		}
		bool Unload(AssetGroupId group, AssetId asset) {

			auto lock = _writeLock();

			return _unload(group, asset);

		}
		void UnloadAll() {

			auto lock = _writeLock();

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				_unloadAll(iter->first);

		}
		void UnloadAll(AssetGroupId group) {

			auto lock = _writeLock();

			// #todo This process could easily be made more efficient

			_unloadAll(group);

		}

		bool Reload(AssetId asset) {

			return Reload(DefaultGroupId, asset);

		}
		bool Reload(AssetGroupId group, AssetId asset) {

			auto lock = _writeLock();

			return _reload(group, asset);

		}
		void ReloadAll() {

			auto lock = _writeLock();

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				_reloadAll(iter->first);

		}
		void ReloadAll(AssetGroupId group) {

			auto lock = _writeLock();

			_reloadAll(group);

		}
		void ReloadAll(AssetGroupId group, unsigned int number_of_threads, std::function<void(AssetLoaderEventArgs&)> callback) {

			UnloadAll(group);

			_loadAll(group, number_of_threads, callback);

		}

		bool Exists(AssetId asset) const {

			return Exists(DefaultGroupId, asset);

		}
		bool Exists(AssetGroupId group, AssetId asset) const {

			auto lock = _readLock();

			return _exists(group, asset);

		}
		bool GroupExists(AssetGroupId group) const {

			auto lock = _readLock();

			auto iter = _groups.find(group);

			return iter != _groups.end();

		}

		bool IsLoaded(AssetId asset) const {

			return IsLoaded(DefaultGroupId, asset);

		}
		bool IsLoaded(AssetGroupId group, AssetId asset) const {

			auto lock = _readLock();

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			return asset_iter->second.IsLoaded;

		}

		size_t Count(AssetGroupId group) const {

			auto lock = _readLock();

			auto iter = _groups.find(group);
			if (iter == _groups.end())
				return 0;

			return iter->second.size();

		}

		AssetHandle<asset_type> operator[](AssetId id) {

			return Get(id, true);

		}

	private:
		std::unique_ptr<IAssetLoader<asset_type> > _loader;
		group_map_type _groups;
		mutable std::shared_timed_mutex _groups_mutex;
		AssetId _next_id;

		/*
		The following functions are non-threadsafe implementations of the public member functions.
		They can be called freely and recursively without worrying about locking twice.
		*/

		// Adds a new asset to the manager (not threadsafe).
		bool _add(AssetGroupId group, AssetId id, const std::string& path, bool load = true) {

			if (_exists(group, id))
				return false;

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				group_iter = _groups.insert(std::make_pair(group, asset_map_type())).first;

			group_iter->second.emplace(id, Asset(path, load ? _loader->LoadData(path) : _loader->GetNull(), load));

			return true;

		}
		// Loads asset data for the given asset (not threadsafe).
		bool _load(AssetGroupId group, AssetId asset) {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			if (asset_iter->second.IsLoaded)
				return true;

			AssetLoaderResult<asset_type> data = _loader->LoadData(asset_iter->second.Path);
			if (!data.Success) {
				asset_iter->second.Data.Size = 0;
				asset_iter->second.IsLoaded = false;
				_loader->FreeData(data);
			}

			asset_iter->second.Data.Data = std::move(_loader->LoadData(asset_iter->second.Path).Data);
			asset_iter->second.IsLoaded = true;

			return true;

		}
		// Loads all assets in the given group that have not already been loaded (not threadsafe).
		void _loadAll(AssetGroupId group) {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return;

			// #todo This process could easily be made more efficient

			for (auto iter = group_iter->second.begin(); iter != group_iter->second.end(); ++iter)
				_load(group, iter->first);

		}
		// Unload asset data for the given asset (not threadsafe).
		bool _unload(AssetGroupId group, AssetId asset) {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			if (!asset_iter->second.IsLoaded)
				return true;

			_loader->FreeData(asset_iter->second.Data);
			asset_iter->second.IsLoaded = false;

			return true;

		}
		// Unloads all assets in the given group (not threadsafe).
		void _unloadAll(AssetGroupId group) {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return;

			for (auto iter = group_iter->second.begin(); iter != group_iter->second.end(); ++iter)
				_unload(group, iter->first);

		}
		// Unloads and reloads asset data for the given asset (not threadsafe).
		bool _reload(AssetGroupId group, AssetId asset) {

			if (!_unload(group, asset))
				return false;

			return _load(group, asset);

		}
		// Unloads and reloads all assets in the given group (not threadsafe).
		void _reloadAll(AssetGroupId group) {

			// #todo This process could easily be made more efficient

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return;

			for (auto iter = group_iter->second.begin(); iter != group_iter->second.end(); ++iter)
				_reload(group, iter->first);

		}
		// Returns true if the given asset exists in the given group (not threadsafe).
		bool _exists(AssetGroupId group, AssetId asset) const {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);

			return asset_iter != group_iter->second.end();

		}
		// Returns a pointer to the requested asset, or nullptr if it doesn't exist (not threadsafe). 
		Asset* _findAsset(AssetGroupId group, AssetId asset) {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return nullptr;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return nullptr;

			return &(asset_iter->second);

		}

		// Loads all assets in the given group using a threadpool (threadsafe).
		void _loadAll(AssetGroupId group, unsigned int number_of_threads, std::function<void(AssetLoaderEventArgs&)> callback) {

			// Enumerate all asset ids for the assets we'll be loading. We iterate the collection in one go to avoid issues with multithreading later.
			// This also means the assets can be modified or even deleted while they're being loaded.

			// Acquire a read lock so we can access the asset map.
			auto read_lock = _readLock();

			// Get an iterator to the group we'll be loading assets from. If the group doesn't exist, quit here.
			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return;

			std::mutex callback_mutex;
			std::atomic<unsigned int> loaded_assets = 0;
			std::atomic<size_t> total_assets = group_iter->second.size();
			Threading::ThreadPool threads(number_of_threads);

			for (auto i = group_iter->second.begin(); i != group_iter->second.end(); ++i) {
				threads.AddTask([&, i, this] {

					// We need to get the asset information for the asset the thread will be loading.
					AssetId asset_id = i->first;

					// Acquire a read lock so we can pull asset information from the map.
					auto read_lock = _readLock();

					// Create a callback lock without yet locking to ensure that only one thread calls the callback at any given time.
					std::unique_lock<std::mutex> callback_lock(callback_mutex, std::defer_lock);

					// Find the asset in the map. We find the group iterator each time because it may have been deleted while loading assets.
					Asset* asset = _findAsset(group, asset_id);
					if (asset == nullptr || asset->IsLoaded) {
						callback_lock.lock();
						if (asset == nullptr)
							callback(AssetLoaderEventArgs(loaded_assets, --total_assets, 0, ASSET_LOADER_EVENT_STATUS_REMOVED, asset_id, group));
						else
							callback(AssetLoaderEventArgs(++loaded_assets, total_assets, asset->Data.Size, ASSET_LOADER_EVENT_STATUS_ALREADY_LOADED, asset_id, group));
						callback_lock.unlock();
						return;
					}

					// The asset exists, so get the information we need in order to load it.
					std::string path = asset->Path;

					// Release the read lock for other threads; we don't need to hold onto it while loading the resource.
					read_lock.unlock();

					// Call the callback function to indicate that the asset has begun being loaded.
					callback_lock.lock();
					callback(AssetLoaderEventArgs(loaded_assets, total_assets, 0, ASSET_LOADER_EVENT_STATUS_STARTED, asset_id, group));
					callback_lock.unlock();

					// Load asset data.
					AssetLoaderResult<asset_type> data = _loader->LoadData(path);

					// Once the asset data has been loaded, apply it to the asset.		

					auto write_lock = _writeLock();

					// Get the asset from the map again in case it was deleted by another thread.
					asset = _findAsset(group, asset_id);

					// If we couldn't find the asset or the asset couldn't be loaded, report the error to the callback function.
					if (asset == nullptr || !data.Success) {
						if (asset != nullptr)
							asset->IsLoaded = false;
						_loader->FreeData(data);
						callback_lock.lock();
						callback(AssetLoaderEventArgs(loaded_assets, --total_assets, data.Size, asset == nullptr ? ASSET_LOADER_EVENT_STATUS_REMOVED : ASSET_LOADER_EVENT_STATUS_FAILED, asset_id, group));
						callback_lock.unlock();
						return;
					}

					// Apply the newly-loaded asset data to the asset.
					asset->Data = std::move(data);
					asset->IsLoaded = true;

					write_lock.unlock();

					// Call the callback function to indicate that the asset has finished being loaded.
					callback_lock.lock();
					callback(AssetLoaderEventArgs(++loaded_assets, total_assets, data.Size, ASSET_LOADER_EVENT_STATUS_FINISHED, asset_id, group));
					callback_lock.unlock();

				});
			}

			// Unlock the read lock so that the threads can begin loading assets.
			read_lock.unlock();

			// Join all threads to wait for the process to complete.
			threads.JoinAll();

		}
		// Generates a new, unique asset id for assets added to the default group.
		AssetId _getNextFreeId() const {

			auto lock = _readLock();

			AssetId id = _next_id;
			AssetId low = id;
			bool found = true;

			while (_exists(DefaultGroupId, id)) {
				if (id == std::numeric_limits<AssetId>::max()) {
					found = false;
					break;
				}
				++id;
			}

			if (found)
				return id;

			id = 0;
			found = true;

			while (_exists(DefaultGroupId, id)) {

				if (id == low) {
					found = false;
					break;
				}

				++id;

			}

			// #todo Throw an exception when this occurs?

			return found ? id : 0;

		}

		std::unique_lock<std::shared_timed_mutex> _writeLock() const {

			return std::unique_lock<std::shared_timed_mutex>(_groups_mutex);

		}
		std::shared_lock<std::shared_timed_mutex> _readLock() const {

			return std::shared_lock<std::shared_timed_mutex>(_groups_mutex);

		}

	};

}