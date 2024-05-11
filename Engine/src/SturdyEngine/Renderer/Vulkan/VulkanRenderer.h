#ifdef VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../Renderer.h"
#include "glslang.h"
#include <GLFW/glfw3.h>
#include <set>
#include <GLFW/glfw3native.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <optional>
#include <limits> 
#include <algorithm>
namespace SFT {


	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


	class VulkanRenderer : public Renderer {
	private:

#pragma region Vulkan Unique Functions
		void create_surface() {};
		void create_vk_instance();
		bool checkValidationLayerSupport();
		void pickPhysicalDevice();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		double ScorePhysicalDevice(VkPhysicalDevice& device);
		std::vector<std::string> getAllowedLayers();
		void createLogicalDevice();
		void setupGLFWSurface();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> extensionsList);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void createSwapChain();
		void createImageViews();
		void createGraphicsPipeline();
		void compile_shader(std::string name);
		void compileShaders();
#pragma endregion
		GLFWwindow* m_win_handle;

#pragma region VulkanMembers
		VkInstance m_instance;
		VkSurfaceKHR m_surface;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_swapChainImages;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		std::vector<VkImageView> m_swapChainImageViews;
		std::vector<std::vector<uint32_t>> m_CompiledShaders;
#pragma endregion


	public:

		VulkanRenderer() = default;
		VulkanRenderer(GLFWwindow* winHandle);
		~VulkanRenderer();
		virtual void initialize() override;
		virtual void destroy() override;
	};
}
#endif