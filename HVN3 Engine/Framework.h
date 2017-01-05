#pragma once

class Framework {

public:
	static void Initialize();
	static void Shutdown();

private:
	static bool __initialized;

	static void InitializeUnderlyingFramework();
	static void ShutdownUnderlyingFramework();
	
};