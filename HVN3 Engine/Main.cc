#include "HVN3.h"
#include "ExampleGame.h"
#include "ExampleGame2.h"

int main(int argc, char *argv[]) {

	try {

		// Initialize the Framework.
		Framework::Initialize();

		// Set up Game Properties.
		GameProperties properties;
		properties.DisplayTitle = "HVN3 Engine";
		properties.OutsideColor = Color::Black;
		properties.DebugMode = true;
		properties.ScalingMode = ScalingMode::MaintainAspectRatio;

		// Set up the initial Scene.
		Scene scene(properties.DisplaySize.Width(), properties.DisplaySize.Height(), nullptr);
		scene.SetBackgroundColor(Color::Red);
		scene.AddObject(new RotatingBox(100.0f, 100.0f));
		scene.AddObject(new RotatingBox(500.0f, 100.0f));

		// Create a new Runner instance to handle the game logic.
		Runner(properties, scene).Loop();

		// Shut down the Framework.
		Framework::Shutdown();

	}
	catch (std::exception& ex) {

		// Output the error message.
		std::cout << ex.what();

		// Rethrow the error, since we cannot handle it here.
		throw;

	}

	return 0;

}