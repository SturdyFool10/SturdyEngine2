#include "../Renderer.h"
#ifdef _WIN64
	#define VK_USE_PLATFORM_WIN32_KHR
#elif defined __linux__
#endif
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include <stdexcept>
namespace SFT {
	class VulkanRenderer : public Renderer {
		GLFWwindow* m_win_handle;

#pragma region VulkanMembers
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		

#pragma endregion


	public:

		VulkanRenderer() = default;
		VulkanRenderer(GLFWwindow* winHandle);
		~VulkanRenderer();
		virtual void initialize() override;
		void create_surface();
		void create_vk_instance();
		virtual void destroy() override;
		bool checkValidationLayerSupport();
		std::vector<std::string> getAllowedLayers();
	};
}