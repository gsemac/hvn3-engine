#pragma once
#include "Runner.h"
#define InitializeFramework() Framework::Initialize(argc, argv); {
#define ShutdownFramework() } Framework::Shutdown();

class Framework {

public:
	Framework();
	Framework(int argc, char *argv[]);
	~Framework();

	static void Initialize();
	static void Initialize(int argc, char *argv[]);
	static void Shutdown();

	static void Loop(Runner& runner);

	static bool Initialized();

protected:
	static void InitializeUnderlyingFramework();
	static void ShutdownUnderlyingFramework();

	static bool __initialized;

};