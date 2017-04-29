#pragma once
#include "UpdateEventArgs.h"
#include "BackgroundDrawEventArgs.h"
#include "BackgroundProperties.h"
#include "ResourceCollection.h"
#include "Background.h"
#include <cstdlib>

class IBackgroundManager {

public:
	virtual size_t BackgroundAdd(ResourceHandle<Background> background) = 0;
	virtual size_t BackgroundAdd(ResourceHandle<Background> background, bool is_foreground) = 0;
	virtual size_t BackgroundAdd(ResourceHandle<Background> background, BackgroundProperties properties) = 0;
	virtual void BackgroundRemove(size_t index) = 0;
	virtual const ResourceHandle<Background>& BackgroundAt(size_t index) const = 0;
	virtual BackgroundProperties& PropertiesAt(size_t index) = 0;
	virtual size_t BackgroundCount() const = 0;
	// Clears all backgrounds from the manager.
	virtual void Clear() = 0;

	// Updates the state of all backgrounds.
	virtual void Update(UpdateEventArgs& e) = 0;
	// Draws all backgrounds.
	virtual void DrawBackgrounds(BackgroundDrawEventArgs& e) = 0;
	// Draws all foregrounds.
	virtual void DrawForegrounds(BackgroundDrawEventArgs& e) = 0;

};