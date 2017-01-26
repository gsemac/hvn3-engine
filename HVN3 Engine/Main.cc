#include "HVN3.h"
//#include "ExampleGame.h"
#include "ExampleGame2.h"
#include "Display.h"
#include "Graphics.h"

int main(int argc, char *argv[]) {

	//InitializeFramework();

	//Display disp(640, 480);
	//Display disp2(640, 480);

	//while (1) {
	//
	//	Drawing::Graphics(disp.BackBuffer()).DrawCircle(100, 100, 100, Color::White, 2);
	//	disp.Refresh();

	//	Drawing::Graphics(disp2.BackBuffer()).DrawCircle(100, 100, 100, Color::DodgerBlue, 2);
	//	disp2.Refresh();

	//}

	//ShutdownFramework();

	SuperMarioBros::RunGame(argc, argv);

	/*try {

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

	}*/

	return 0;

}