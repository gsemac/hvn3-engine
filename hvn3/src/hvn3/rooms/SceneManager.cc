#include "hvn3/rooms/SceneManager.h"
#include "hvn3/rooms/SceneTransitionFade.h"

namespace hvn3 {

	// SceneManager

	// Public methods

	SceneManager::SceneManager() :
		_loaded_scene(false),
		_loaded_temporary_scene(false),
		_scene_index(0),
		_transitioning_to_index(0),
		_transition_state(NO_TRANSITION_PENDING),
		_transition(nullptr) {}
	SceneManager::~SceneManager() {}

	void SceneManager::GoToScene(scene_index sceneIndex) {

		assert(sceneIndex >= 0);
		assert(sceneIndex < _scenes.size());

		_beginTransitionTo(sceneIndex);

	}
	void SceneManager::GoToPreviousScene() {

		assert(_scenes.size() > 0);

		_beginTransitionTo(_getPreviousSceneIndex());

	}
	void SceneManager::GoToNextScene() {

		assert(_scenes.size() > 0);

		_beginTransitionTo(_getNextSceneIndex());

	}
	void SceneManager::ResetScene() {

		// Resets the current scene by exiting it, resetting the room state, and then entering it again.

		assert(IsSceneLoaded());

		IScene::ExitEventArgs exit_args(Context());

		_getSceneInfo().scene->OnExit(exit_args);

		// #todo Reset the state of all relevant managers before re-entering the scene.

		// Call appropriate event handlers to set up the scene.

		IScene::CreateEventArgs create_args(Context());
		IScene::EnterEventArgs enter_args(Context());

		_getSceneInfo().scene->OnEnter(enter_args);
		_getSceneInfo().scene->OnCreate(create_args);

	}
	bool SceneManager::IsSceneLoaded() const {

		return _loaded_scene;

	}
	const IScene& SceneManager::Scene() const {

		assert(IsSceneLoaded());

		assert(_scenes.size() > 0);
		assert(_scene_index >= 0);
		assert(_scene_index < _scenes.size());

		return *_scenes[_scene_index].scene.get();

	}
	const SceneManager::scene_index SceneManager::SceneIndex() const {

		return _scene_index;

	}
	size_t SceneManager::Count() const {

		return _scenes.size();

	}
	void SceneManager::SetSceneTransition(SceneTransition transition) {

		switch (transition) {

		case SceneTransition::Fade:
			SetSceneTransition(std::make_unique<SceneTransitionFade>(Color::Black, true));
			break;

		default:
		case SceneTransition::None:
			SetSceneTransition(std::make_unique<SceneTransitionNone>());
			break;

		}

	}

	// Protected methods

	void SceneManager::LoadScene(std::unique_ptr<IScene>&& scene) {

		// Scenes loaded in this way will be temporary, and won't be permanently part of the scene collection.

		_loaded_temporary_scene = true;

		SceneInfo info;
		info.scene = std::move(scene);

		_scenes.push_back(std::move(info));

		_beginTransitionTo(_scenes.size() - 1);

	}
	SceneManager::scene_index SceneManager::AddScene(std::unique_ptr<IScene>&& scene) {

		SceneInfo info;
		info.scene = std::move(scene);

		_scenes.push_back(std::move(info));

		return _scenes.size() - 1;

	}
	void SceneManager::SetSceneTransition(std::unique_ptr<ISceneTransition>&& transition) {

		_transition = std::move(transition);

	}

	void SceneManager::OnUpdate(UpdateEventArgs& e) {

		// Update the state of the room if there is nothing blocking updates.
		if (IsSceneLoaded() && (!_isTransitioning() || !_transition || !_transition->Blocking()))
			_updateScene();

		else if (!IsSceneLoaded() && _scenes.size() > 0) {

			// If no scene is loaded but scenes have been added, load the first scene.
			GoToScene(0);

		}

		switch (_transition_state) {

		case TRANSITION_PENDING:

			if (!_transition) {

				// If the transition is null (no transition), just perform the change immediately.

				_loadNextScene();
				_transition_state = NO_TRANSITION_PENDING;

			}
			else {

				_transition->ExitBegin();
				_transition_state = EXIT_IN_PROGRESS;

			}

			break;

		case EXIT_IN_PROGRESS:

			// Update the state of the scene transition. 
			// If the transition is complete, load the next scene and begin the enter transition.

			if (_transition->ExitStep(e)) {

				_transition->ExitEnd();

				_loadNextScene();

				_transition->EnterBegin();
				_transition_state = ENTER_IN_PROGRESS;

			}

			break;

		case ENTER_IN_PROGRESS:

			// Update the state of the scene transition. 
			// If the transition is complete, reset the transition state to allow future scene changes.

			if (_transition->EnterStep(e)) {

				_transition->EnterEnd();
				_transition_state = NO_TRANSITION_PENDING;

			}

			break;

		}

	}
	void SceneManager::OnDraw(DrawEventArgs& e) {

		if (!IsSceneLoaded())
			return;

		e.Graphics().Clear(_getSceneInfo().scene->BackgroundColor());

		_transition->OnDraw(e);

	}

	// Private methods

	bool SceneManager::_isTransitioning() const {

		return _transition_state != NO_TRANSITION_PENDING;

	}
	bool SceneManager::_beginTransitionTo(scene_index sceneIndex) {
		std::cout << "is trans: " << _isTransitioning() << std::endl;
		if (_isTransitioning())
			return false;

		assert(sceneIndex >= 0);
		assert(sceneIndex < _scenes.size());

		_transitioning_to_index = sceneIndex;

		if (!IsSceneLoaded()) {

			// If this is the first scene to be loaded, skip the transition animation.
			_loadNextScene();

		}
		else
			_transition_state = TRANSITION_PENDING;

		return true;

	}
	void SceneManager::_loadNextScene() {

		if (IsSceneLoaded()) {

			// Call the exit event for the current scene.

			IScene::ExitEventArgs exit_args(Context());

			_getSceneInfo().scene->OnExit(exit_args);

			// #todo If the room is flagged as persistent, save its state to restore later.

		}

		// Set the new scene to the current scene.
		_scene_index = _transitioning_to_index;

		// A scene has been loaded, so set the loaded scene flag.
		_loaded_scene = true;

		// Call the appropriate events on the new scene.

		IScene::CreateEventArgs create_args(Context());
		IScene::EnterEventArgs enter_args(Context());

		_getSceneInfo().scene->OnEnter(enter_args);
		_getSceneInfo().scene->OnCreate(create_args);

	}

	SceneManager::SceneInfo& SceneManager::_getSceneInfo() {

		assert(_scene_index >= 0);
		assert(_scene_index < _scenes.size());
		assert(static_cast<bool>(_scenes[_scene_index].scene));

		return _scenes[_scene_index];

	}
	void SceneManager::_updateScene() {



	}
	size_t SceneManager::_getPreviousSceneIndex() const {

		// Treat the scene collection as circular.
		size_t index = (_scene_index <= 0) ? _scenes.size() - 1 : _scene_index - 1;

		return index;

	}
	size_t SceneManager::_getNextSceneIndex() const {

		// Treat the scene collection as circular.
		size_t index = (_scene_index >= _scenes.size() - 1) ? 0 : _scene_index + 1;

		return index;

	}

}