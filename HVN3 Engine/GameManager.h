#pragma once
#include "Framework.h"
#include "RoomManager.h"
#include "Runner.h"
#include "ResourceManager.h"

class GameManager {

public:
	GameManager();
	~GameManager();
	void Initialize(int argc, char* argv[]);
	void Loop();

	Properties& Properties();
	RoomManager& RoomManager();
	ResourceManager& Resources();
	Runner& Runner();

protected:
	void Shutdown();

private:
	::Properties _properties;
	::RoomManager _room_manager;
	::ResourceManager _resource_manager;
	::Runner* _runner;

};