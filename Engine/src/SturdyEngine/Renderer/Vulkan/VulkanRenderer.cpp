#ifdef VULKAN
#include "VulkanRenderer.h"
#include <iostream>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> disallowedLayers = {
	"VK_LAYER_LUNARG_api_dump",
	"VK_LAYER_LUNARG_gfxreconstruct",
	"VK_LAYER_KHRONOS_synchronization2",
	"VK_LAYER_LUNARG_monitor",
	"VK_LAYER_LUNARG_screenshot",
	"VK_LAYER_KHRONOS_profiles"
};

#ifdef DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif



namespace SFT {
	VulkanRenderer::VulkanRenderer(GLFWwindow* winHandle) {
		this->m_win_handle = winHandle;
	}
	VulkanRenderer::~VulkanRenderer() {
		this->destroy();
	}
#pragma region VKInstance Creation
	void VulkanRenderer::create_vk_instance() {
		std::vector<std::string> layers = getAllowedLayers();
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

		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		std::vector<const char*> layersC{};
		for (auto& i : layers) {
			layersC.emplace_back(i.c_str());
		}
		createInfo.ppEnabledLayerNames = layersC.data();

		if (vkCreateInstance(&createInfo, nullptr, &this->m_instance) != VK_SUCCESS) {
			throw std::runtime_error("Vulkan failed to create a instance! VulkanRenderer.cpp");
		}
	}

	bool VulkanRenderer::checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		bool layerFound = false;
		for (VkLayerProperties &i : availableLayers) {
			if (strcmp(i.layerName, "VK_LAYER_KHRONOS_validation") == 0) {
				layerFound = true;
			}
		}

		return layerFound;
	}

	std::vector<std::string> VulkanRenderer::getAllowedLayers() {
		std::vector<std::string> layers;
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
		for (VkLayerProperties& i : availableLayers) {
			bool layerAllowed = true;
			for (const char* j : disallowedLayers) {
				if (strcmp(i.layerName, j) == 0 or (strcmp(i.layerName, "VK_LAYER_KHRONOS_validation") == 0 and (enableValidationLayers == false))) {
					layerAllowed = false;
				}
			}
			if (layerAllowed) {
				layers.emplace_back(std::string(i.layerName));
			}
		}
		return layers;
	}
#pragma endregion

#pragma region Physical Device Creation
	void VulkanRenderer::pickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, nullptr);
		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}
		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, devices.data());
		double maxScore = 0;
		VkPhysicalDevice HighestScorer = VK_NULL_HANDLE;
		for (auto& i : devices) {
			double score = this->ScorePhysicalDevice(i);
			if (score > maxScore) {
				if (isDeviceSuitable(i)) {
					HighestScorer = i;
					maxScore = score;
				}
			}
		}

		if (HighestScorer == VK_NULL_HANDLE) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
		this->m_physicalDevice = HighestScorer;
	}

	bool VulkanRenderer::isDeviceSuitable(VkPhysicalDevice device) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		auto queFams = this->findQueueFamilies(device);
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && queFams.isComplete()) {
			return true;
		}
		return false;
	}
	double VulkanRenderer::ScorePhysicalDevice(VkPhysicalDevice& device) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
		double score = 0.;
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			score += deviceProperties.limits.maxFramebufferWidth;
			score += deviceProperties.limits.maxFramebufferHeight;
			score += deviceProperties.limits.maxMemoryAllocationCount;
			score += deviceFeatures.multiViewport * 10000;
			score += deviceFeatures.geometryShader * 10000;
		}

		return score;
	}

#pragma endregion

#pragma region QueueFamily Discovery
	QueueFamilyIndices VulkanRenderer::findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		// Logic to find queue family indices to populate struct with
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->m_surface, &presentSupport);
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}
			if (presentSupport) {
				indices.presentFamily = i;
			}

			i++;
		}

		return indices;
	}
#pragma endregion

#pragma region Logical Device Creation

	void VulkanRenderer::createLogicalDevice() {
		QueueFamilyIndices indices = findQueueFamilies(this->m_physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};
		vkGetPhysicalDeviceFeatures(m_physicalDevice, &deviceFeatures);

		VkDeviceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;
		auto layers = getAllowedLayers();
		std::vector<const char*> cLayers;
		for (auto& i : layers) {
			cLayers.push_back(i.c_str());
		}
		createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
		createInfo.ppEnabledLayerNames = cLayers.data();

		if (vkCreateDevice(this->m_physicalDevice, &createInfo, nullptr, &this->m_logicalDevice) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		vkGetDeviceQueue(this->m_logicalDevice, indices.graphicsFamily.value(), 0, &this->m_graphicsQueue);
	}


#pragma endregion

#pragma region Surface Creation

	void VulkanRenderer::createSurface() {
	}

	void VulkanRenderer::setupGLFWSurface() {
		VkSurfaceKHR surface = this->m_surface;
		auto glfwresult = glfwCreateWindowSurface(this->m_instance, this->m_win_handle, nullptr, &this->m_surface);
		if (glfwresult != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

#pragma endregion
	void VulkanRenderer::initialize() {
		this->create_vk_instance();
		this->createSurface();
		this->setupGLFWSurface();
		this->pickPhysicalDevice();
		this->createLogicalDevice();
	}

	void VulkanRenderer::destroy() {
		vkDestroyDevice(this->m_logicalDevice, nullptr);
		vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
		vkDestroyInstance(this->m_instance, nullptr);
	}
}
#endif