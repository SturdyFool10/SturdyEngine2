#ifdef VULKAN
#include "VulkanRenderer.h"
#include <iostream>
#include <filesystem>
#include "glslang.h"
#include <map>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> disallowedLayers = {
	"VK_LAYER_LUNARG_api_dump",
	"VK_LAYER_LUNARG_gfxreconstruct",
	"VK_LAYER_KHRONOS_synchronization2",
	"VK_LAYER_LUNARG_monitor",
	"VK_LAYER_LUNARG_screenshot",
	"VK_LAYER_KHRONOS_profiles",
	"VK_LAYER_VALVE_steam_fossilize"
};
const std::vector<const char*> essentialExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef DEBUG
	const bool enableValidationLayers = true;
#else
	const bool enableValidationLayers = false;
#endif

constexpr int SWAPCHAIN_PREFERRED_IMAGE_COUNT = 3;
namespace fs = std::filesystem;

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
			//std::cout << "Layer: " << i.layerName << "found..." << std::endl;
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
		bool requiredExtensionsSupported = checkDeviceExtensionSupport(device, essentialExtensions);
		bool swapChainAdequate = false;
		if (requiredExtensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}
		
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader && queFams.isComplete() && requiredExtensionsSupported && swapChainAdequate) {
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

	bool VulkanRenderer::checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> extensionsList) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(extensionsList.begin(), extensionsList.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->m_surface, &details.capabilities);
		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->m_surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->m_surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->m_surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->m_surface, &presentModeCount, details.presentModes.data());
		}

		return details;
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
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(this->m_physicalDevice, nullptr, &extensionCount, nullptr);
		createInfo.enabledExtensionCount = static_cast<uint32_t>(essentialExtensions.size());
		createInfo.ppEnabledExtensionNames = essentialExtensions.data();
		if (vkCreateDevice(this->m_physicalDevice, &createInfo, nullptr, &this->m_logicalDevice) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}
		vkGetDeviceQueue(this->m_logicalDevice, indices.graphicsFamily.value(), 0, &this->m_graphicsQueue);
	}


#pragma endregion

#pragma region Surface Creation

	void VulkanRenderer::setupGLFWSurface() {
		VkSurfaceKHR surface = this->m_surface;
		auto glfwresult = glfwCreateWindowSurface(this->m_instance, this->m_win_handle, nullptr, &this->m_surface);
		if (glfwresult != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}


#pragma endregion

#pragma region Swapchain Creation

	VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}
		return availableFormats[0];
	}

	VkExtent2D VulkanRenderer::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			int width, height;
			glfwGetFramebufferSize(this->m_win_handle, &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	void VulkanRenderer::createSwapChain() {
		SwapChainSupportDetails swapChainSupport = this->querySwapChainSupport(this->m_physicalDevice);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);
		uint32_t MinImageCount = swapChainSupport.capabilities.minImageCount;
		uint32_t realImageCount = MinImageCount + 1;
		uint32_t prefferedImageCount = SWAPCHAIN_PREFERRED_IMAGE_COUNT;
		if (prefferedImageCount > MinImageCount) {
			realImageCount = prefferedImageCount;
		}
		if (swapChainSupport.capabilities.maxImageCount > 0 && realImageCount > swapChainSupport.capabilities.maxImageCount) {
			realImageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = this->m_surface;

		QueueFamilyIndices indices = findQueueFamilies(this->m_physicalDevice);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0; // Optional
			createInfo.pQueueFamilyIndices = nullptr; // Optional
		}
		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(this->m_logicalDevice, &createInfo, nullptr, &this->m_swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(this->m_logicalDevice, this->m_swapChain, &realImageCount, nullptr);
		this->m_swapChainImages.resize(realImageCount);
		vkGetSwapchainImagesKHR(this->m_logicalDevice, this->m_swapChain, &realImageCount, this->m_swapChainImages.data());

		this->m_swapChainImageFormat = surfaceFormat.format;
		this->m_swapChainExtent = extent;
	}


#pragma endregion

#pragma region CreateImageViews

	void VulkanRenderer::createImageViews() {
		this->m_swapChainImageViews.resize(this->m_swapChainImages.size());
		for (size_t i = 0; i < this->m_swapChainImages.size(); i++) {
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = this->m_swapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = this->m_swapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;
			if (vkCreateImageView(this->m_logicalDevice, &createInfo, nullptr, &this->m_swapChainImageViews[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image views!");
			}
		}
	}

#pragma endregion

#pragma region Shader Compilation

	void VulkanRenderer::compileShaders() {
		VkShaderCompiler::initializeGlslang();

		const std::string shaderDir = std::filesystem::current_path().string() + "/shaders";
		//std::cout << shaderDir << std::endl;
		const std::string spirvExtension = ".spv";

		std::map<std::string, EShLanguage> shaderTypes = {
			{".vert", EShLangVertex},
			{".frag", EShLangFragment},
			{".geom", EShLangGeometry},
			{".comp", EShLangCompute},
			{".tesc", EShLangTessControl},
			{".tese", EShLangTessEvaluation},
			{".rgen", EShLangRayGen},
			{".rint", EShLangIntersect},
			{".rahit", EShLangAnyHit},
			{".rchit", EShLangClosestHit},
			{".rmiss", EShLangMiss},
			{".rcall", EShLangCallable},
			{".task", EShLangTask},
			{".mesh", EShLangMesh}
		};
		if (!fs::is_directory(shaderDir)) {
			std::cout << "shaders folder is M.I.A." << std::endl;
			exit(1);
			return;
		}
		for (const auto& entry : fs::recursive_directory_iterator(shaderDir)) {
			if (entry.is_regular_file()) {
				std::string ext = entry.path().extension().string();
				if (shaderTypes.count(ext)) {
					fs::path glslPath = entry.path();
					fs::path spirvPath = glslPath;
					spirvPath.replace_extension(glslPath.extension().string() + spirvExtension);

					if (!fs::exists(spirvPath)) {
						std::ifstream shaderFile(glslPath);
						std::string shaderCode((std::istreambuf_iterator<char>(shaderFile)),
							std::istreambuf_iterator<char>());

						EShLanguage shaderType = shaderTypes[ext];

						std::vector<uint32_t> spirvCode = VkShaderCompiler::compileGLSLtoSPIRV(shaderCode, shaderType);
						std::ofstream outputFile(spirvPath, std::ios::out | std::ios::binary);
						m_CompiledShaders.emplace_back(spirvCode);
						outputFile.write(reinterpret_cast<const char*>(spirvCode.data()), spirvCode.size() * sizeof(uint32_t));
					}
					else {
						std::cout << "Found an existing shader output file... attempting to load..." << std::endl;
						std::ifstream file(spirvPath, std::ios::ate | std::ios::binary);
						if (!file.is_open()) {
							std::cerr << "Failed to open file: " << spirvPath << std::endl;
							exit(1);
						}
						size_t fileSize = static_cast<size_t>(file.tellg());
						std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
						file.seekg(0);
						file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
						file.close();
						m_CompiledShaders.emplace_back(buffer);
					}
				}
			}
		}

		VkShaderCompiler::finalizeGlslang();

	}

#pragma endregion


#pragma region Pipeline Creation

	void VulkanRenderer::createGraphicsPipeline() {

	}

#pragma endregion
	void VulkanRenderer::initialize() {
		this->create_vk_instance();
		this->setupGLFWSurface();
		this->pickPhysicalDevice();
		this->createLogicalDevice();
		this->createSwapChain();
		this->createImageViews();
		this->compileShaders();
		this->createGraphicsPipeline();
	}

	void VulkanRenderer::destroy() {
		for (auto imageView : this->m_swapChainImageViews) {
			vkDestroyImageView(this->m_logicalDevice, imageView, nullptr);
		}
		vkDestroySwapchainKHR(this->m_logicalDevice, this->m_swapChain, nullptr);
		vkDestroyDevice(this->m_logicalDevice, nullptr);
		vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
		vkDestroyInstance(this->m_instance, nullptr);
	}
}
#endif