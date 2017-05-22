#pragma once
#include "RoomBase.h"
#include "BackgroundManager.h"
#include "ViewManager.h"

namespace hvn3 {

	// This class extends the responsibilites of the room class to include background, tile, and view management.
	class Room : public RoomBase {

	public:
		Room(unsigned int width, unsigned int height);
		virtual ~Room();

		const View* CurrentView();
		IBackgroundManager* BackgroundManager();
		IViewManager* ViewManager();

		virtual void Update(UpdateEventArgs& e) override;
		virtual void Draw(DrawEventArgs& e) override;

		template<typename T, typename ... Args>
		static std::unique_ptr<Room> Create(Args &&... args) {

			return std::make_unique<T>(std::forward<Args>(args)...);

		}

	protected:
		virtual void Reset() override;
		virtual void Render(DrawEventArgs& e) override;

	private:
		hvn3::BackgroundManager _background_manager;
		hvn3::ViewManager _view_manager;

		size_t _rendering_view;

	};

}