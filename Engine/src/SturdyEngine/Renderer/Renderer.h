#pragma once
#include "GLFW/glfw3.h"
#include "GLM/common.hpp"
#include "iostream"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
using glm::tvec2;

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

		virtual void draw_frame() {};
		virtual void initialize() {};
		virtual void destroy() {};
		virtual void resize(tvec2<size_t> size) {};
	};
}