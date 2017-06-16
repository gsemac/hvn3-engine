#pragma once
#include "ISizeable.h"
#include "IUpdatable.h"
#include "IDrawable.h"
#include "ObjectManager.h"
#include "RoomEnterEventArgs.h"
#include "RoomExitEventArgs.h"
#include "DisplaySizeChangedEventArgs.h"
#include "Color.h"

namespace hvn3 {

	typedef int RoomId;

	class ObjectManager;

	class RoomBase : public IUpdatable, public IDrawable, public ISizeable<int> {
		friend class RoomController;

	public:
		RoomBase(RoomId id, const SizeI& size);
		RoomBase(RoomId id, const SizeI& size, std::unique_ptr<IObjectManager>& object_manager);
		virtual ~RoomBase();

		virtual void OnUpdate(UpdateEventArgs& e) override;
		virtual void OnDraw(DrawEventArgs& e) override;
		virtual void OnDisplaySizeChanged(DisplaySizeChangedEventArgs& e);

		void SetBackgroundColor(const Color& color);
		const Color& BackgroundColor() const;
		IObjectManager& Objects();
		virtual RoomId Id() const;
		bool Persistent() const;
		void SetPersistent(bool value);
		virtual RectangleF GetVisibleRegion();

		template<typename T, typename ... Args>
		static std::unique_ptr<RoomBase> Create(Args &&... args) {

			return std::make_unique<T>(std::forward<Args>(args)...);

		}

	protected:
		virtual void OnRoomEnter(RoomEnterEventArgs& e);
		virtual void OnRoomExit(RoomExitEventArgs& e);
		virtual void SetUp();
		virtual void Reset();
		virtual void Render(DrawEventArgs& e);

	private:
		std::unique_ptr<IObjectManager> _obj_manager;

		RoomId _id;
		Color _background_color;
		bool _set_up;
		bool _persistent;

	};

}