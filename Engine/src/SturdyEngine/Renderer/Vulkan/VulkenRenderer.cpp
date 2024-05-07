#include "VulkanRenderer.h"
namespace SFT {
	using VKR = VulkanRenderer; //adding shorthand
	VKR::VulkanRenderer(GLFWwindow* winHandle) {
		this->m_win_handle = winHandle;
	}
}