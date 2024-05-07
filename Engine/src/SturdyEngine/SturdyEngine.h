#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "Renderer/Vulkan/VulkanRenderer.h" //for now just include the vulkan header, we will not need anything else for nwo
namespace SFT {
	
	class SturdyEngine {
	private:
		VulkanRenderer m_renderer;
		Window m_window;

		static void resizeCallback(const input::ResizeEvent& ev) {
			VulkanRenderer* rend = static_cast<VulkanRenderer*>(ev.arb); //PERSONAL CONTRACT WITH CODE: Engine sets this from its renderer member at construction
			rend->resize(ev.size);
		}

	public:
		SturdyEngine();

		void run();
	};
}