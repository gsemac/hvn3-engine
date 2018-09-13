#include "hvn3/assets/SystemAssets.h"
#include "hvn3/core/DrawEventArgs.h"
#include "hvn3/io/Path.h"
#include "hvn3/graphics/BitmapUtils.h"
#include "hvn3/graphics/Graphics.h"
#include "hvn3/math/MathUtils.h"
#include "hvn3/rooms/BasicLightingManager.h"
#include "hvn3/rooms/Room.h"

namespace hvn3 {

	LightSource::LightSource() {
		direction = 0.0f;
		brightness = 1.0f;
		intensity = 1.0f;
		on = true;
		type = LightSourceType::Radial;
	}



	BasicLightingManager::BasicLightingManager(System::IContextProvider& context_provider) {

		_context_provider = &context_provider;
		SetAmbientLightLevel(0.0f);
		_enabled = true;

		if (_instance_count++ == 0)
			_loadDefaultLightMaps();

	}
	BasicLightingManager::~BasicLightingManager() {

		if (--_instance_count == 0)
			_freeDefaultLightMaps();

	}
	LightSourceId BasicLightingManager::Create(const PointF& position, LightSourceType type) {

		LightSourceId lsid = _getNextLightSourceId();
		LightSource ls;

		ls.position = position;
		ls.type = type;

		_light_sources[lsid] = ls;

		return lsid;

	}
	LightSource* BasicLightingManager::GetLightSourceById(LightSourceId id) {

		auto iter = _light_sources.find(id);

		if (iter == _light_sources.end())
			return nullptr;

		return &iter->second;

	}
	void BasicLightingManager::Clear() {
		_light_sources.clear();
		_next_id = 0;
	}
	const Graphics::Bitmap& BasicLightingManager::Surface() const {
		return _surface;
	}
	float BasicLightingManager::AmbientLightLevel() const {
		return 1.0f - _ambient_light_level;
	}
	void BasicLightingManager::SetAmbientLightLevel(float value) {
		_ambient_light_level = 1.0f - Math::Clamp(value, 0.0f, 1.0f);
	}
	bool BasicLightingManager::Enabled() const {
		return _enabled;
	}
	void BasicLightingManager::SetEnabled(bool value) {
		_enabled = value;
	}
	void BasicLightingManager::AddLightMap(const Graphics::Bitmap& light_map, LightSourceType type, const PointF& origin, bool set_alpha) {

		LightMapData map;
		map.bitmap = light_map;
		map.origin = origin;

		_light_maps[type] = map;

		if (set_alpha)
			Graphics::SetAlphaFromBitmap(_light_maps[type].bitmap, _light_maps[type].bitmap);

	}
	void BasicLightingManager::OnDraw(DrawEventArgs& e) {

		RectangleF visible_region = _context_provider->Context().Room().VisibleRegion();
		SizeI visible_size = static_cast<SizeI>(visible_region.Size());

		// If the visible region has changed in size, resize the lighting surface.
		if (!_surface || _surface.Width() != visible_size.Width() || _surface.Height() != visible_size.Height())
			_surface = Graphics::Bitmap(visible_size);

		PointF origin;
		float angle = 0.0f;

		// If there is an active view, we need to rotate the lights and lighting surface accordingly.
		if (_context_provider->Context().Room().Views().Count() > 0) {
			const View& view = _context_provider->Context().Room().CurrentView();
			origin = view.Region().Midpoint() - view.Position();
			angle = view.Angle();
		}

		// Draw each light on the lighting surface.

		Graphics::Graphics g(_surface);
		Graphics::Transform transform;

		transform.Translate(-visible_region.X(), -visible_region.Y());
		transform.Rotate(origin, angle);

		g.Clear(Color::FromArgbf(0.0f, 0.0f, 0.0f, _ambient_light_level));
		g.SetBlendMode(Graphics::BlendOperation::Subtract);
		g.SetTransform(transform);

		for (auto i = _light_sources.begin(); i != _light_sources.end(); ++i) {

			LightMapData* lm = _findLightMapData(i->second.type);

			if (lm == nullptr)
				continue;

			g.DrawBitmap(i->second.position.x, i->second.position.y, lm->bitmap, 1.0f, 1.0f, lm->origin, i->second.direction);

		}

		g.ResetBlendMode();

		// Draw the lighting surface.

		PointF draw_pos = visible_region.Position() + origin;
		e.Graphics().DrawBitmap(draw_pos.x, draw_pos.y, _surface, 1.0f, 1.0f, origin, -angle);

	}


	LightSourceId BasicLightingManager::_getNextLightSourceId() {
		return _next_id++;
	}
	BasicLightingManager::LightMapData* BasicLightingManager::_findLightMapData(LightSourceType type) {

		auto it = _light_maps.find(type);

		if (it == _light_maps.end())
			return nullptr;

		return &it->second;

	}

	std::unordered_map<LightSourceType, BasicLightingManager::LightMapData> BasicLightingManager::_light_maps;
	int BasicLightingManager::_instance_count = 0;

	void BasicLightingManager::_loadDefaultLightMaps() {

		// Store the existing bitmap flags, and enable the AntiAlias flag (so the light maps can be stretched without pixellation).
		Graphics::BitmapFlags old_flags = Graphics::Bitmap::DefaultBitmapFlags();
		Graphics::Bitmap::SetDefaultBitmapFlags(Graphics::BitmapFlags::Default | Graphics::BitmapFlags::AntiAlias);
		
		// Attempt to load the system light maps. Do not throw an exception if this fails, because the user can still add their own light maps if they choose not to use the system ones.
		// #todo Store these assets in code, not on the file system.
		Graphics::Bitmap lm_radial = Graphics::Bitmap::FromFile(System::GetSystemAssetPath(System::SystemAssetType::Graphics) + "lm_radial.png");
		Graphics::Bitmap lm_spotlight = Graphics::Bitmap::FromFile(System::GetSystemAssetPath(System::SystemAssetType::Graphics) + "lm_spotlight.png");

		if (lm_radial)
			AddLightMap(lm_radial, LightSourceType::Radial, hvn3::PointF(lm_radial.Width() / 2.0f, lm_radial.Height() / 2.0f), true);
		if (lm_spotlight)
			AddLightMap(lm_spotlight, LightSourceType::Spotlight, hvn3::PointF(lm_spotlight.Width() / 2.0f, 3.0f), true);

		// Restore the previous bitmap flags.
		Graphics::Bitmap::SetDefaultBitmapFlags(old_flags);

	}
	void BasicLightingManager::_freeDefaultLightMaps() {
		_light_maps.clear();
	}

}