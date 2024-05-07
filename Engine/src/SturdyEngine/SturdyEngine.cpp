#include "SturdyEngine.h"
namespace SFT {
	SturdyEngine::SturdyEngine() {
		this->m_window = Window(800, 600, "SturdyEngine App", true);
		this->m_renderer = VulkanRenderer(this->m_window.getHandle());
		this->m_renderer.initialize();
		size_t callbackID = this->m_window.addResizeHandler(resizeCallback);
		this->m_window.setResizeUserPointer(&this->m_renderer, callbackID);
	}
	void SturdyEngine::run() {
		while (!this->m_window.shouldWindowClose()) {
			this->m_window.pollEvents();
		}
	}
}