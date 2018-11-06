#pragma once
#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/graphics/Bitmap.h"
#include "hvn3/objects/IObject.h"
#include <unordered_map>

namespace hvn3 {

	typedef unsigned long LightSourceId;
	typedef unsigned long LightMapId;

	enum class LightSourceType {
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

	class BasicLightingManager;

	class LightSourceHandle {

	public:
		LightSourceHandle();
		LightSourceHandle(BasicLightingManager* manager, LightSourceId id);

		void Release();

		LightSource* operator->();
		const LightSource* operator->() const;

		explicit operator bool() const;

	private:
		BasicLightingManager* _manager;
		LightSourceId _id;

	};

	class BasicLightingManager :
		public IManager,
		public IDrawable {

		struct LightMapData {
			Graphics::Bitmap bitmap;
			PointF origin;
		};

	public:
		BasicLightingManager();
		~BasicLightingManager();

		LightSourceHandle CreateLight(const PointF& position, LightSourceType type);
		LightSource* GetLight(LightSourceId id);
		bool LightExists(LightSourceId id) const;
		bool RemoveLight(LightSourceId id);
		void RemoveLight(LightSourceHandle& handle);

		void Clear();

		const Graphics::Bitmap& Surface() const;
		float AmbientLightLevel() const;
		void SetAmbientLightLevel(float value);
		bool Enabled() const;
		void SetEnabled(bool value);

		static void AddLightMap(const Graphics::Bitmap& light_map, LightSourceType type, const PointF& origin, bool set_alpha = true);

		void OnDraw(DrawEventArgs& e) override;
		void OnContextChanged(ContextChangedEventArgs& e) override;

	private:
		LightSourceId _getNextLightSourceId();
		LightMapData* _findLightMapData(LightSourceType type);

		Context _context;
		std::unordered_map<LightSourceId, LightSource> _light_sources;
		LightSourceId _next_id;
		bool _enabled;
		float _ambient_light_level;
		Graphics::Bitmap _surface;

		// Default light maps are shared across all instances (there is no reason to duplicate them).
		// When the last instance of the class is deinitialized, the light maps are cleared.
		static std::unordered_map<LightSourceType, LightMapData> _light_maps;
		static int _instance_count;

		static void _loadDefaultLightMaps();
		static void _freeDefaultLightMaps();

	};

	template<>
	struct ManagerIdTraits<BASIC_LIGHTING_MANAGER> {
		typedef BasicLightingManager type;
	};

}