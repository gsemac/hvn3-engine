#pragma once
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/IContextProvider.h"
#include "hvn3/core/IContextReceiver.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/rooms/RoomEnterEventArgs.h"
#include "hvn3/rooms/RoomExitEventArgs.h"
#include "hvn3/rooms/RoomTypeDefs.h"

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
		public System::IContextReceiver,
		public virtual ISizeable<int> {

		friend class System::RoomProxy;

	public:
		virtual ~IRoom() = default;

		// Returns the object manager for this room.
		virtual IObjectManager& GetObjects() = 0;
		// Returns the backhground manager for this room.
		virtual IBackgroundManager& GetBackgrounds() = 0;
		// Returns the view manager for this room.
		virtual IViewManager& GetViews() = 0;
		// Returns the collision manager for this room.
		virtual ICollisionManager<IObject>& GetCollisions() = 0;
		// Returns the physics manager for this room.
		virtual Physics::IPhysicsManager& GetPhysics() = 0;
		// Returns the tile manager for this room.
		virtual TileManager& GetTiles() = 0;

		// Returns the object manager for this room.
		virtual const IObjectManager& Objects() const = 0;
		// Returns the backhground manager for this room.
		virtual const IBackgroundManager& Backgrounds() const = 0;
		// Returns the view manager for this room.
		virtual const IViewManager& Views() const = 0;
		// Returns the collision manager for this room.
		virtual const ICollisionManager<IObject>& Collisions() const = 0;
		// Returns the physics manager for this room.
		virtual const Physics::IPhysicsManager& Physics() const = 0;
		// Returns the tile manager for this room.
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

		// Registers a manager so it can be accessed through the current context.
		template <typename manager_type>
		void RegisterManager(std::shared_ptr<manager_type>& manager) {
			
			std::shared_ptr<ManagerBase> ptr = manager;
			
			_addManager(manager_type::Id(), ptr);

		}
		// Returns a reference to the manager with the given ID, if it exists. Otherwise, returns nullptr.
		template <typename manager_type>
		manager_type* GetManager() const {
			return static_cast<manager_type*>(_getManager(manager_type::Id()));
		}
		template <typename manager_type>
		manager_type* GetManagerById(ManagerId id) const {
			return static_cast<manager_type*>(_getManager(id));
		}

	protected:
		// Returns the context assigned to the room.
		virtual hvn3::Context Context() = 0;

		virtual void Restart() = 0;
		virtual void OnEnter(RoomEnterEventArgs& e) = 0;
		virtual void OnExit(RoomExitEventArgs& e) = 0;
		virtual void OnCreate() = 0;
		virtual bool IsReady() const = 0;
		virtual void OnReset() = 0;
		virtual void OnRender(DrawEventArgs& e) = 0;

	private:
		// Adds a new manager to the room, mapped to the given ID.
		virtual void _addManager(ManagerId id, std::shared_ptr<ManagerBase>& manager) = 0;
		// Returns a reference to the manager with the given ID, if it exists. Otherwise, returns nullptr.
		virtual ManagerBase* _getManager(ManagerId id) const = 0;

	};

}