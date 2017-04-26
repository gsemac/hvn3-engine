#include "Game.h"

Game::Game() {

	Framework::Initialize();

}
void Game::Initialize(int argc, char* argv[]) {

	Framework::Initialize(argc, argv);

}
void Game::Loop() {

	_runner = new ::Runner(_properties, _room_manager);

	_runner->Loop();

	Shutdown();

}
void Game::Shutdown() {

	delete _runner;

	Framework::Shutdown();

}

::RoomManager& Game::RoomManager() {

	return _room_manager;

}
Runner& Game::Runner() {

	return *_runner;

}
Properties& Game::Properties() {

	return _properties;

}