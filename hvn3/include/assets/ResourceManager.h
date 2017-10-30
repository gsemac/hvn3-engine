#pragma once
#include "assets/ResourceCollection.h"
#include "backgrounds/Background.h"
#include "fonts/Font.h"
#include "sound/Sound.h"
#include "sprites/Sprite.h"
#include "tilesets/Tileset.h"

namespace hvn3 {

	class ResourceManager {

	public:
		ResourceManager();

		ResourceCollection<Background>& Backgrounds();
		ResourceCollection<Font>& Fonts();
		ResourceCollection<Sound>& Sounds();
		ResourceCollection<Sprite>& Sprites();
		ResourceCollection<Tileset>& Tilesets();

		ResourceHandle<Background> Backgrounds(ResourceId id);
		ResourceHandle<Font> Fonts(ResourceId id);
		ResourceHandle<Sound> Sounds(ResourceId id);
		ResourceHandle<Sprite> Sprites(ResourceId id);
		ResourceHandle<Tileset> Tilesets(ResourceId id);

		void Clear();

	private:
		ResourceCollection<hvn3::Background> _backgrounds;
		ResourceCollection<hvn3::Font> _fonts;
		ResourceCollection<hvn3::Sound> _sounds;
		ResourceCollection<hvn3::Sprite> _sprites;
		ResourceCollection<hvn3::Tileset> _tilesets;

	};

}