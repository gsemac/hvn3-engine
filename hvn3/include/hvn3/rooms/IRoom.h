#pragma once
#include "hvn3/core/Context.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/IDrawable.h"
#include "hvn3/graphics/DisplaySizeChangedEventArgs.h"
#include "hvn3/graphics/Color.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/rooms/RoomEnterEventArgs.h"
#include "hvn3/rooms/RoomExitEventArgs.h"
#include "hvn3/rooms/RoomTypeDefs.h"
#include "hvn3/collision/ICollisionManager.h"

namespace hvn3 {

	class View;
	class IObjectManager;
	class IBackgroundManager;
	class IViewManager;
	class IObject;

	namespace System {
		class RoomController;
	}

	class IRoom : public IUpdatable, public IDrawable, public virtual ISizeable<int> {
		friend class System::RoomController;

	public:
		virtual ~IRoom() = default;

		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e) = 0;

		virtual void SetBackgroundColor(const Color& color) = 0;
		virtual const Color& BackgroundColor() const = 0;

		virtual const View* CurrentView() const = 0;

		virtual IObjectManager& Objects() = 0;
		virtual IBackgroundManager& Backgrounds() = 0;
		virtual IViewManager& Views() = 0;
		virtual ICollisionManager<IObject>& Collisions() = 0;
		virtual Physics::IPhysicsManager& Physics() = 0;

		virtual const IObjectManager& Objects() const = 0;
		virtual const IBackgroundManager& Backgrounds() const = 0;
		virtual const IViewManager& Views() const = 0;
		virtual const ICollisionManager<IObject>& Collisions() const = 0;
		virtual const Physics::IPhysicsManager& Physics() const = 0;

		virtual RoomId Id() const = 0;
		virtual bool Persistent() const = 0;
		virtual void SetPersistent(bool value) = 0;
		virtual RectangleF GetVisibleRegion() = 0;
		virtual RectangleF Bounds() const = 0;

		virtual void Restart() = 0;

		virtual hvn3::Context Context() = 0;
		virtual void SetContext(hvn3::Context context) = 0;

	protected:
		virtual void OnRoomEnter(RoomEnterEventArgs& e) = 0;
		virtual void OnRoomExit(RoomExitEventArgs& e) = 0;
		virtual void OnSetUp() = 0;
		virtual bool IsSetUp() const = 0;
		virtual void OnReset() = 0;
		virtual void OnRender(DrawEventArgs& e) = 0;

	};

}