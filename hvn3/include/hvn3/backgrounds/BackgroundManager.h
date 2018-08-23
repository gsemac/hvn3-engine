#pragma once
#include "hvn3/backgrounds/IBackgroundManager.h"
#include <vector>
#include <utility>

namespace hvn3 {

	class BackgroundManager : public IBackgroundManager {

	public:
		size_t Add(const Background& background) override;
		size_t Add(const Background& background, bool foreground) override;
		void Remove(size_t index) override;
		const Background& At(size_t index) const override;
		size_t Count() const override;
		void Clear() override;

		void ForEach(const std::function<bool(Background&)>& func) override;
		void ForEach(const std::function<bool(const Background&)>& func) const override;

		void Update(UpdateEventArgs& e) override;
		void Draw(BackgroundDrawEventArgs& e) override;

	private:
		std::vector<Background> _backgrounds;

		void _drawBackground(BackgroundDrawEventArgs& e, const Background& background) const;

	};

}