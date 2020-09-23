#pragma once

#include "hvn3/core/IDrawable.h"
#include "hvn3/core/IManager.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/rooms/IScene.h"
#include "hvn3/rooms/ISceneTransition.h"

#include <memory>
#include <utility>

namespace hvn3::scenes {

	class ISceneManager {

	public:
		typedef size_t index_type;
		typedef std::unique_ptr<IScene> scene_handle;
		typedef std::unique_ptr<ISceneTransition> transition_handle;

		virtual ~ISceneManager() = default;

		virtual void LoadScene(scene_handle&& scene) = 0;
		virtual index_type AddScene(scene_handle&& scene) = 0;

		virtual void GoToScene(index_type sceneIndex) = 0;
		virtual void GoToPreviousScene() = 0;
		virtual void GoToNextScene() = 0;

		virtual void SetSceneTransition(SceneTransition transition) = 0;
		virtual void SetSceneTransition(transition_handle&& transition) = 0;

		virtual const IScene& CurrentScene() const = 0;
		virtual const index_type CurrentIndex() const = 0;

		virtual void ReloadScene() = 0;
		virtual bool IsSceneLoaded() const = 0;

		virtual size_t Count() const = 0;

	};

}