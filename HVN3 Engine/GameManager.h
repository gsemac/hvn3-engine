#pragma once
#include "Framework.h"
#include "RoomManager.h"
#include "Runner.h"
#include "ResourceManager.h"

class GameManager {

public:
	GameManager();
	virtual ~GameManager();
	virtual void Initialize(int argc, char* argv[]);
	virtual void Loop();

	Properties& Properties();
	RoomManager& RoomManager();
	ResourceManager& ResourceManager();
	Runner& Runner();

protected:
	virtual void Shutdown();

private:
	::Properties _properties;
	::RoomManager _room_manager;
	::ResourceManager _resource_manager;
	::Runner* _runner;

};