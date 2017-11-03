#pragma once
#include "AssetHandle.h"
#include "IAssetLoader.h"
#include "AssetLoadEventArgs.h"
#include "utility/ThreadPool.h"
#include <functional>
#include <limits>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace hvn3 {

	template <typename asset_type>
	class AssetManager {

	private:
		struct Asset {

			Asset(std::string Path, asset_type&& Data, bool IsLoaded) :
				Path(Path),
				Data(std::move(Data)),
				IsLoaded(IsLoaded) {
			}

			std::string Path;
			asset_type Data;
			bool IsLoaded;

		};

		typedef std::unordered_map<AssetId, Asset> asset_map_type;
		typedef std::unordered_map<AssetGroupId, asset_map_type> group_map_type;

	public:
		typedef asset_type asset_type;
		static const AssetGroupId default_group_id = 0;

		AssetManager(std::unique_ptr<IAssetLoader<asset_type> >& loader) :
			_loader(std::move(loader)) {

			_next_id = 0;

		}
		~AssetManager() {

			FreeAll();

		}

		AssetId Add(const std::string& path, bool load = true) {

			AssetId id = _getNextFreeId();

			Add(id, path, load);

			return id;

		}
		bool Add(AssetId id, const std::string& path, bool load = true) {

			return Add(default_group_id, id, path, load);

		}
		bool Add(AssetGroupId group, AssetId id, const std::string& path, bool load = true) {

			auto guard = _writeLock();

			return _add(group, id, path, load);

		}

		bool Load(AssetId asset) {

			return Load(default_group_id, asset);

		}
		bool Load(AssetGroupId group, AssetId asset) {

			auto guard = _writeLock();

			return _load(group, asset);

		}
		void LoadAll();
		void LoadAll(AssetGroupId group);
		void LoadAllAsync(AssetGroupId group);

		bool Unload(AssetId asset) {

			return Unload(default_group_id, asset);

		}
		bool Unload(AssetGroupId group, AssetId asset) {

			auto guard = _writeLock();

			return _unload(group, asset);

		}
		void UnloadAll() {

			auto guard = _writeLock();

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				_unloadAll(iter->first);

		}
		void UnloadAll(AssetGroupId group) {

			auto guard = _writeLock();

			// #todo This process could easily be made more efficient

			_unloadAll(group);

		}

		bool Reload(AssetId asset) {

			return Reload(default_group_id, asset);

		}
		bool Reload(AssetGroupId group, AssetId asset) {

			auto guard = _writeLock();

			return _reload(group, asset);

		}
		void ReloadAll() {

			auto guard = _writeLock();

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				_reloadAll(iter->first);

		}
		void ReloadAll(AssetGroupId group) {

			auto guard = _writeLock();

			_reloadAll(group);

		}
		void ReloadAll(AssetGroupId group, unsigned int number_of_threads, std::function<void(AssetLoadEventArgs&)> callback) {

			_reload_all_mt(group, number_of_threads, callback);

		}

		bool Free(AssetId asset) {

			return Free(default_group_id, asset);

		}
		bool Free(AssetGroupId group, AssetId asset) {

			auto guard = _writeLock();

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			group_iter->second.erase(asset_iter);

			return true;

		}
		bool FreeAll() {

			auto guard = _writeLock();

			_groups.clear();

			return true;

		}
		bool FreeAll(AssetGroupId group) {

			auto guard = _writeLock();

			auto iter = _groups.find(group);
			if (iter == _groups.end())
				return false;

			iter->second.clear();

			return true;

		}

		bool Exists(AssetId asset) const {

			return Exists(default_group_id, asset);

		}
		bool Exists(AssetGroupId group, AssetId asset) const {

			auto guard = _readLock();

			return _exists(group, asset);

		}
		bool GroupExists(AssetGroupId group) const {

			auto guard = _readLock();

			auto iter = _groups.find(group);

			return iter != _groups.end();

		}

		bool IsLoaded(AssetId asset) const {

			return IsLoaded(default_group_id, asset);

		}
		bool IsLoaded(AssetGroupId group, AssetId asset) const {

			auto guard = _readLock();

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			return asset_iter->second.IsLoaded;

		}

		size_t Count(AssetGroupId group) const {

			auto guard = _readLock();

			auto iter = _groups.find(group);
			if (iter == _groups.end())
				return 0;

			return iter->second.size();

		}

	private:
		std::unique_ptr<IAssetLoader<asset_type> > _loader;
		group_map_type _groups;
		mutable std::shared_timed_mutex _groups_mutex;
		AssetId _next_id;

		/*
		The following functions are non-threadsafe implementations of the public member functions.
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

			asset_iter->second.Data = std::move(_loader->LoadData(asset_iter->second.Path));
			asset_iter->second.IsLoaded = true;

			return true;

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

		void _reload_all_mt(AssetGroupId group, unsigned int number_of_threads, std::function<void(AssetLoadEventArgs&)> callback) {

			UnloadAll(group);

			// Enumerate all asset ids for the assets we'll be loading. We iterate the collection in one go to avoid issues with multithreading later.
			// This also means the assets can be modified or even deleted while they're being loaded.
			std::vector<AssetId> asset_ids;
			for (int i = 0; i < 8; ++i)
				asset_ids.push_back(i);

			std::mutex callback_mutex;
			std::atomic<unsigned int> loaded_assets = 0;
			Threading::ThreadPool threads(number_of_threads);

			for (size_t i = 0; i < asset_ids.size(); ++i) {
				threads.AddTask([&, this] {

					// We need to get the asset information for the asset the thread will be loading.
					auto read_lock = _readLock();

					read_lock.unlock();

					// By ensuring that only one thread can enter the callback function at a time, the events are generated in the proper order.
					std::lock_guard<std::mutex> callback_lock(callback_mutex);
					callback(AssetLoadEventArgs(++loaded_assets, asset_ids.size()));

				});
			}

			threads.JoinAll();

		}

		AssetId _getNextFreeId() const {

			auto guard = _readLock();

			AssetId id = _next_id;
			AssetId low = id;
			bool found = true;

			while (_exists(default_group_id, id)) {
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

			while (_exists(default_group_id, id)) {

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