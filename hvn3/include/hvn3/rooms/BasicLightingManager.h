#pragma once
#include "hvn3/core/IContextProvider.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/objects/IObject.h"
#include <unordered_map>

namespace hvn3 {

	typedef unsigned long LightSourceId;
	typedef unsigned long LightMapId;

	enum LightSourceType {
		Radial,
		Spotlight
	};

	struct LightSource {
		LightSource();
		PointF position;
		float direction;
		float brightness;
		float intensity;
		Color color;
		bool on;
		LightSourceType type;
	};

	class BasicLightingManager : public System::ManagerBase, public IDrawable {

	public:
		BasicLightingManager(System::IContextProvider& context_provider);

		LightSourceId Create(const PointF& position, LightSourceType type);
		LightSource* GetLightSourceById(LightSourceId id);
		void Clear();

		const Graphics::Bitmap& Surface() const;
		float AmbientLightLevel() const;
		void SetAmbientLightLevel(float value);
		bool Enabled() const;
		void SetEnabled(bool value);

		void AddLightMap(LightSourceType type, const Graphics::Bitmap& light_map, bool set_alpha = true);

		void OnDraw(DrawEventArgs& e) override;

		static System::ManagerId Id();

	private:
		LightSourceId _getNextLightSourceId();
		PointF _getSurfacePosition();
		SizeI _getSurfaceSize();

		System::IContextProvider* _context_provider;
		std::unordered_map<LightSourceId, LightSource> _light_sources;
		LightSourceId _next_id;
		bool _enabled;
		float _ambient_light_level;
		Graphics::Bitmap _surface;

		// Default light maps are shared across all instances (there is no reason to duplicate them).
		// When the last instance of the class is deinitialized, the light maps are cleared.
		static std::unordered_map<LightSourceType, Graphics::Bitmap> _light_maps;

	};

}