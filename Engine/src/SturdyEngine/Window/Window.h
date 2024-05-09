#pragma once
#ifdef _WIN64
#define GLFW_EXPOSE_NATIVE_WIN32 0
#define GLFW_INCLUDE_VULKAN
#elif defined __linux__

#endif
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
			void* arb = nullptr;
		};

		struct CharEvent {
			Window* window;
			unsigned int codepoint;
			void* arb = nullptr;
		};

		struct CursorPositionEvent {
			Window* window;
			tvec2<double> pos;
			void* arb = nullptr;
		};
		struct ClickEvent {
			GLFWwindow* window;
			int button; 
			int action; 
			int mods;
			void* arb = nullptr;
		};

		struct ScrollEvent {
			GLFWwindow* window;
			tvec2<double> offset;
			void* arb = nullptr;
		};

		struct ResizeEvent {
			GLFWwindow* window;
			tvec2<int> size;
			void* arb = nullptr;
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
			void* arb = nullptr;
		};
		struct CursorPositionHandlerStore {
			CursorPositionEventHandler handler;
			bool active;
			void* arb = nullptr;
		};
		struct ClickHandlerStore {
			ClickEventHandler handler;
			bool active;
			void* arb = nullptr;
		};
		struct ScrollHandlerStore {
			ScrollEventHandler handler;
			bool active;
			void* arb = nullptr;
		};
		struct ResizeHandlerStore {
			ResizeEventHandler handler;
			bool active;
			void* arb = nullptr;
		};
#pragma endregion

	}
/*
TODO:
Cursor Changing / Control
*/

#pragma region Window
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

#pragma region CHandlers
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			input::KeyEvent ev{
				win,
				key,
				scancode,
				action,
				mods,
				nullptr
			};
			win->callKeyHandlers(ev);
		}

		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			input::CursorPositionEvent ev{
				win,
				{
					xpos,
					ypos
				},
				nullptr
			};
			win->callCursorPosHandlers(ev);
		}

		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->callClickHandlers({
				window,
				button,
				action,
				mods,
				nullptr
			});
		}

		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->callScrollHandlers({
				window,
				{
					xoffset,
					yoffset
				},
				nullptr
			});
		}

		static void resize_callback(GLFWwindow* window, int width, int height) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->callResizeHandlers({
				window,
				{
					width,
					height
				},
				nullptr
			});
		}

#pragma endregion

	public:
#pragma region Constructor / Destructor
		Window(int width, int height, std::string title, bool destroyDefaultSurface = false);
		Window();
		~Window();
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

		void setKeyUserPointer(void* ptr, size_t index);
		void setCursorUserPointer(void* ptr, size_t index);
		void setClickUserPointer(void* ptr, size_t index);
		void setScrollUserPointer(void* ptr, size_t index);
		void setResizeUserPointer(void* ptr, size_t index);


#pragma endregion

	};
#pragma endregion

}