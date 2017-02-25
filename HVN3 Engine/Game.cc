#include "Game.h"
#include "Framework.h"

void Game::Init() {

	// Initialize the framework.
	Framework::Initialize();

}
void Game::Run() {



}
void Game::End() {

	// Dispose of resources.
	//__sprites.Clear();
	//__backgrounds.Clear();

	// Shut down the framework.
	Framework::Shutdown();

}