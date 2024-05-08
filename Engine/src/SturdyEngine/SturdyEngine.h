#include "Window/Window.h"
#define VULKAN
#include "Renderer/Renderer.h"
#include <functional>
#include <chrono>
#include "Renderer/Vulkan/VulkanRenderer.h" //for now just include the vulkan header, we will not need anything else for nwo
namespace SFT {
	
	using MainLoopFunction = std::function<void(double felapsedTime)>; //define the criteria a function must meet for it to be counted as a mainloop function
	using CleanFunction = std::function<void()>;
	using SetupFunction = std::function<void()>;

	class SturdyEngine {
	private:
		VulkanRenderer* m_renderer;
		Window* m_window;
		MainLoopFunction m_mainFn;
		SetupFunction m_setupFn;
		CleanFunction m_cleanFn;
		bool m_hasFuncs[3] = { false };
		static void resizeCallback(const input::ResizeEvent& ev) {
			VulkanRenderer* rend = static_cast<VulkanRenderer*>(ev.arb); //PERSONAL CONTRACT WITH CODE: Engine sets this from its renderer member at construction
			rend->resize(ev.size);
		}

	public:
		SturdyEngine();
		~SturdyEngine();
		void run();
		void setMainFn(MainLoopFunction fn);
		void setSetupFn(SetupFunction fn);
		void setCleanFn(CleanFunction fn);
	};
}