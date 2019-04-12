#pragma once

#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/rooms/IScene.h"
#include "hvn3/rooms/ISceneTransition.h"

#include <memory>
#include <utility>

namespace hvn3 {

	class ISceneManager {

	public:
		typedef size_t scene_index;

		virtual ~ISceneManager() = default;

		virtual void LoadScene(std::unique_ptr<IScene>&& scene) = 0;
		virtual scene_index AddScene(std::unique_ptr<IScene>&& scene) = 0;

		virtual void GoToScene(scene_index sceneIndex) = 0;
		virtual void GoToPreviousScene() = 0;
		virtual void GoToNextScene() = 0;

		virtual void SetSceneTransition(SceneTransition transition) = 0;
		virtual void SetSceneTransition(std::unique_ptr<ISceneTransition>&& transition) = 0;

		virtual void ResetScene() = 0;
		virtual bool IsSceneLoaded() const = 0;
		virtual const IScene& Scene() const = 0;
		virtual const scene_index SceneIndex() const = 0;
		virtual size_t Count() const = 0;

	};

}