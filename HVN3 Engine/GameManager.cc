#include "GameManager.h"

GameManager::GameManager() {

	Framework::Initialize();

}
GameManager::~GameManager() {

	Shutdown();

}
void GameManager::Initialize(int argc, char* argv[]) {

	Framework::Initialize(argc, argv);

}
void GameManager::Loop() {

	_runner = new ::Runner(_properties, _room_manager);

	_runner->Loop();

}
void GameManager::Shutdown() {

	delete _runner;

	Framework::Shutdown();

}

::RoomManager& GameManager::RoomManager() {

	return _room_manager;

}
Runner& GameManager::Runner() {

	return *_runner;

}
Properties& GameManager::Properties() {

	return _properties;

}