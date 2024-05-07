#include "../Renderer.h"
namespace SFT {
	class VulkanRenderer : Renderer {
		GLFWwindow* m_win_handle;
	public:

		VulkanRenderer() = default;
		VulkanRenderer(GLFWwindow* winHandle);
	};
}