#include "Window.h"
namespace SFT {

#pragma region Window Constructor / Destructor
    Window::Window(int width, int height, std::string title, bool destroyDefaultSurface) {
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
        glfwSetWindowSizeLimits(this->m_window_handle, 120, 120, GLFW_DONT_CARE, GLFW_DONT_CARE);

        this->m_key_handlers = {};
        this->m_cursor_position_handlers = {};
    }

    Window::Window() {
        int width = 800, height = 600;
        std::string title = "Hello World!";
        bool destroyDefaultSurface = false;
    }

    Window::~Window() {
        if (this->m_window_handle) {
            glfwDestroyWindow(this->m_window_handle);
        }
        glfwTerminate();
    }
#pragma endregion

#pragma region Window Method Implimentations

#pragma region Window Properties Management

    void Window::pollEvents() {
        glfwPollEvents();
    }

    void Window::setTitle(const std::string& newTitle) {
        glfwSetWindowTitle(this->m_window_handle, newTitle.c_str());
    }

    void Window::setPosition(tvec2<size_t> position) {
        glfwSetWindowPos(this->m_window_handle, position.x, position.y);
    }

    tvec2<size_t> Window::getPosition() {
        int x, y;
        glfwGetWindowPos(this->m_window_handle, &x, &y);
        return tvec2<size_t>(x, y);
    }

    GLFWwindow* Window::getHandle() {
        return this->m_window_handle;
    }

    bool Window::shouldWindowClose() {
        return glfwWindowShouldClose(this->m_window_handle);
    }

    void Window::requestFocus() {
        glfwRequestWindowAttention(this->m_window_handle);
    }

    void Window::enablePointerLock() {
        glfwSetInputMode(this->m_window_handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    void Window::disablePointerLock() {
        glfwSetInputMode(this->m_window_handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    tvec2<size_t> Window::getSize() {
        int x = 0, y = 0;
        glfwGetWindowSize(this->m_window_handle, &x, &y);
        return { x, y };
    }

#pragma endregion

#pragma region Event Handling

    void Window::callKeyHandlers(const input::KeyEvent& event) {
        for (auto& handler : this->m_key_handlers) {
            if (handler.active) {
                input::KeyEvent eve{
                    event.window,
                    event.key,
                    event.scancode,
                    event.action,
                    event.mods,
                    handler.arb
                };
                handler.handler(eve);
            }
        }
    }

    void Window::callCursorPosHandlers(const input::CursorPositionEvent& event) {
        for (auto& handler : this->m_cursor_position_handlers) {
            if (handler.active) {
                input::CursorPositionEvent eve{
                    event.window,
                    event.pos,
                    handler.arb
                };
                handler.handler(eve);
            }
        }
    }

    void Window::callClickHandlers(const input::ClickEvent& event) {
        for (auto& handler : this->m_click_handlers) {
            if (handler.active) {
                input::ClickEvent eve{
                    event.window,
                    event.button,
                    event.action,
                    event.mods,
                    handler.arb
                };
                handler.handler(eve);
            }
        }
    }

    void Window::callScrollHandlers(const input::ScrollEvent& event) {
        for (auto& handler : this->m_scroll_handlers) {
            if (handler.active) {
                input::ScrollEvent eve{
                    event.window,
                    event.offset,
                    handler.arb
                };
                handler.handler(eve);
            }
        }
    }

    void Window::callResizeHandlers(const input::ResizeEvent& event) {
        for (auto& handler : this->m_resize_handlers) {
            if (handler.active) {
                input::ResizeEvent eve{
                    event.window,
                    event.size,
                    handler.arb
                };
                handler.handler(eve);
            }
        }
    }

#pragma endregion

#pragma region Handler Management

    size_t Window::addKeyHandler(input::KeyEventHandler handler) {
        input::KeyHandlerStore store{ handler, true };
        size_t size = this->m_key_handlers.size();
        this->m_key_handlers.push_back(store);
        return size;
    }

    size_t Window::addCursorPositionHandler(input::CursorPositionEventHandler handler) {
        input::CursorPositionHandlerStore store{ handler, true };
        size_t size = this->m_cursor_position_handlers.size();
        this->m_cursor_position_handlers.push_back(store);
        return size;
    }

    size_t Window::addClickHandler(input::ClickEventHandler handler) {
        input::ClickHandlerStore store{ handler, true };
        size_t len = this->m_click_handlers.size();
        this->m_click_handlers.push_back(store);
        return len;
    }

    size_t Window::addScrollHandler(input::ScrollEventHandler handler) {
        input::ScrollHandlerStore store{ handler, true };
        size_t len = this->m_scroll_handlers.size();
        this->m_scroll_handlers.push_back(store);
        return len;
    }

    size_t Window::addResizeHandler(input::ResizeEventHandler handler) {
        input::ResizeHandlerStore store{ handler, true };
        size_t len = this->m_resize_handlers.size();
        this->m_resize_handlers.push_back(store);
        return len;
    }

    void Window::deactivateKeyHandler(size_t index) {
        if (index >= 0 && (index < this->m_key_handlers.size())) {
            this->m_key_handlers[index].active = false;
        }
    }

    void Window::deactivateCursorPositionHandler(size_t index) {
        if (index >= 0 && (index < this->m_cursor_position_handlers.size())) {
            this->m_cursor_position_handlers[index].active = false;
        }
    }

    void Window::deactivateClickHandler(size_t index) {
        if (index >= 0 && (index < this->m_click_handlers.size())) {
            this->m_click_handlers[index].active = false;
        }
    }

    void Window::deactivateScrollHandler(size_t index) {
        if (index >= 0 && (index < this->m_scroll_handlers.size())) {
            this->m_click_handlers[index].active = false;
        }
    }

    void Window::deactivateResizeHandler(size_t index) {
        if (index >= 0 && (index < this->m_resize_handlers.size())) {
            this->m_resize_handlers[index].active = false;
        }
    }

    void Window::activateKeyHandler(size_t index) {
        if (index >= 0 && (index < this->m_key_handlers.size())) {
            this->m_key_handlers[index].active = true;
        }
    }

    void Window::activateCursorPositionHandler(size_t index) {
        if (index >= 0 && (index < this->m_cursor_position_handlers.size())) {
            this->m_cursor_position_handlers[index].active = true;
        }
    }

    void Window::activateClickHandler(size_t index) {
        if (index >= 0 && (index < this->m_click_handlers.size())) {
            this->m_click_handlers[index].active = true;
        }
    }

    void Window::activateScrollHandler(size_t index) {
        if (index >= 0 && (index < this->m_scroll_handlers.size())) {
            this->m_scroll_handlers[index].active = true;
        }
    }

    void Window::activateResizeHandler(size_t index) {
        if (index >= 0 && (index < this->m_resize_handlers.size())) {
            this->m_resize_handlers[index].active = true;
        }
    }

    void Window::setKeyUserPointer(void* ptr, size_t index) {
        if (index >= 0 && (index < this->m_key_handlers.size())) {
            this->m_key_handlers[index].arb = ptr;
        }
    }

    void Window::setCursorUserPointer(void* ptr, size_t index) {
        if (index >= 0 && (index < this->m_cursor_position_handlers.size())) {
            this->m_cursor_position_handlers[index].arb = ptr;
        }
    }

    void Window::setClickUserPointer(void* ptr, size_t index) {
        if (index >= 0 && (index < this->m_click_handlers.size())) {
            this->m_click_handlers[index].arb = ptr;
        }
    }

    void Window::setScrollUserPointer(void* ptr, size_t index) {
        if (index >= 0 && (index < this->m_scroll_handlers.size())) {
            this->m_scroll_handlers[index].arb = ptr;
        }
    }

    void Window::setResizeUserPointer(void* ptr, size_t index) {
        if (index >= 0 && (index < this->m_resize_handlers.size())) {
            this->m_resize_handlers[index].arb = ptr;
        }
    }

#pragma endregion

#pragma endregion

}
