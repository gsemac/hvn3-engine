#pragma once
#include "AssetHandle.h"
#include "FileAssetLoader.h"
#include <unordered_map>
#include <string>
#include <limits>
#include <memory>

namespace hvn3 {

	template <typename asset_type>
	class AssetManager {
		
	private:
		struct Asset {

			Asset(std::string Path, asset_type& Data, bool IsLoaded) :
				Path(Path),
				Data(std::move(Data)),
				IsLoaded(IsLoaded) {
			}

			std::string Path;
			asset_type Data;
			bool IsLoaded;

		};

		typedef std::unordered_map<AssetId, Asset> map_value_type;
		typedef std::unordered_map<AssetGroupId, map_value_type> map_type;

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

			if (Exists(group, id))
				return false;
			
			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				group_iter = _groups.insert(std::make_pair(group, map_value_type())).first;				
			
			group_iter->second.emplace(id, Asset(path, load ? _loader->LoadData(path) : _loader->LoadNull(), load));

			return true;

		}

		bool Load(AssetId asset) {

			return Load(default_group_id, asset);

		}
		bool Load(AssetGroupId group, AssetId asset) {

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
		
		bool Unload(AssetId asset) {

			return Unload(default_group_id, asset);

		}
		bool Unload(AssetGroupId group, AssetId asset) {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			if (!asset_iter->second.IsLoaded)
				return true;

			_loader->UnloadData(asset_iter->second.Data);
			asset_iter->second.IsLoaded = false;

			return true;

		}
		void UnloadAll() {

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				UnloadAll(iter->first);

		}
		void UnloadAll(AssetGroupId group) {

			// #todo This process could easily be made more efficient

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return;

			for (auto iter = group_iter->second.begin(); iter != group_iter->second.end(); ++iter)
				Unload(group, iter->first);

		}

		bool Reload(AssetId asset) {

			return Reload(default_group_id, asset);

		}
		bool Reload(AssetGroupId group, AssetId asset) {

			if (!Unload(group, asset))
				return false;

			return Load(group, asset);

		}
		void ReloadAll() {

			// #todo This process could easily be made more efficient

			for (auto iter = _groups.begin(); iter != _groups.end(); ++iter)
				ReloadAll(iter->first);

		}
		void ReloadAll(AssetGroupId group) {
			
			// #todo This process could easily be made more efficient

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return;

			for (auto iter = group_iter->second.begin(); iter != group_iter->second.end(); ++iter)
				Reload(group, iter->first);

		}

		bool Free(AssetId asset) {

			return Free(default_group_id, asset);

		}
		bool Free(AssetGroupId group, AssetId asset) {

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

			_groups.clear();

		}
		bool FreeAll(AssetGroupId group) {

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

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			
			return asset_iter != group_iter->second.end();

		}
		bool GroupExists(AssetGroupId group) const {

			auto iter = _groups.find(group);

			return iter != _groups.end();

		}

		bool IsLoaded(AssetId asset) const {

			return IsLoaded(default_group_id, asset);

		}
		bool IsLoaded(AssetGroupId group, AssetId asset) const {

			auto group_iter = _groups.find(group);
			if (group_iter == _groups.end())
				return false;

			auto asset_iter = group_iter->second.find(asset);
			if (asset_iter == group_iter->second.end())
				return false;

			return asset_iter->second.IsLoaded;

		}

	private:
		std::unique_ptr<IAssetLoader<asset_type> > _loader;
		std::unordered_map<AssetGroupId, std::unordered_map<AssetId, Asset> > _groups;
		AssetId _next_id;

		AssetId _getNextFreeId() const {

			AssetId id = _next_id;
			AssetId low = id;
			bool found = true;

			while (Exists(id)) {
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

			while (Exists(id)) {

				if (id == low) {
					found = false;
					break;
				}

				++id;

			}

			// #todo Throw an exception when this occurs?

			return found ? id : 0;

		}

	};

}