#pragma once
#define InitializeFramework() Framework::Initialize(argc, argv); {
#define ShutdownFramework() } Framework::Shutdown();

class Framework {

public:
	static void Initialize();
	static void Initialize(int argc, char *argv[]);
	static void Shutdown();

private:
	static bool __initialized;

	static void InitializeUnderlyingFramework();
	static void ShutdownUnderlyingFramework();
	
};