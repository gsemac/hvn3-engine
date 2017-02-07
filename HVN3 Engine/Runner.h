#pragma once
#include "Display.h"
#include "EventQueue.h"
#include "Timer.h"
#include "GameProperties.h"
#include "Graphics.h"

class Font;
class Scene;

class Runner {

public:
	Runner();
	Runner(const GameProperties& properties);
	Runner(const GameProperties& properties, Scene& scene);
	~Runner();

	// Processes the main game loop.
	virtual void Loop();

	// Returns an object representing a collection of game properties. 
	const GameProperties& Properties() const;

protected:
	// Renders the current game state to the display surface.
	virtual void Draw();
	// Waits for and processes the next event in the event queue.
	virtual void Update();

	// Returns a pointer to the default system font.
	const Font* SystemFont();
	void DrawFPS();

	virtual void OnTimerTick(Event& ev);
	virtual void OnDisplayClose(Event& ev);
	virtual void OnKeyDown(Event& ev);
	virtual void OnKeyUp(Event& ev);
	virtual void OnKeyChar(Event& ev);
	virtual void OnMouseButtonDown(Event& ev);
	virtual void OnMouseButtonUp(Event& ev);
	virtual void OnMouseAxes(Event& ev);
	virtual void OnMouseEnterDisplay(Event& ev);
	virtual void OnMouseLeaveDisplay(Event& ev);
	virtual void OnDisplayResize(Event& ev);
	virtual void OnDisplaySwitchOut(Event& ev);
	virtual void OnDisplaySwitchIn(Event& ev);
	virtual void OnRedraw();

private:
	bool __allow_redraw;
	bool __exit_loop;
	int __frames_skipped;
	EventQueue __event_queue;
	Timer __timer;

	Scene* __scene;
	Display __display;
	Font* __default_font;
	GameProperties __properties;
	Drawing::Graphics* __graphics;

	void ApplyScalingMode();

};