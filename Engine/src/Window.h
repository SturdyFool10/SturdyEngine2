#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/common.hpp>
#include <iostream>
#include <string>
#include<vector>

using glm::tvec2;


namespace SFT {
	class Window;
	namespace input {

#pragma region Event Structs
		struct KeyEvent {
			Window* window;
			int key;
			int scancode;
			int action;
			int mods;
		};

		struct CharEvent {
			Window* window;
			unsigned int codepoint;
		};

		struct CursorPositionEvent {
			Window* window;
			double xpos;
			double ypos;
		};
		struct ClickEvent {
			GLFWwindow* window;
			int button; 
			int action; 
			int mods;
		};

		struct ScrollEvent {
			GLFWwindow* window;
			double xoffset;
			double yoffset;
		};

		struct ResizeEvent {
			GLFWwindow* window;
			tvec2<int> size;
		};

#pragma endregion

#pragma region Handler Type Definitions
		using KeyEventHandler = std::function<void(const KeyEvent&)>;
		using CharEventHandler = std::function<void(const CharEvent&)>;
		using CursorPositionEventHandler = std::function<void(const CursorPositionEvent&)>;
		using ClickEventHandler = std::function<void(const ClickEvent&)>;
		using ScrollEventHandler = std::function<void(const ScrollEvent&)>;
		using ResizeEventHandler = std::function<void(const ResizeEvent&)>;
#pragma endregion

#pragma region Event Handler Storage
		struct KeyHandlerStore {
			KeyEventHandler handler;
			bool active;
		};
		struct CursorPositionHandlerStore {
			CursorPositionEventHandler handler;
			bool active;
		};
		struct ClickHandlerStore {
			ClickEventHandler handler;
			bool active;
		};
		struct ScrollHandlerStore {
			ScrollEventHandler handler;
			bool active;
		};
		struct ResizeHandlerStore {
			ResizeEventHandler handler;
			bool active;
		};
#pragma endregion

	}
/*
TODO:
Cursor Changing / Control
*/

#pragma region window
	class Window {
	private:
#pragma region Class Members
		GLFWwindow* m_window_handle;
		std::vector<input::KeyHandlerStore> m_key_handlers;
		std::vector<input::CursorPositionHandlerStore> m_cursor_position_handlers;
		std::vector<input::ClickHandlerStore> m_click_handlers;
		std::vector<input::ScrollHandlerStore> m_scroll_handlers;
		std::vector<input::ResizeHandlerStore> m_resize_handlers;
#pragma endregion

	public:
#pragma region Constructor / Destructor
		Window(int width, int height, std::string title, bool destroyDefaultSurface = false) {
			if (!glfwInit()) {
				std::cerr << "Failed to initialize window... glfwInit() failed..." << std::endl;
				exit(1);
			}
			if (destroyDefaultSurface) {
				glfwWindowHint(GLFW_NO_API, GLFW_TRUE);
			}
			auto window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
			if (!window) {
				std::cerr << "Failed to initialize window... glfwCreateWindow() failed..." << std::endl;
				exit(1);
			}
			if (glfwRawMouseMotionSupported())
				std::cout << "Raw mouse motion is being used..." << std::endl;
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			this->m_window_handle = window;
			glfwSetKeyCallback(this->m_window_handle, key_callback);
			glfwSetCursorPosCallback(this->m_window_handle, cursor_position_callback);
			glfwSetMouseButtonCallback(this->m_window_handle, mouse_button_callback);
			glfwSetScrollCallback(this->m_window_handle, scroll_callback);
			glfwSetWindowSizeCallback(this->m_window_handle, resize_callback);
			//the line below stores a user pointer, this allows the window class instance to reach the input functions assuming nobody messes with the user pointer
			glfwSetWindowUserPointer(this->m_window_handle, this);
			
			this->m_key_handlers = {};
			this->m_cursor_position_handlers = {};
		}
		Window() {
			int width = 800, height = 600;
			std::string title = "Hello World!";
			bool destroyDefaultSurface = false;
		}
		~Window() {
			if (this->m_window_handle) {
				glfwDestroyWindow(this->m_window_handle);
			}
			glfwTerminate();
		}
#pragma endregion

#pragma region CHandlers
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			input::KeyEvent ev{
				win,
				key,
				scancode,
				action,
				mods
			};
			win->callKeyHandlers(ev);
		}

		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			input::CursorPositionEvent ev{
				win,
				xpos,
				ypos
			};
			win->callCursorPosHandlers(ev);
		}

		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->callClickHandlers({
				window,
				button,
				action,
				mods
			});
		}

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->callScrollHandlers({
				window,
				xoffset,
				yoffset
			});
		}

		static void resize_callback(GLFWwindow* window, int width, int height) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->callResizeHandlers({
				window,
				{width, height}
				});
		}

#pragma endregion

#pragma region Window Properties Management
		void pollEvents();
		void setTitle(const std::string& newTitle);
		void setPosition(const tvec2<size_t> position);
		tvec2<size_t> getPosition();
		tvec2<size_t> getSize();
		GLFWwindow* getHandle();
		bool shouldWindowClose();
		void requestFocus();
		void enablePointerLock();
		void disablePointerLock();
#pragma endregion

#pragma region Event Handling
		void callKeyHandlers(const input::KeyEvent& event);
		void callCursorPosHandlers(const input::CursorPositionEvent& event);
		void callClickHandlers(const input::ClickEvent& event);
		void callScrollHandlers(const input::ScrollEvent& event);
		void callResizeHandlers(const input::ResizeEvent& event);
#pragma endregion

#pragma region Handler Management

		size_t addKeyHandler(input::KeyEventHandler handler);
		size_t addCursorPositionHandler(input::CursorPositionEventHandler handler);
		size_t addClickHandler(input::ClickEventHandler handler);
		size_t addScrollHandler(input::ScrollEventHandler handler);
		size_t addResizeHandler(input::ResizeEventHandler handler);

		void deactivateKeyHandler(size_t index);
		void deactivateCursorPositionHandler(size_t index);
		void deactivateClickHandler(size_t index);
		void deactivateScrollHandler(size_t index);
		void deactivateResizeHandler(size_t index);

		void activateKeyHandler(size_t index);
		void activateCursorPositionHandler(size_t index);
		void activateClickHandler(size_t index);
		void activateScrollHandler(size_t index);
		void activateResizeHandler(size_t index);

#pragma endregion

	};
#pragma endregion

}