#pragma once
#include "hvn3/exceptions/Exception.h"
#include "hvn3/rooms/IRoom.h"
#include <unordered_map>

#pragma warning(push)
// Disables warning about SizeableBase providing implementation for ISizeable.
#pragma warning(disable:4250) 

namespace hvn3 {

	class RoomBase :
		public IRoom,
		public SizeableBase<int> {

	public:
		RoomBase(RoomId id, const SizeI& size);

		void OnUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

		IObjectManager& GetObjects() override;
		IBackgroundManager& GetBackgrounds() override;
		IViewManager& GetViews() override;
		ICollisionManager<IObject>& GetCollisions() override;
		Physics::IPhysicsManager& GetPhysics() override;
		TileManager& GetTiles() override;

		const IObjectManager& Objects() const override;
		const IBackgroundManager& Backgrounds() const override;
		const IViewManager& Views() const override;
		const ICollisionManager<IObject>& Collisions() const override;
		const Physics::IPhysicsManager& Physics() const override;
		const TileManager& Tiles() const override;

		void SetBackgroundColor(const Color& color) override;
		const Color& BackgroundColor() const override;
		void SetBackground(const Background& value) override;

		const View& CurrentView() const override;
		RoomId Id() const override;
		RectangleF VisiblePort() const override;
		RectangleF VisibleRegion() const override;
		RectangleF Bounds() const override;
		bool Persistent() const override;
		void SetPersistent(bool value) override;

	protected:
		hvn3::Context Context() override;
		void OnContextChanged(ContextChangedEventArgs& e) override;

		void OnEnter(RoomEnterEventArgs& e) override;
		void OnExit(RoomExitEventArgs& e) override;
		void OnCreate() override;
		bool IsReady() const;
		void OnReset() override;
		void OnRender(DrawEventArgs& e) override;

	private:
		void _addManager(ManagerId id, std::shared_ptr<ManagerBase>& manager) override;
		ManagerBase* _getManager(ManagerId id) const override;

		RoomId _id;
		Color _background_color;
		bool _set_up;
		bool _persistent;
		hvn3::Context _context;
		std::unordered_map<ManagerId, std::shared_ptr<ManagerBase>> _registered_managers;

	};

}

#pragma warning(pop)