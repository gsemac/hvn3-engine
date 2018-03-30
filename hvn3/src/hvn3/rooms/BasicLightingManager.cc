#include "hvn3/core/DrawEventArgs.h"
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
	void BasicLightingManager::AddLightMap(LightSourceType type, const Graphics::Bitmap& light_map, bool set_alpha) {

		_light_maps[type] = light_map;

		if (set_alpha)
			Graphics::SetAlphaFromBitmap(_light_maps[type], _light_maps[type]);

	}
	void BasicLightingManager::OnDraw(DrawEventArgs& e) {

		SizeI surface_size = _getSurfaceSize();

		if (!_surface || _surface.Width() != surface_size.Width() || _surface.Height() != surface_size.Height())
			_surface = Graphics::Bitmap(surface_size);

		Graphics::Graphics g(_surface);

		Graphics::Transform transform;
		Graphics::Transform view_transform;
		transform.Translate(-view_transform.GetOffset().x, -view_transform.GetOffset().y);
		transform.Rotate(view_transform.GetAngle());

		g.Clear(Color::FromArgbf(0.0f, 0.0f, 0.0f, _ambient_light_level));
		g.SetBlendMode(Graphics::BlendOperation::Subtract);
		g.SetTransform(transform);

		for (auto i = _light_sources.begin(); i != _light_sources.end(); ++i)
			g.DrawBitmap(i->second.position.x, i->second.position.y, _light_maps[i->second.type]);

		g.ResetBlendMode();

		PointF draw_pos = _getSurfacePosition();
		e.Graphics().DrawBitmap(draw_pos.x, draw_pos.y, _surface);

	}
	System::ManagerId BasicLightingManager::Id() {
		return System::BASIC_LIGHTING_MANAGER;
	}

	std::unordered_map<LightSourceType, Graphics::Bitmap> BasicLightingManager::_light_maps;


	LightSourceId BasicLightingManager::_getNextLightSourceId() {
		return _next_id++;
	}
	PointF BasicLightingManager::_getSurfacePosition() {

		if (_context_provider->Context().GetRoom().Views().ViewCount() > 0)
			return _context_provider->Context().GetRoom().CurrentView().Position();

		return PointF(0.0f, 0.0f);

	}
	SizeI BasicLightingManager::_getSurfaceSize() {

		SizeI surface_size = _context_provider->Context().GetRoom().Size();

		if (_context_provider->Context().GetRoom().Views().ViewCount() > 0) {
			const View& current_view = _context_provider->Context().GetRoom().CurrentView();
			surface_size = SizeI(static_cast<int>(current_view.Region().Width()), static_cast<int>(current_view.Region().Height()));
		}

		return surface_size;

	}

}