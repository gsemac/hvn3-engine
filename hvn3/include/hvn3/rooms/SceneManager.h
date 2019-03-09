#pragma once

#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/core/ManagerBase.h"
#include "hvn3/rooms/Scene.h"
#include "hvn3/rooms/SceneTransition.h"

#include <memory>
#include <vector>

namespace hvn3 {

	class SceneManager :
		public ManagerBase,
		public IUpdatable,
		public IDrawable {

		enum ROOM_TRANSITION_STATE {
			NO_TRANSITION_PENDING,
			TRANSITION_PENDING,
			EXIT_IN_PROGRESS,
			ENTER_IN_PROGRESS
		};

	public:
		SceneManager();
		~SceneManager();

		void LoadScene(const Scene& scene);
		void LoadScene(int sceneIndex);
		template<typename SceneType, typename ...Args>
		void LoadScene(Args&&... args);

		void AddScene(const Scene& scene);
		template<typename SceneType, typename ...Args>
		void AddScene(Args&&... args);

		void LoadPreviousScene();
		void LoadNextScene();

		void SetSceneTransition(RoomTransition transition);
		template<typename TransitionType, typename ...Args>
		void SetSceneTransition(Args&&... args);

		void ResetScene();
		bool IsSceneLoaded() const;

		void OnUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

	protected:
		void SetSceneTransition(std::unique_ptr<ISceneTransition>&& transition);

		void _loadNextRoom();
		// If the current room is not null, resets the room and calls the on exit event; otherwise, does nothing.
		void _exitRoom();
		void _callRoomOnContextChanged() const;

		bool _roomTransitionIsInProgress() const;
		void _beginRoomTransition(IRoomPtr& next_room);

	private:
		IRoomPtr _current_room;
		IRoomPtr _next_room;
		ROOM_TRANSITION_STATE _room_transition_state;
		std::unique_ptr<RoomTransitionBase> _transition;
		ApplicationContext _context;

	};

}