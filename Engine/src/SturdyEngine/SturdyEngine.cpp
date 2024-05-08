#include "SturdyEngine.h"
namespace SFT {
	SturdyEngine::SturdyEngine() {
		this->m_window = new Window(800, 600, "SturdyEngine App", true);
#ifdef VULKAN
		this->m_renderer = new VulkanRenderer(this->m_window->getHandle());
#endif
		this->m_renderer->initialize();
		size_t callbackID = this->m_window->addResizeHandler(resizeCallback);
		this->m_window->setResizeUserPointer(this->m_renderer, callbackID);
	}
	SturdyEngine::~SturdyEngine() {
		delete this->m_renderer;
		delete this->m_window;
	}
	void SturdyEngine::run() {
		if (this->m_hasFuncs[0]) {
			this->m_setupFn();
		}
		auto timePoint = std::chrono::high_resolution_clock::now();
		while (!this->m_window->shouldWindowClose()) {
			this->m_window->pollEvents();
			auto newTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> delta = newTime - timePoint;
			if (this->m_hasFuncs[1]) {
				this->m_mainFn(delta.count());
			}
		}
		if (this->m_hasFuncs[2]) {
			this->m_cleanFn();
		}
	}

	void SturdyEngine::setMainFn(MainLoopFunction fn) {
		this->m_mainFn = fn;
		this->m_hasFuncs[1] = true;
	}
	void SturdyEngine::setSetupFn(SetupFunction fn) {
		this->m_setupFn = fn;
		this->m_hasFuncs[0] = true;
	};
	void SturdyEngine::setCleanFn(CleanFunction fn) {
		this->m_cleanFn = fn;
		this->m_hasFuncs[2] = true;
	};
}