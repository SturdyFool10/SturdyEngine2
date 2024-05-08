#include "Window/Window.h"
#include "Renderer/Renderer.h"
#ifdef VULKAN
#include "Renderer/Vulkan/VulkanRenderer.h" //for now just include the vulkan header, we will not need anything else for nwo
#endif
namespace SFT {
	
	class SturdyEngine {
	private:
#ifdef VULKAN
		VulkanRenderer* m_renderer;
#endif
		Window* m_window;

		static void resizeCallback(const input::ResizeEvent& ev) {
#ifdef VULKAN
			VulkanRenderer* rend = static_cast<VulkanRenderer*>(ev.arb); //PERSONAL CONTRACT WITH CODE: Engine sets this from its renderer member at construction
#endif
			rend->resize(ev.size);
		}

	public:
		SturdyEngine();
		~SturdyEngine();
		void run();
	};
}