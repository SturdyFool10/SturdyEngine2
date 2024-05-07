#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Vulkan/VulkanRenderer.h" //for now just include the vulkan header, we will not need anything else for nwo
namespace SFT {
	class SturdyEngine {
	private:
		Renderer m_renderer;
		Window m_window;
	public:
		SturdyEngine() {
			this->m_window = Window(800, 600, "SturdyEngine App", true);
			this->m_renderer = VulkanRenderer(this->m_window.getHandle());
			this->m_renderer.initialize();
		}
	};
}