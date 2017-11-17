#include "hvn3.h"
#include "test/Global.h"
#include "test/LevelEditor.h"
#include "test/PhysicsRoom.h"

int main(int argc, char *argv[]) {

	// Initialize game properties.
	GameState.Initialize(argc, argv);
	GameState.Properties().DebugMode = true;
	GameState.Properties().ScalingMode = hvn3::ScalingMode::Full;
	GameState.Properties().DisplayFlags |= hvn3::DisplayFlags::Resizable;
	GameState.Properties().DisplaySize = hvn3::SizeI(640, 480);
	GameState.Properties().FrameRate = 60.0f;
	GameState.Properties().FixedFrameRate = true;
	
	auto room = hvn3::RoomPtr(new hvn3::Room(0, GameState.Properties().DisplaySize));
	GameState.Rooms().AddRoom(room);

	// Run the main game loop.
	GameState.Loop();

	return 0;

}