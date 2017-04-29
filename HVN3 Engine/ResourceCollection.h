#pragma once
#include <unordered_map>
#include <utility>
#include <memory>

typedef uintptr_t ResourceId;

class Resource {

public:
	template<typename T, typename ... Args>
	static std::unique_ptr<T> Create(Args &&... args) {

		return std::make_unique<T>(std::forward<Args>(args)...);

	}

private:

};

template <typename resource_type>
class ResourceCollection {

public:
	ResourceCollection() {}
	~ResourceCollection() {

		Clear();

	}

	ResourceId Add(std::unique_ptr<resource_type>& resource) {

		// Use the memory address of the resource as the key, since this is guaranteed to be 
		// unique so long as the same resource isn't added multiple times.
		uintptr_t id = reinterpret_cast<uintptr_t>(resource.get());

		// Add the resource to the collection.
		return ResourceAdd(id, resource);

	}
	ResourceId Add(ResourceId id, std::unique_ptr<resource_type>& resource) {

		_map.insert(std::make_pair(id, std::move(resource)));

		return id;

	}
	bool Remove(ResourceId id) {

		// Attempt to find the key in the map.
		auto it = __map.find(id);

		// If it wasn't found, do nothing.
		if (it == __map.end())
			return;

		// Free the resource.
		delete it->second;

		// Remove the resource from the map.
		__map.erase(it);

	}
	resource_type* Find(ResourceId id) {

		// Attempt to find the key in the map.
		auto it = _map.find(id);

		// If it wasn't found, return nullptr.
		if (it == _map.end())
			return nullptr;

		// Otherwise, return the value.
		return it->second.get();

	}
	bool Exists(ResourceId id) {

		return ResourceFind(id) == nullptr;

	}
	
	void Clear() {

		_map.clear();

	}

	resource_type* operator[] (ResourceId id) {

		return Find(id);

	}

private:
	std::unordered_map<ResourceId, std::unique_ptr<resource_type>> _map;

};