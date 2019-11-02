//#pragma once
//#include "hvn3/rooms/RoomBase.h"
//#include "hvn3/collision/CollisionManager.h"
//#include "hvn3/collision/SpatialPartitioningGrid.h"
//#include "hvn3/collision/NarrowPhase.h"
//#include "hvn3/backgrounds/BackgroundManager.h"
//#include "hvn3/views/ViewManager.h"
//#include "hvn3/physics/BasicPhysicsManager.h"
//#include "hvn3/objects/ObjectManager.h"
//#include "hvn3/tiles/TileManager.h"
//#include <memory>
//#include <unordered_map>
//
//#pragma warning(push)
//#pragma warning(disable:4250)
//
//namespace hvn3 {
//
//	class Room :
//		public RoomBase {
//
//	public:
//		typedef BackgroundManager background_manager_type;
//		typedef ObjectManager object_manager_type;
//		typedef hvn3::Physics::BasicPhysicsManager physics_manager_type;
//		typedef TileManager tile_manager_type;
//		typedef ViewManager view_manager_type;
//
//		Room(RoomId id, const SizeI& size);
//		Room(RoomId id, int width, int height);
//		Room(const SizeI& size);
//		Room(int width, int height);
//
//		virtual ~Room() = default;
//
//		virtual void OnUpdate(UpdateEventArgs& e) override;
//		virtual void OnDraw(DrawEventArgs& e) override;
//
//		void SetBackground(const Background& value) override;
//
//		const View& CurrentView() const override;
//		RectangleF VisiblePort() const override;
//		RectangleF VisibleRegion() const override;
//
//		void Restart() override;
//		void OnReset() override;
//		void OnRender(DrawEventArgs& e) override;
//
//	private:
//		void _initializeAllManagers();
//
//		size_t _rendering_view;
//		bool _restart_pending;
//
//	};
//
//}