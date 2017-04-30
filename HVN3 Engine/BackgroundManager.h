#pragma once
#include "IBackgroundManager.h"
#include <vector>
#include <utility>

class RoomBase;

class BackgroundManager : public IBackgroundManager {

	typedef std::pair<ResourceHandle<Background>, BackgroundProperties> bg_type;

public:
	virtual size_t BackgroundAdd(ResourceHandle<Background> background) override;
	virtual size_t BackgroundAdd(ResourceHandle<Background> background, bool is_foreground) override;
	virtual size_t BackgroundAdd(ResourceHandle<Background> background, BackgroundProperties properties) override;
	virtual void BackgroundRemove(size_t index) override;
	virtual const ResourceHandle<Background>& BackgroundAt(size_t index) const override;
	virtual BackgroundProperties& PropertiesAt(size_t index) override;
	virtual size_t BackgroundCount() const override;
	virtual void Clear() override;

	virtual void Update(UpdateEventArgs& e) override;
	virtual void DrawBackgrounds(BackgroundDrawEventArgs& e) override;
	virtual void DrawForegrounds(BackgroundDrawEventArgs& e) override;

protected:
	virtual void DrawBackground(BackgroundDrawEventArgs& e, const BackgroundManager::bg_type& background) const;

private:
	std::vector<bg_type> _backgrounds;

};