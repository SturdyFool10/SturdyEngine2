#include <GLFW/glfw3.h>

namespace SFT {
	class Renderer {
	private:
		GLFWwindow* window;
	public:
		Renderer() {
			//this exists to make it hassle free to leave undefined members inside other classes
		}

		Renderer(GLFWwindow* window) {
			//renderers have a responsibility to create a surface for the window using its window handle
			this->window = window;
		}

		~Renderer() {
			this->destroy();
		}

		void draw_frame();
		void initialize();
		void destroy();



	};
}