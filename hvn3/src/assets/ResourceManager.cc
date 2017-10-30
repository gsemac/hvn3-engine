#include "assets/ResourceManager.h"

namespace hvn3 {

	ResourceManager::ResourceManager() {}

	ResourceCollection<Sprite>& ResourceManager::Sprites() {

		return _sprites;

	}
	ResourceCollection<Font>& ResourceManager::Fonts() {

		return _fonts;

	}
	ResourceCollection<Sound>& ResourceManager::Sounds() {

		return _sounds;

	}
	ResourceCollection<Background>& ResourceManager::Backgrounds() {

		return _backgrounds;

	}
	ResourceCollection<Tileset>& ResourceManager::Tilesets() {

		return _tilesets;

	}

	ResourceHandle<Background> ResourceManager::Backgrounds(ResourceId id) {

		return Backgrounds()[id];

	}
	ResourceHandle<Font> ResourceManager::Fonts(ResourceId id) {

		return Fonts()[id];

	}
	ResourceHandle<Sound> ResourceManager::Sounds(ResourceId id) {

		return Sounds()[id];

	}
	ResourceHandle<Sprite> ResourceManager::Sprites(ResourceId id) {

		return Sprites()[id];

	}
	ResourceHandle<Tileset> ResourceManager::Tilesets(ResourceId id) {

		return Tilesets()[id];

	}

	void ResourceManager::Clear() {

		_sprites.Clear();
		_fonts.Clear();
		_sounds.Clear();
		_backgrounds.Clear();
		_tilesets.Clear();

	}

}