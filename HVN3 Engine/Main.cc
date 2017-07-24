#include "HVN3.h"
#include "test/Global.h"

int main(int argc, char *argv[]) {
	
	// Initialize game properties.
	GameState.Initialize(argc, argv);
	GameState.Properties().DebugMode = true;
	GameState.Properties().Fps = 60.0f;
	GameState.Properties().FixedFrameRate = true;

	// Run the main game loop.
	GameState.Loop();

	return 0;

}