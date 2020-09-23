//#pragma once
//#include "hvn3/core/ApplicationContext.h"
//#include "hvn3/core/ManagerRegistry.h"
//#include "hvn3/rooms/IRoom.h"
//
//#pragma warning(push)
//// Disables warning about SizeableBase providing implementation for ISizeable.
//#pragma warning(disable:4250) 
//
//namespace hvn3 {
//
//	class RoomBase :
//		public IRoom,
//		public SizeableBase<int> {
//
//	public:
//		RoomBase(RoomId id, const SizeI& size);
//
//		void OnUpdate(UpdateEventArgs& e) override;
//		void OnDraw(DrawEventArgs& e) override;
//
//		IObjectManager& Objects() override;
//		IBackgroundManager& Backgrounds() override;
//		IViewManager& Views() override;
//		ICollisionManager& Collisions() override;
//		Physics::IPhysicsManager& Physics() override;
//		TileManager& Tiles() override;
//
//		const IObjectManager& Objects() const override;
//		const IBackgroundManager& Backgrounds() const override;
//		const IViewManager& Views() const override;
//		const ICollisionManager& Collisions() const override;
//		const Physics::IPhysicsManager& Physics() const override;
//		const TileManager& Tiles() const override;
//
//		void SetBackgroundColor(const Color& color) override;
//		const Color& BackgroundColor() const override;
//		void SetBackground(const Background& value) override;
//
//		const View& CurrentView() const override;
//		RoomId Id() const override;
//		RectangleF VisiblePort() const override;
//		RectangleF VisibleRegion() const override;
//		RectangleF Bounds() const override;
//		bool Persistent() const override;
//		void SetPersistent(bool value) override;
//
//		void OnContextChanged(ContextChangedEventArgs& e) override;
//		void OnEnter(RoomEnterEventArgs& e) override;
//		void OnExit(RoomExitEventArgs& e) override;
//		void OnCreate(RoomCreateEventArgs& e) override;
//		bool IsReady() const;
//		void OnReset() override;
//		void OnRender(DrawEventArgs& e) override;
//
//	protected:
//		const ManagerRegistry& Local() const;
//		ManagerRegistry& Local();
//		const ApplicationContext& GetContext() const;
//
//	private:
//		RoomId _id;
//		Color _background_color;
//		bool _set_up;
//		bool _persistent;
//		ApplicationContext _context;
//		class ManagerRegistry _local;
//
//	};
//
//}
//
//#pragma warning(pop)