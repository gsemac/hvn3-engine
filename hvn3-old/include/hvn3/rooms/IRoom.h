#pragma once
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/ManagerDefs.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/rooms/RoomDefs.h"
#include "hvn3/rooms/RoomEventArgs.h"

namespace hvn3 {

	class Background;
	class View;

	class IObjectManager;
	class IBackgroundManager;
	class IViewManager;
	class IObject;
	class TileManager;

	namespace System {
		class RoomProxy;
	}
	namespace Physics {
		class IPhysicsManager;
	}

	class IRoom :
		public IUpdatable, public IDrawable,
		public virtual ISizeable<int> {

		friend class System::RoomProxy;

	public:
		virtual ~IRoom() = default;

		// Returns the object manager for this room.
		virtual IObjectManager& Objects() = 0;
		// Returns the backhground manager for this room.
		virtual IBackgroundManager& Backgrounds() = 0;
		// Returns the view manager for this room.
		virtual IViewManager& Views() = 0;
		// Returns the collision manager for this room.
		virtual ICollisionManager& Collisions() = 0;
		// Returns the physics manager for this room.
		virtual Physics::IPhysicsManager& Physics() = 0;
		// Returns the tile manager for this room.
		virtual TileManager& Tiles() = 0;

		virtual const IObjectManager& Objects() const = 0;
		virtual const IBackgroundManager& Backgrounds() const = 0;
		virtual const IViewManager& Views() const = 0;
		virtual const ICollisionManager& Collisions() const = 0;
		virtual const Physics::IPhysicsManager& Physics() const = 0;
		virtual const TileManager& Tiles() const = 0;

		// Sets the room's background color.
		virtual void SetBackgroundColor(const Color& color) = 0;
		// Returns the room's background color.
		virtual const Color& BackgroundColor() const = 0;
		// Sets the room's background image. If any other backgrounds have been added, they will be cleared.
		virtual void SetBackground(const Background& value) = 0;

		// Returns the view currently being rendered.
		virtual const View& CurrentView() const = 0;
		// Returns the room's identifier.
		virtual RoomId Id() const = 0;
		virtual bool Persistent() const = 0;
		virtual void SetPersistent(bool value) = 0;
		virtual RectangleF VisiblePort() const = 0;
		virtual RectangleF VisibleRegion() const = 0;
		virtual RectangleF Bounds() const = 0;

		virtual void Restart() = 0;
		virtual void OnEnter(RoomEnterEventArgs& e) = 0;
		virtual void OnExit(RoomExitEventArgs& e) = 0;
		virtual void OnCreate(RoomCreateEventArgs& e) = 0;
		virtual bool IsReady() const = 0;
		virtual void OnReset() = 0;
		virtual void OnRender(DrawEventArgs& e) = 0;

	};

}