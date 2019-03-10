#pragma once

#include "hvn3/rooms/IScene.h"
#include "hvn3/rooms/ISceneTransition.h"
#include "hvn3/rooms/SceneManagerBase.h"
#include "hvn3/rooms/SceneState.h"

#include <memory>
#include <vector>

namespace hvn3 {

	class SceneManager :
		public SceneManagerBase {

		enum TRANSITION_STATE {
			NO_TRANSITION_PENDING,
			TRANSITION_PENDING,
			EXIT_IN_PROGRESS,
			ENTER_IN_PROGRESS
		};

		struct SceneInfo {
			std::unique_ptr<IScene> scene;
			SceneState persistentState;
		};

	public:
		SceneManager();
		~SceneManager();

		void GoToScene(scene_index sceneIndex) override;
		void GoToPreviousScene() override;
		void GoToNextScene() override;

		void ResetScene() override;
		bool IsSceneLoaded() const override;
		const IScene& Scene() const override;
		const scene_index SceneIndex() const override;
		size_t Count() const override;

		void SetSceneTransition(SceneTransition transition) override;

		template<typename SceneType, typename ...Args>
		void LoadScene(Args&&... args);
		template<typename SceneType, typename ...Args>
		scene_index AddScene(Args&&... args);
		template<typename TransitionType, typename ...Args>
		void SetSceneTransition(Args&&... args);

	protected:
		void LoadScene(std::unique_ptr<IScene>&& scene) override;
		scene_index AddScene(std::unique_ptr<IScene>&& scene) override;
		void SetSceneTransition(std::unique_ptr<ISceneTransition>&& transition) override;

		void OnUpdate(UpdateEventArgs& e) override;
		void OnDraw(DrawEventArgs& e) override;

	private:
		bool _loaded_scene;
		bool _loaded_temporary_scene;
		size_t _scene_index;
		std::vector<SceneInfo> _scenes;

		TRANSITION_STATE _transition_state;
		std::unique_ptr<ISceneTransition> _transition;
		size_t _transitioning_to_index;

		// Returns true if the manager is currently transitioning to a new scene.
		bool _isTransitioning() const;
		// If another transition is not already in progress, begins transitioning to the scene at the given index.
		bool _beginTransitionTo(scene_index sceneIndex);
		void _loadNextScene();

		SceneInfo& _getSceneInfo();
		void _updateScene();
		size_t _getPreviousSceneIndex() const;
		size_t _getNextSceneIndex() const;

	};

	template<typename SceneType, typename ...Args>
	void SceneManager::LoadScene(Args&&... args) {

		static_assert(std::is_base_of<IScene, SceneType>::value, "Type must implement IScene");

		std::unique_ptr<IScene> ptr = std::make_unique<SceneType>(std::forward<Args>...);

		LoadScene(std::move(ptr));

	}
	template<typename SceneType, typename ...Args>
	SceneManager::scene_index SceneManager::AddScene(Args&&... args) {

		static_assert(std::is_base_of<IScene, SceneType>::value, "Type must implement IScene");

		std::unique_ptr<IScene> ptr = std::make_unique<SceneType>(std::forward<Args>...);

		return AddScene(std::move(ptr));

	}
	template<typename TransitionType, typename ...Args>
	void SceneManager::SetSceneTransition(Args&&... args) {

		static_assert(std::is_base_of<ISceneTransition, TransitionType>::value, "Type must implement ISceneTransition");

		std::unique_ptr<ISceneTransition> ptr = std::make_unique<TransitionType>(std::forward<Args>...);

		SetSceneTransition(std::move(ptr));

	}

}