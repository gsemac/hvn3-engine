#pragma once
#include <unordered_map>
#include <utility>

template <typename key_type, typename resource_type>
class ResourceCollection {

public:
	~ResourceCollection() {

		// Free all resources.
		Clear();

	}

	// Adds a resource pointer to the collection with the given key.
	int Add(const key_type& key, resource_type* resource) {

		__map.insert(std::pair<key_type, resource_type*>(key, resource));

		return key;

	}
	// Returns a pointer to the resource with the given key.
	resource_type* Find(const key_type& key) {

		// Attempt to find the key in the map.
		auto it = __map.find(key);

		// If it wasn't found, return nullptr.
		if (it == __map.end())
			return nullptr;
		
		// Otherwise, return the value.
		return it->second;
	
	}
	// Removes the resource with the given key from the collection.
	void Remove(const key_type& key) {

		// Attempt to find the key in the map.
		auto it = __map.find(key);

		// If it wasn't found, do nothing.
		if (it == __map.end())
			return;

		// Free the resource.
		delete it->second;

		// Remove the resource from the map.
		__map.erase(it);

	}
	// Clears and frees all resources from the collection.
	void Clear() {

		for (auto it = __map.cbegin(); it != __map.cend();)
			it = __map.erase(it);

	}

	resource_type* operator[] (const key_type& key) {

		return Find(key);

	}

private:
	std::unordered_map<key_type, resource_type*> __map;

};