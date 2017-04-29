#pragma once
#include "ResourceCollection.h"
#include "Background.h"
#include "Font.h"
#include "Sound.h"
#include "Sprite.h"
#include "Tileset.h"

class ResourceManager {

public:
	ResourceManager();
	ResourceCollection<Background>& Backgrounds();
	ResourceCollection<Font>& Fonts();
	ResourceCollection<Sound>& Sounds();
	ResourceCollection<Sprite>& Sprites();
	ResourceCollection<Tileset>& Tilesets();

private:
	ResourceCollection<Background> _backgrounds;
	ResourceCollection<Font> _fonts;
	ResourceCollection<Sound> _sounds;
	ResourceCollection<Sprite> _sprites;
	ResourceCollection<Tileset> _tilesets;

};