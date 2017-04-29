#include "ResourceManager.h"

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
void ResourceManager::Clear() {

	_sprites.Clear();
	_fonts.Clear();
	_sounds.Clear();
	_backgrounds.Clear();
	_tilesets.Clear();

}