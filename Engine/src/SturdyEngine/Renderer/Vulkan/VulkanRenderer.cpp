#include "VulkanRenderer.h"
namespace SFT {
	VulkanRenderer::VulkanRenderer(GLFWwindow* winHandle) {
		this->m_win_handle = winHandle;
		this->initialize();
		this->setupMainLoop();
		this->destroy();
	}
	VulkanRenderer::~VulkanRenderer() {
		this->destroy();
	}
	
	void VulkanRenderer::create_surface() {
	}

	void VulkanRenderer::create_vk_instance() {
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "SturdyEngine Application";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "SturdyEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_3;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = 0;

		if (vkCreateInstance(&createInfo, nullptr, &this->m_instance) != VK_SUCCESS) {
			throw std::runtime_error("Vulkan failed to create a instance! VulkanRenderer.cpp");
		}

	}



	void VulkanRenderer::initialize() {

		
		this->create_vk_instance();
		this->create_surface();
	}
}