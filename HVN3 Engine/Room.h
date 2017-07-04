#pragma once
#include "RoomBase.h"
#include "BackgroundManager.h"
#include "ViewManager.h"
#include "CollisionManager.h"

namespace hvn3 {

	// This class extends the responsibilites of the room class to include background, tile, and view management.
	class Room : public RoomBase {

	public:
		Room(RoomId id, const SizeI& size);
		virtual ~Room();

		const View* CurrentView();
		IBackgroundManager& Backgrounds();
		IViewManager& Views();
		ICollisionManager& Collisions();

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;

		virtual RectangleF GetVisibleRegion() override;

		template<typename T, typename ... Args>
		static std::unique_ptr<Room> Create(Args &&... args) {

			return std::make_unique<T>(std::forward<Args>(args)...);

		}

	protected:
		virtual void Reset() override;
		virtual void Render(DrawEventArgs& e) override;

	private:
		hvn3::CollisionManager _collision_manager;
		hvn3::BackgroundManager _background_manager;
		hvn3::ViewManager _view_manager;

		size_t _rendering_view;

	};

}