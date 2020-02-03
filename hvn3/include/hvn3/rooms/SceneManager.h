#pragma once

#include "hvn3/core/ManagerBase.h"
#include "hvn3/events/EventDefs.h"
#include "hvn3/events/EventListenerBase.h"
#include "hvn3/events/EventManager.h"
#include "hvn3/rooms/IScene.h"
#include "hvn3/rooms/ISceneManager.h"
#include "hvn3/rooms/ISceneTransition.h"
#include "hvn3/rooms/SceneState.h"
#include "hvn3/services/di_service_container.h"

#include <memory>
#include <vector>

namespace hvn3::scenes {

	class SceneManager :
		ISceneManager,
		public EventListenerBase<events::UpdateEvents> {

		enum class TransitionState {
			NoTransitionPending,
			TransitionPending,
			ExitInProgress,
			EnterInProgress
		};

		struct SceneInfo {
			std::unique_ptr<IScene> scene;
			SceneState persistentState;
		};

	public:
		HVN3_INJECT(SceneManager(services::DIServiceContainer& services, IEventManager& eventManager));

		~SceneManager();

		void OnEvent(UpdateEventArgs& e) override;

		void LoadScene(scene_handle&& scene) override;
		index_type AddScene(scene_handle&& scene) override;

		void GoToScene(index_type sceneIndex) override;
		void GoToPreviousScene() override;
		void GoToNextScene() override;

		void SetSceneTransition(SceneTransition transition) override;
		void SetSceneTransition(transition_handle&& transition) override;

		const IScene& CurrentScene() const override;
		const index_type CurrentIndex() const override;

		void ReloadScene() override;
		bool IsSceneLoaded() const override;

		size_t Count() const override;

		template<typename SceneType, typename ...Args>
		void LoadScene(Args&&... args);
		template<typename SceneType>
		void LoadScene(const SceneType& scene);
		template<typename SceneType, typename ...Args>
		index_type AddScene(Args&&... args);
		template<typename TransitionType, typename ...Args>
		void SetSceneTransition(Args&&... args);

	private:
		IEventManager* eventManager;
		services::DIServiceContainer* services;

		bool _loaded_scene;
		bool _loaded_temporary_scene;
		size_t _scene_index;
		std::vector<SceneInfo> _scenes;

		TransitionState _transition_state;
		std::unique_ptr<ISceneTransition> _transition;
		size_t _transitioning_to_index;

		// Returns true if the manager is currently transitioning to a new scene.
		bool _isTransitioning() const;
		// If another transition is not already in progress, begins transitioning to the scene at the given index.
		bool _beginTransitionTo(index_type sceneIndex);
		void _loadNextScene();

		SceneInfo& _getSceneInfo();
		void _updateScene();
		size_t _getPreviousSceneIndex() const;
		size_t _getNextSceneIndex() const;

	};

	template<typename SceneType, typename ...Args>
	void SceneManager::LoadScene(Args&&... args) {

		static_assert(std::is_base_of<IScene, SceneType>::value, "Type must implement IScene");

		std::unique_ptr<IScene> ptr = std::make_unique<SceneType>(std::forward<Args>(args)...);

		LoadScene(std::move(ptr));

	}
	template<typename SceneType>
	void SceneManager::LoadScene(const SceneType& scene) {

		static_assert(std::is_base_of<IScene, SceneType>::value, "Type must implement IScene");

		std::unique_ptr<IScene> ptr = std::make_unique<SceneType>(scene);

		LoadScene(std::move(ptr));

	}
	template<typename SceneType, typename ...Args>
	SceneManager::index_type SceneManager::AddScene(Args&&... args) {

		static_assert(std::is_base_of<IScene, SceneType>::value, "Type must implement IScene");

		std::unique_ptr<IScene> ptr = std::make_unique<SceneType>(std::forward<Args>(args)...);

		return AddScene(std::move(ptr));

	}
	template<typename TransitionType, typename ...Args>
	void SceneManager::SetSceneTransition(Args&&... args) {

		static_assert(std::is_base_of<ISceneTransition, TransitionType>::value, "Type must implement ISceneTransition");

		std::unique_ptr<ISceneTransition> ptr = std::make_unique<TransitionType>(std::forward<Args>(args)...);

		SetSceneTransition(std::move(ptr));

	}

}