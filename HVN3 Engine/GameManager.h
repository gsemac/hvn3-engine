#pragma once
#include "Framework.h"
#include "RoomManager.h"
#include "Runner.h"

class GameManager {

public:
	GameManager();
	~GameManager();
	void Initialize(int argc, char* argv[]);
	void Loop();

	RoomManager& RoomManager();
	Runner& Runner();
	Properties& Properties();

protected:
	void Shutdown();

private:
	::Properties _properties;
	::RoomManager _room_manager;
	::Runner* _runner;

};