#ifdef VULKAN
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
		VkPhysicalDevice m_physicalDevice;

#pragma endregion


	public:

		VulkanRenderer() = default;
		VulkanRenderer(GLFWwindow* winHandle);
		~VulkanRenderer();
		virtual void initialize() override;
		virtual void destroy() override;
#pragma region Vulkan Unique Functions
		void create_surface() {};
		void create_vk_instance();
		bool checkValidationLayerSupport();
		void pickPhysicalDevice();
		bool isDeviceSuitable(VkPhysicalDevice device);
		double ScorePhysicalDevice(VkPhysicalDevice& device);
		std::vector<std::string> getAllowedLayers();
#pragma endregion
	};
}
#endif